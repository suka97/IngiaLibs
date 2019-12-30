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


class IngiaAmadeus2 : public AmadeusLCD
{

// ---------------------------------- Private Members -------------------------------------------------
private:
    // Hardware Dependant Constants
    static const uint8_t ENCODER_STEPS_PER_NOTCH = 4;


    // Interrupt every 1ms for encoder
    static IngiaAmadeus2 *instance;
    static void timer1Isr() {
        instance->onInterrupt();
    };
    void onInterrupt() {
        encoder.service();
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
    int _cantMenus = 0;
    uint16_t _xPrintIndex = 0, _yPrintIndex = 0;


    // Internal Methods    
    uint8_t _getOpcionesSize(char const *str, char index);
    uint8_t _splitOpciones(char const *str, char** buffer, uint8_t wordSize, char index);




// -------------------------------------------- Public Members -------------------------------------------------
public:
    // Constructor and Initialization
    IngiaAmadeus2( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus, 
        uint8_t pinEnc_A = PIN_ENCODER_A, uint8_t pinEnc_B = PIN_ENCODER_B ):   // por si hay que invertir el encoder...
        AmadeusLCD(U8G2_R0, PIN_LCD_CS, PIN_LCD_DC, PIN_LCD_RES),
        stepper1(AccelStepper::DRIVER, PIN_STEPPER1_PULSO, PIN_STEPPER1_DIRECCION),
        stepper2(AccelStepper::DRIVER, PIN_STEPPER2_PULSO, PIN_STEPPER2_DIRECCION),
        encoder(pinEnc_A, pinEnc_B, PIN_ENCODER_BUTTON, ENCODER_STEPS_PER_NOTCH, LOW) 
        {
            _menus = menus; _valores = valoresDefault; _unids = unids; _valmin = valmin; 
            _valmax = valmax; _cantMenus = cantMenus;
            instance = this;
            stepper[0] = &stepper1; stepper[1] = &stepper2;   // stepper array initialization
        }
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
    AccelStepper stepper1;
    AccelStepper stepper2;
    AccelStepper *stepper[2];
    AmadeusEncoder encoder;
};

#endif