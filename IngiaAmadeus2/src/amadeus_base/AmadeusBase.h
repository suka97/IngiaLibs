#ifndef AmadeusBase_h
#define AmadeusBase_h

#include <avr/pgmspace.h>
#include <limits.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <TimerOne.h>

#include "../amadeus_lcd/PrintFlags.h"

class AmadeusBase
{
public:
    AmadeusBase(const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus) {
        _menus = menus; _valores = valoresDefault; _unids = unids; _valmin = valmin; 
        _valmax = valmax; _cantMenus = cantMenus;
        instance = this;
    }

    // Public Methods: Predefined Menus
    void cambiarVarMostrando(char *nombre, long *var, long valmin, long valmax, char *unidad = NULL, bool restart = false);
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
    uint8_t _getOpcionesSize(char const *str, char index);
    uint8_t _splitOpciones(char const *str, char** buffer, uint8_t wordSize, char index);

    // Interrupt every 1ms for encoder
    static AmadeusBase *instance;
    static void timer1Isr() {
        instance->onInterrupt();
    };
    virtual void onInterrupt();

    // Virtual Protected Methods
    virtual void setIncrementalAcceleration(bool);
    virtual bool enterPressed();
    virtual bool backPressed();


public:
    // EEPROM
    void EEPROM_Clear() {
        for (unsigned int i = 0 ; i < EEPROM.length() ; i++)
            EEPROM.write(i, 0);
    }
    void EEPROM_WriteValue(long value, int i) {
        EEPROM.put ( sizeof(long)*i, value );
    }
    void EEPROM_WriteValues() {
        for ( int i=0 ; i<_cantMenus ; i++ )
            EEPROM.put ( sizeof(long)*i, pgm_read_dword(&_valores[i]) );
    }
    static long mPow(long base, long exp) {
        long salida = 1;
        for ( int i=0 ; i<exp ; i++ )
            salida = salida * base;
        return salida;
    }
};

#endif