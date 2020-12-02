#ifndef IngiaAmadeus2_h
#define IngiaAmadeus2_h

#define MI_FUENTE u8g2_font_courB12_tf //u8g2_font_koleeko_tr  //u8g2_font_courB10_tf

#include <avr/pgmspace.h>
#include <limits.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <IngiaStepper.h>

#include "amadeus_encoder/AmadeusEncoder.h"
#include "accelstepper/AccelStepper.h"
#include "amadeus_lcd/AmadeusLCD.h"
#include "splash_INGIA.h"
#include "IngiaAmadeus2_PINS.h"

// Constants
typedef enum {
    NO_FLAGS   = 0x0,
    NO_DRIVERS = 0x1
} AmadeusFlags_t;


class IngiaAmadeus2_old : public AmadeusLCD
{

// ---------------------------------- Private Members -------------------------------------------------
private:
    // Hardware Dependant Constants
    static const uint8_t ENCODER_STEPS_PER_NOTCH = 4;


    // Interrupt every 1ms for encoder
    static IngiaAmadeus2_old *instance;
    static void timer1Isr() {
        instance->onInterrupt();
    };
    void onInterrupt() {
        encoder->service();
        if( _customInterrupt_Pointer != NULL )
            _customInterrupt_Pointer();
    };
    // Timer config for ESP32
    #ifndef __AVR__     
        hw_timer_t * timer = NULL;
        void toggleTimer() {
            if(timer) {
                timerEnd(timer);
                timer = NULL;
            } else {
                timer = timerBegin(3, 80, 1);//div 80
                timerAttachInterrupt(timer, &timer1Isr, 1);
                timerAlarmWrite(timer, 1000, true);//1ms
                timerAlarmEnable(timer);
            }
        }
    #endif


    // Internal Pointers and Variables
    void (*_customInterrupt_Pointer)() = NULL;
    const char * const *_menus;
    const long *_valores;
    const char * const *_unids;
    const long *_valmin;
    const long *_valmax;
    int _cantMenus = 0, _cantMotores;
    int _menuPos = 0;
    uint16_t _xPrintIndex = 0, _yPrintIndex = 0;


    // Internal Methods    
    uint8_t _getOpcionesSize(char const *str, char index);
    uint8_t _splitOpciones(char const *str, char** buffer, uint8_t wordSize, char index);




// -------------------------------------------- Public Members -------------------------------------------------
public:
    // Constructor and Initialization
    IngiaAmadeus2_old( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus, 
        AmadeusEncoder *rotaryEncoder = NULL,     // por si hay que invertir el encoder ...
        U8G2 *u8g2 = NULL, AccelStepper **stepperArray = NULL, int cantMotores = 1 ):        // por si no lo uso fuera del hardware Amadeus
            AmadeusLCD() 
        {
            if ( u8g2 == NULL )
                _setU8G2( new U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI(U8G2_R0, PIN_LCD_CS, PIN_LCD_DC, PIN_LCD_RES) );
            else 
                _setU8G2( u8g2 );
            if ( stepperArray == NULL ) {
                stepper = new AccelStepper *[2];
                stepper[0] = new AccelStepper(AccelStepper::DRIVER, PIN_STEPPER1_PULSO, PIN_STEPPER1_DIRECCION);
                stepper[1] = new AccelStepper(AccelStepper::DRIVER, PIN_STEPPER2_PULSO, PIN_STEPPER2_DIRECCION);
                _cantMotores = 2;
            }
            else {
                _cantMotores = cantMotores;
                stepper = stepperArray;
            }

            if ( rotaryEncoder == NULL )
                encoder = new AmadeusEncoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON, ENCODER_STEPS_PER_NOTCH, LOW);
            else
                encoder = rotaryEncoder;

            _menus = menus; _valores = valoresDefault; _unids = unids; _valmin = valmin; 
            _valmax = valmax; _cantMenus = cantMenus;
            instance = this;
        }
    IngiaAmadeus2_old(const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus, 
        AmadeusFlags_t flags) :
            AmadeusLCD()
        {
            _setU8G2( new U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI(U8G2_R0, PIN_LCD_CS, PIN_LCD_DC, PIN_LCD_RES) );
            encoder = new AmadeusEncoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON, ENCODER_STEPS_PER_NOTCH, LOW);

            if ( flags & NO_DRIVERS ) {
                _cantMotores = 0;
            }
            else {
                stepper = new AccelStepper *[2];
                stepper[0] = new AccelStepper(AccelStepper::DRIVER, PIN_STEPPER1_PULSO, PIN_STEPPER1_DIRECCION);
                stepper[1] = new AccelStepper(AccelStepper::DRIVER, PIN_STEPPER2_PULSO, PIN_STEPPER2_DIRECCION);
                _cantMotores = 2;
            }

            _menus = menus; _valores = valoresDefault; _unids = unids; _valmin = valmin; 
            _valmax = valmax; _cantMenus = cantMenus;
            instance = this;
        }
    boolean begin_CustomHardWare();
    boolean begin( uint8_t pinModeA = OUTPUT, uint8_t pinModeB = OUTPUT, uint8_t pinModeC = OUTPUT, 
        uint8_t pinModeD = OUTPUT, uint8_t pinModeE = OUTPUT, uint8_t pinModeF = OUTPUT);


    // Public Methods: Stepper Control 
    void move(uint8_t motorNumber, long pasos);
    void setSteppersEnable(boolean en) {
        digitalWrite(PIN_STEPPER_ENABLE, en);
    }


    // EEPROM
    void EEPROM_Clear();
    void EEPROM_WriteValue(long value, int i);
    void EEPROM_WriteValues();
    static long mPow(long base, long exp) {
        long salida = 1;
        for ( int i=0 ; i<exp ; i++) {
            salida = salida * base;
        }
        return salida;
    }


    // Public Methods: Predefined Menus
    boolean menuParametros();
    void jogEncoder(uint8_t motorNumber = 1, float defaultSpeed = 2000);
    int menu(const char* opciones);
    void resetMenuIndex() { _menuPos = 0; }
    boolean cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin = NULL);
    void cambiarVarMostrando(char *nombre, long *var, long valmin, long valmax, char *unidad = NULL, boolean restart = false);
    void menuConfig() { while(menuParametros()); }
    boolean login(char const *password);


    // Public Methods: Others
    long getVal(uint8_t i) { 
        long salida;
        EEPROM.get ( sizeof(long)*i, salida ); 
        return salida;
    };
    void attachinterrupt(void (*func)()) {
        _customInterrupt_Pointer = func;
    }
    void changeFont(uint8_t n) {
        switch (n) {
            case 1:
                _changeFont(u8g2_font_inb33_mn, 0, 0, 7);
                break;
            default:
                _changeFont(MI_FUENTE, 5, 0, 0);  // default
                break;
        }
    }


    // Public Variables and Classes
    AccelStepper **stepper;
    AmadeusEncoder *encoder;
};

#endif