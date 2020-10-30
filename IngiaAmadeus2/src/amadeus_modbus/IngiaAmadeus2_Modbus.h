#ifndef IngiaAmadeus2_Modbus_h
#define IngiaAmadeus2_Modbus_h

#include "../amadeus_base/AmadeusBase.h"
#include "ModbusMaster/ModbusMaster.h"
#include "ModbusHMI_registers.h"

#define TECLA_MENOS         lastKeysState[3][0]    // *
#define TECLA_MENOS_MASK    (uint16_t)0x1000
#define TECLA_BORRAR        lastKeysState[1][3]    // B
#define TECLA_BORRAR_MASK   (uint16_t)0x0080
#define TECLA_ENTER         lastKeysState[2][3]    // C
#define TECLA_ENTER_MASK    (uint16_t)0x0800
#define TECLA_ATRAS         lastKeysState[3][3]    // D
#define TECLA_ATRAS_MASK    (uint16_t)0x8000
#define TECLA_ARRIBA        lastKeysState[0][3]    // A
#define TECLA_ARRIBA_MASK   (uint16_t)0x0008
#define TECLA_ABAJO         lastKeysState[1][3]    // B
#define TECLA_ABAJO_MASK    (uint16_t)0x0080
// LOS NUMEROS
#define TECLA_1_MASK    (uint16_t)0x0001
#define TECLA_2_MASK    (uint16_t)0x0002
#define TECLA_3_MASK    (uint16_t)0x0004
#define TECLA_4_MASK    (uint16_t)0x0010
#define TECLA_5_MASK    (uint16_t)0x0020
#define TECLA_6_MASK    (uint16_t)0x0040
#define TECLA_7_MASK    (uint16_t)0x0100
#define TECLA_8_MASK    (uint16_t)0x0200
#define TECLA_9_MASK    (uint16_t)0x0400
#define TECLA_0_MASK    (uint16_t)0x2000


struct KeyboardState {
    KeyState key_1; KeyState key_2; KeyState key_3; KeyState key_A;
    KeyState key_4; KeyState key_5; KeyState key_6; KeyState key_B;
    KeyState key_7; KeyState key_8; KeyState key_9; KeyState key_C;
    KeyState key_X; KeyState key_0; KeyState key_H; KeyState key_D;
};
struct KeysClickedReleased {
    uint16_t clicked;
    uint16_t released;
};

class AmadeusScreenBuffer
{
protected:
    uint8_t *_log_buffer;
    uint8_t _bufSize_X, _bufSize_Y;
    uint8_t _index_X=0, _index_Y=0;

    void _clearBuffer() {
        for (uint16_t j=0 ; j<(_bufSize_Y) ; j++)
            _bufferRenglonVacio(j);
    }
    void _bufferRenglonVacio(uint8_t row) {
        for (uint8_t i=0 ; i<(_bufSize_X-1) ; i++)
            _log_buffer[(row*_bufSize_X)+i] = ' ';
        _log_buffer[(row*_bufSize_X)+_bufSize_X-1] = '\0';
    }

public:
    AmadeusScreenBuffer(uint8_t *buf, uint8_t ySize, uint8_t xSize) {
        _log_buffer = buf; _bufSize_X = xSize; _bufSize_Y = ySize; 
        _clearBuffer();
    }
    void clearRow(uint8_t r) { _bufferRenglonVacio(r); }
    uint8_t getMaxRows() { return _bufSize_Y; }
    uint8_t getMaxLetters() { return (_bufSize_X-1); }
    void print(String str, uint8_t row, uint8_t printFlag = LCD_CENTER) {
        if ( printFlag & LCD_CLEAR)
            _clearBuffer();
        _index_Y = (row < getMaxRows()) ? row : (getMaxRows()-1);

        if ( printFlag & LCD_RIGHT ) 
            _index_X = _bufSize_X - str.length() - 1;
        else if ( printFlag & LCD_CENTER )
            _index_X = (_bufSize_X - str.length()) / 2;
        else
            _index_X = 0;   // LCD_LEFT

        for (uint8_t i=0 ; (_index_X<getMaxLetters()) && (i<str.length()) ; i++) {
            _log_buffer[(_index_Y*_bufSize_X)+_index_X] = (uint8_t)str.charAt(i);
            _index_X++;
        }
    }
};


class IngiaAmadeus2_Modbus : public AmadeusBase
{
protected:
    ModbusMaster modbus;
    AmadeusScreenBuffer screenBuffer;
    uint8_t buffer[3][LETTERS_X_ROW+1];
    void (*_customInterrupt_Pointer)() = NULL;
    bool flagRequestKeys = false;

// Modbus Methods and Variables
protected:
    uint32_t butStatus;
    bool sendScreenBuffer();
    bool incAcceleration = false;
    KeyState lastKeysState[4][4];
    KeysClickedReleased lastKeysClickedReleased;

public:
    IngiaAmadeus2_Modbus( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus 
        ) : AmadeusBase(menus, valoresDefault, unids, valmin, valmax, cantMenus),
            screenBuffer(buffer[0], 3, LETTERS_X_ROW+1)
    {  }
    bool begin(uint8_t hmiID, Stream &serial);
    KeyboardState getKeyboardState();
    void getKeyboardState(KeyState arr_4x4[][4]);
    KeysClickedReleased getKeysClickedReleased(bool reset = true);
    void modbusService() {
        if ( flagRequestKeys ) { 
            flagRequestKeys = false;
            lastKeysClickedReleased = getKeysClickedReleased();
            getKeyboardState(lastKeysState);
        }
    }


// -------------------------------------------- Virtual Members -------------------------------------------------
public:
    // AmadeusLCD call virtual methods
    void lcdClear() { 
        screenBuffer.clearRow(0);
        screenBuffer.clearRow(1);
        screenBuffer.clearRow(2);
        sendScreenBuffer();
    }
    void clearRow(uint8_t row) { 
        screenBuffer.clearRow(row); sendScreenBuffer();
    }
    uint8_t getMaxRows() { return 3; }
    uint8_t getMaxLetters() { return LETTERS_X_ROW; }
    void print(String text, uint8_t row, uint8_t printFlag, boolean clearFlag = false, int startIndex = -1, boolean printNow = true) {
        screenBuffer.print( text, row, printFlag|((clearFlag)?LCD_CLEAR:0) ); 
        if ( printNow )
            sendScreenBuffer();
    }
    // Other virtual public methods
    bool cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin = NULL);
    bool cambiarVarIncremental(long *var, long valmin, long valmax, int *flagMaxMin = NULL);

    
protected:
    void setIncrementalAcceleration(bool en) { incAcceleration = en; }
    bool enterPressed() { return (TECLA_ENTER_MASK & lastKeysClickedReleased.clicked); }
    bool backPressed() { return (TECLA_ATRAS_MASK & lastKeysClickedReleased.clicked); }
    void onInterrupt() {
        // cada 50ms reviso el teclado
        static uint32_t cont = 0;
        if ( cont >= 50 ) {
            cont = 0; flagRequestKeys = true;
        }
        else
            cont++;

        if( _customInterrupt_Pointer != NULL )
            _customInterrupt_Pointer();
    };
};

#endif