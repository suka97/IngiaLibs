#ifndef AmadeusBase_h
#define AmadeusBase_h

#ifdef __AVR__
    #include <avr/pgmspace.h>
    #include <TimerOne.h>
#endif
#include <limits.h>
#include <Arduino.h>
#include <EEPROM.h>

#include "../amadeus_lcd/PrintFlags.h"
#define MAX_MENU_LEN    25

class AmadeusBase
{
public:
    AmadeusBase(const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus) {
        _menus = menus; _valores = valoresDefault; _unids = unids; _valmin = valmin; 
        _valmax = valmax; _cantMenus = cantMenus;
        instance = this;
    }

    // Public Methods: Predefined Menus
    void cambiarVarMostrando(const char *nombre, long *var, long valmin, long valmax, const char *unidad = NULL, bool restart = false);
    bool menuParametros();
    int menu(const char* opciones);
    void resetMenuIndex() { _menuPos = 0; }
    void menuConfig() { while(menuParametros()); }

    // Public Methods: Others
    long getVal(uint8_t i) { 
        long salida;
        EEPROM.get ( sizeof(long)*i, salida ); 
        return salida;
    };
    template <class printTemplate>
    void printf(printTemplate text, uint8_t row, uint8_t printFlag = LCD_CENTER) {
        print( String(text), row, (printFlag&0x3), (printFlag&LCD_CLEAR), 0, !(printFlag&LCD_SKIP_PRINT) );
    }
    void printScreen(String row1, String row2, String row3) {
        print(row1, 0, LCD_CENTER, true, -1, false);
        print(row2, 1, LCD_CENTER, false, -1, false);
        print(row3, 2, LCD_CENTER, false, -1, true);
    }


    // Virtual LCD Methods    
    virtual void lcdClear();
    virtual void clearRow(uint8_t row);
    virtual uint8_t getMaxRows();
    virtual uint8_t getMaxLetters();
    virtual void print(String text, uint8_t row, uint8_t printFlag, boolean clearFlag = false, int startIndex = -1, boolean printNow = true);
    // Virtual Other Methods
    virtual bool cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin = NULL);
    virtual bool cambiarVarIncremental(long *var, long valmin, long valmax, int *flagMaxMin = NULL);


protected:
    const char * const *_menus;
    const long *_valores;
    const char * const *_unids;
    const long *_valmin;
    const long *_valmax;
    long _cantMenus = 0;
    long _menuPos = 0;

    // Internal Methods    
    uint8_t _splitOpciones(char const *str, uint16_t *startIndexes, uint16_t *endIndexes, uint8_t buffSize, char index);

    // Interrupt every 1ms for encoder
    static AmadeusBase *instance;
    static void timer1Isr() {
        instance->onInterrupt();
    };
    virtual void onInterrupt();
    virtual void set_timer(bool);

    // Virtual Protected Methods
    virtual void setIncrementalAcceleration(bool);
    virtual bool enterPressed();
    virtual bool backPressed();
    virtual bool custom1Pressed();
    virtual bool anyKeyPressed();


public:
    // EEPROM
    void EEPROM_Clear() {
        for (unsigned int i = 0 ; i < EEPROM.length() ; i++)
            EEPROM.write(i, 0);
        #ifndef __AVR__
        set_timer(false); EEPROM.commit(); set_timer(true);
        #endif
    }
    void EEPROM_WriteValue(long value, int i) {
        EEPROM.put ( sizeof(long)*i, value );
        #ifndef __AVR__
        set_timer(false); EEPROM.commit(); set_timer(true);
        #endif
    }
    void EEPROM_WriteValues() {
        for ( int i=0 ; i<_cantMenus ; i++ )
            EEPROM.put ( sizeof(long)*i, pgm_read_dword(&_valores[i]) );
        #ifndef __AVR__
        set_timer(false); EEPROM.commit(); set_timer(true);
        #endif
    }

    template <class EEPROM_template>
    void EEPROM_WriteExtras(EEPROM_template buffer) { 
        EEPROM_WriteExtras((char *)&buffer, sizeof(buffer));
    }
    void EEPROM_WriteExtras(char *buffer, uint8_t size) { 
        for ( uint8_t i=0 ; i<size ; i++ )
            EEPROM.put ( (sizeof(long)*_cantMenus)+i, buffer[i] ); 
        #ifndef __AVR__
        set_timer(false); EEPROM.commit(); set_timer(true);
        #endif
    }
    template <class EEPROM_template>
    void EEPROM_GetExtras(EEPROM_template &buffer) {
        EEPROM_GetExtras((char *)&buffer, sizeof(buffer));
    }
    void EEPROM_GetExtras(char *buffer, uint8_t size) {
        for ( uint8_t i=0 ; i<size ; i++ )
            EEPROM.get ( (sizeof(long)*_cantMenus)+i, buffer[i] ); 
    }
    static long mPow(long base, long exp) {
        long salida = 1;
        for ( int i=0 ; i<exp ; i++ )
            salida = salida * base;
        return salida;
    }
};

#endif