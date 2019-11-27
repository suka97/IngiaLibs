#ifndef IngiaAmadeus_h
#define IngiaAmadeus_h

#include <AnalogMultiButton.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <AccelStepper.h>

#define PIN_BUTTONS A0
#define PIN_STEPPER_ENABLE 10
#define PIN_STEPPER_PULSO 11
#define PIN_STEPPER_DIRECCION 12
#define PIN_LCD_RS 8
#define PIN_LCD_EN 9
#define PIN_LCD_D4 4
#define PIN_LCD_D5 5
#define PIN_LCD_D6 6
#define PIN_LCD_D7 7
#define PIN_INPUT_A1 A1
#define PIN_INPUT_A2 A2
#define PIN_INPUT_A3 A3

#define BUTTONS_TOTAL 5
#define LCD_CANT_CARACTERES 16

class IngiaAmadeus
{

private:
    const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0 , 132, 320, 487 , 726 };
    LiquidCrystal _lcd;
    char **_menus;
    long *_valores;
    char **_unids;
    long *_valmin;
    long *_valmax;
    int _cantMenus = 0;
    uint8_t _pinInOut[3] = {PIN_INPUT_A1, PIN_INPUT_A2, PIN_INPUT_A3};

public:
    IngiaAmadeus(char **menus, long *valores, char **unids, long *valmin, long *valmax, int cantMenus,
        int pinStep = PIN_STEPPER_PULSO, int pinDir = PIN_STEPPER_DIRECCION): 
        stepper(AccelStepper::DRIVER, pinStep, pinDir),
        _lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7),
        buttons(PIN_BUTTONS, BUTTONS_TOTAL, BUTTONS_VALUES) 
        {
            _menus = menus; _valores = valores; _unids = unids; _valmin = valmin; 
            _valmax = valmax; _cantMenus = cantMenus;
        }

    AccelStepper stepper;
    boolean begin(boolean writeArraysEEPROM = false);
    void setCantMenus(int cant) { _cantMenus = cant; }
    long getVal(int i) { return _valores[i]; }
    boolean cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin = NULL, boolean addDelay = false);
    void menuParametros();
    typedef enum {
        BUTTON_LEFT = 0,
        BUTTON_UP = 1,
        BUTTON_DOWN = 2,
        BUTTON_RIGHT = 3,
        BUTTON_SELECT = 4
    } Button;
    AnalogMultiButton buttons;
    void setInOut( uint8_t n, uint8_t mode, int newPin = -1 ) {
        if ( newPin != (-1) ) {
            _pinInOut[n-1] = newPin;
            pinMode(newPin, mode);
        }
        else
            pinMode(_pinInOut[n-1], mode);
    }

    // MANEJO DEL LCD -------------------------------------------------------------------
    typedef enum {
        LCD_LEFT = 0,
        LCD_RIGHT = 1,
        LCD_CENTER = 2,
    } PrintFlag;
    void print(char *str, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1);
    void print(long n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        char bufferStr[16];
        print(longToString(n, bufferStr), row, printFlag, clearFlag, startIndex);
    }
    void print(int n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        print((long)n, row, printFlag, clearFlag, startIndex);
    }
    void print(float n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        char bufferStr[16], auxStr[16];
        long aux = (long)(n * 100);
        longToString(aux/100, bufferStr); strcat(bufferStr, "."); strcat(bufferStr, longToString(aux%100, auxStr));
        print(bufferStr, row, printFlag, clearFlag, startIndex);
    }
    void print(double n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        print((float)n, row, printFlag, clearFlag, startIndex);
    }
    void println(char *str, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false);
    void println(long n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false) {
        char bufferStr[16];
        println(longToString(n, bufferStr), row, printFlag, clearFlag);
    }
    void println(int n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        println((long)n, row, printFlag, clearFlag);
    }
    void println(float n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        char bufferStr[16], auxStr[16];
        long aux = (long)(n * 100);
        longToString(aux/100, bufferStr); strcat(bufferStr, "."); strcat(bufferStr, longToString(aux%100, auxStr));
        println(bufferStr, row, printFlag, clearFlag);
    }
    void println(double n, int row = 0, PrintFlag printFlag = LCD_LEFT, boolean clearFlag = false, int startIndex = -1) {
        println((float)n, row, printFlag, clearFlag);
    }
    void lcdClear(boolean mantenerPos = false);
    // ------------------------------------------------------------------------------

    void EEPROM_Clear();
    void EEPROM_ReadValues();
    void EEPROM_WriteValue(long value, int i);
    void EEPROM_WriteValues();
    static const char *LCD_RENGLON_LIMPIO;
    static char * longToString(long n, char *str);
    static long mPow(long base, long exp) {
        long salida = 1;
        for ( int i=0 ; i<exp ; i++) {
            salida = salida * base;
        }
        return salida;
    }
    boolean getEstadoInput(int n) { return digitalRead(_pinInOut[n-1]); }
    void setEstadoOutput(int n, uint8_t estado) { digitalWrite(_pinInOut[n-1], estado); }
    boolean jog(float defaultSpeed = 2000, Button buttonEsc = -1, boolean restart = false);
    boolean updateAndCheckButton_OnPress(int button) {
        buttons.update();
        return buttons.onPress(button);
    }

};

#endif