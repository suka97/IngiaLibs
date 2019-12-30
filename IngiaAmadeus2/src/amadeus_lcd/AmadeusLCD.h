#ifndef AmadeusLCD_h
#define AmadeusLCD_h

#include "AmadeusLCD_SpecialChars.h"
#include "u8g2lib/U8g2lib.h"


// Constants
typedef enum {
    LCD_LEFT = 0,
    LCD_RIGHT = 1,
    LCD_CENTER = 2
} PrintFlag_t;


class AmadeusLCD
{
// -------------------------------------------- Public Members -------------------------------------------------
public:
    // Constructor, Deconstructor and Initialization
    AmadeusLCD(const u8g2_cb_t* r, int cs, int dc, int res) : _u8g2(r, cs, dc, res) { }
    ~AmadeusLCD() { _freeBuffer(); };
    void init(const uint8_t *font, uint8_t charsExtra = 0, uint8_t rowsExtra = 0, int espacioTop = -1);
    

    // Drawing Methods
    void drawBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t crop = 0);
    void drawBitmap_Curtain(const uint8_t *bitmap, uint16_t width, uint16_t height, unsigned long millisLength = 1000, unsigned long millisRefresh = 50);
    void setCursor(uint8_t x, uint8_t y) { _index_X=x; _index_Y=y; };
    void lcdClear();
    void flipScreenColor() {
        _colorFliped = (_colorFliped) ? false : true;
        _sendBuffer();
    }
    void setScreenColor(uint8_t n) {
        _colorFliped = (n==1) ? true : false;
    }
    void clearRow(uint8_t r) {
        _bufferRenglonVacio(r);
        print("", r, LCD_LEFT, false);
    }


    // Getters
    uint16_t getEspacioLetter() { return (_u8g2.getDisplayWidth() / _bufSize_X); };
    uint8_t getMaxLetters() { return (_bufSize_X); }
    uint8_t getMaxRows() { return _bufSize_Y; }
    void setRain(boolean r, uint8_t numRains = 10);
    uint16_t getAlturaRenglon(uint8_t renglon) {
        return ( renglon * (_u8g2.getMaxCharHeight() +  ESPACIO_EXTRA_RENGLON) + _espacioTop );
    }
    

    // Print
    template <class printTemplate>
    void print(printTemplate text, uint8_t row = 0, PrintFlag_t printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1, boolean printNow = true) {
        String str = String(text);

        if (clearFlag) {
            _clearBuffer();
        }
        _index_Y = (row < getMaxRows()) ? row : (getMaxRows()-1);

        switch (printFlag) {
            case LCD_LEFT:
                if ( startIndex != -1 )
                    _index_X = startIndex;
                break;
            case LCD_RIGHT:
                _index_X = _bufSize_X - str.length() - 1;
                break;
            case LCD_CENTER:
                _index_X = (_bufSize_X - str.length()) / 2;
                break;
        }

        for (uint8_t i=0 ; (_index_X<getMaxLetters()) && (i<str.length()) ; i++) {
            _log_buffer[_index_Y][_index_X] = (uint8_t)str.charAt(i);
            _index_X++;
        }
        if ( printNow )
            _sendBuffer();
    }
    
    

// ---------------------------------------- Protected Members -----------------------------------------
protected:
    // Hardware Dependant Constants
    static const uint8_t ESPACIO_EXTRA_RENGLON = 2;


    // Internal Pointers and Variables
    U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI _u8g2;
    uint8_t **_log_buffer;
    uint8_t _bufSize_X, _bufSize_Y;
    uint8_t _index_X=0, _index_Y=0;
    uint8_t _espacioTop;
    boolean _colorFliped = false;
    // RAIN
    boolean _raining = false;
    uint8_t _numRains;
    uint16_t ** _rainArray;


    // Protected Methods
    void _clearBuffer();
    void _sendBuffer();
    void _bufferRenglonVacio(uint8_t row);
    uint8_t _verExcepciones(char letter);
    void _makeBuffer(uint8_t charsExtra = 0, uint8_t rowsExtra = 0);
    void _freeBuffer();
    void _clearScreen();
    void _changeFont(const uint8_t *font, uint8_t charsExtra = 0, uint8_t rowsExtra = 0, int espacioTop = -1);
};

#endif