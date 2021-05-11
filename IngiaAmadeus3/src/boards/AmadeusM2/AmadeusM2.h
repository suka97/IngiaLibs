#ifndef AmadeusM2_h
#define AmadeusM2_h

#define MI_FUENTE u8g2_font_courB12_tf //u8g2_font_koleeko_tr  //u8g2_font_courB10_tf

#include "../../libs/amadeus_encoder/AmadeusEncoder.h"
#include "../../libs/accelstepper/AccelStepper.h"
#include "../../libs/amadeus_lcd/AmadeusLCD.h"
#include "../../libs/amadeus_base/AmadeusBase.h"
#include "../../splash_INGIA.h"


class AmadeusM2_Base : public AmadeusLCD, public AmadeusBase
{

// ------------------------------------- PINS -------------------------------------------------
public:
    static const uint8_t 
        #ifdef __AVR__
            PIN_STEPPER_ENABLE     =     12,
            PIN_STEPPER1_PULSO     =     4,
            PIN_STEPPER1_DIRECCION =     5,
            PIN_STEPPER2_PULSO     =     6,
            PIN_STEPPER2_DIRECCION =     7,
            PIN_IO_A               =     2,
            PIN_IO_B               =     A2,
            PIN_IO_C               =     A4,
            PIN_IO_D               =     3,
            PIN_IO_E               =     A3,
            PIN_IO_F               =     A5, 
            PIN_ENCODER_A          =     A1,
            PIN_ENCODER_B          =     A0,
            PIN_ENCODER_BUTTON     =     1,
            PIN_LCD_CS             =     10,
            PIN_LCD_DC             =     9,
            PIN_LCD_RES            =     8;
        #else
            PIN_STEPPER_ENABLE     =     13,
            PIN_STEPPER1_PULSO     =     33,
            PIN_STEPPER1_DIRECCION =     35,    //X IO19
            PIN_STEPPER2_PULSO     =     34,    // NO VAN LOS ADC
            PIN_STEPPER2_DIRECCION =     14,
            PIN_IO_A               =     26, 
            PIN_IO_B               =     16,
            PIN_IO_C               =     21,
            PIN_IO_D               =     4,     //X IO35 
            PIN_IO_E               =     17, 
            PIN_IO_F               =     22, 
            PIN_ENCODER_A          =     32,    //X SD3
            PIN_ENCODER_B          =     15,    //X CMD
            PIN_ENCODER_BUTTON     =     2,     //X SD2
            PIN_LCD_CS             =     5,
            PIN_LCD_DC             =     27,
            PIN_LCD_RES            =     25;
        #endif

// ---------------------------------- Private Members -------------------------------------------------
protected:
    // Hardware Dependant Constants
    static const uint8_t ENCODER_STEPS_PER_NOTCH = 4;

    // Internal Pointers and Variables
    void (*_customInterrupt_Pointer)() = NULL;
    AmadeusEncoder::Button _encButtonStatus = AmadeusEncoder::Open;


// -------------------------------------------- Public Members -------------------------------------------------
public:
    // Constructor and Initialization
    AmadeusM2_Base( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus, 
        AmadeusEncoder *rotaryEncoder, U8G2 *u8g2 ) :    // por si hay cambio de hardware
            AmadeusLCD(u8g2),
            AmadeusBase(menus, valoresDefault, unids, valmin, valmax, cantMenus)
    {
        encoderPtr = rotaryEncoder;
    }
    bool begin();


    // Public Methods: Predefined Menus
    bool login(char const *password);
    void jogEncoder_Base(AccelStepper *stepper, String title="", float defaultSpeed = 2000);


    // Public Methods: Others
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
    // Timer config for ESP32
    #ifndef __AVR__     
        hw_timer_t * timer = NULL;
        void set_timer(bool state) {
            if(!state) {
                if(timer) timerEnd(timer);
                timer = NULL;
            } else {
                timer = timerBegin(3, 80, 1);//div 80
                timerAttachInterrupt(timer, &timer1Isr, 1);
                timerAlarmWrite(timer, 1000, true);//1ms
                timerAlarmEnable(timer);
            }
        }
    #else
        void set_timer(bool state) {  }
    #endif


    // Public Variables and Classes
    AmadeusEncoder *encoderPtr;


// -------------------------------------------- Virtual Members -------------------------------------------------
public:
    // AmadeusLCD call virtual methods
    void lcdClear() { AmadeusLCD::lcdClear(); }
    void clearRow(uint8_t row) { AmadeusLCD::clearRow(row); }
    uint8_t getMaxRows() { return AmadeusLCD::getMaxRows(); }
    uint8_t getMaxLetters() { return AmadeusLCD::getMaxLetters(); }
    void print(String text, uint8_t row, uint8_t printFlag, boolean clearFlag = false, int startIndex = -1, boolean printNow = true) {
        AmadeusLCD::print(String(text), row, printFlag, clearFlag, startIndex, printNow);
    }
    template <class printTemplate>
    void print(printTemplate text, uint8_t row, uint8_t printFlag, boolean clearFlag = false, int startIndex = -1, boolean printNow = true) {
        print(String(text), row, printFlag, clearFlag, startIndex, printNow);  
    }
    // Other virtual public methods
    bool cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin = NULL);
    bool cambiarVarIncremental(long *var, long valmin, long valmax, int *flagMaxMin = NULL) { 
        return cambiarVar(var, valmin, valmax, flagMaxMin); 
    }
    void cambiarString(const char *title, char *buffer, uint8_t size, char *opc = NULL);
    bool askYesNo(const char *title);

public:
    void setIncrementalAcceleration(bool en) { encoderPtr->setAccelerationEnabled(en); }
    bool enterPressed() {  
        if( _encButtonStatus == AmadeusEncoder::Clicked ) {
            while( encoderPtr->getButton(true) == AmadeusEncoder::Clicked )
                _encButtonStatus = encoderPtr->getButton();
            encoderPtr->getButton(true);
            return true;
        }
        else
            return false;  
    }
    bool backPressed() { 
        if( _encButtonStatus == AmadeusEncoder::DoubleClicked ) {
            while( _encButtonStatus == AmadeusEncoder::DoubleClicked )
                _encButtonStatus = encoderPtr->getButton();
            encoderPtr->getButton();
            return true;
        }
        else
            return false; 
    }
    bool custom1Pressed() { return encoderPtr->isHeld(); }
    bool anyKeyPressed() { return (backPressed() || enterPressed()); }
    void onInterrupt() {
        encoderPtr->service();
        _encButtonStatus = encoderPtr->getButton(false);
        if( _customInterrupt_Pointer != NULL )
            _customInterrupt_Pointer();
    };
};


class AmadeusM2_NoDrivers : public AmadeusM2_Base
{
protected:
    U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2;

public: 
    AmadeusM2_NoDrivers( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus )
        : AmadeusM2_Base(menus, valoresDefault, unids, valmin, valmax, cantMenus, &encoder, &u8g2),
        u8g2(U8G2_R0, PIN_LCD_CS, PIN_LCD_DC, PIN_LCD_RES),
        encoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_BUTTON, ENCODER_STEPS_PER_NOTCH, LOW)
    {  }
    AmadeusEncoder encoder;
};



class AmadeusM2 : public AmadeusM2_NoDrivers
{
public:
    AmadeusM2( const char * const *menus, const long *valoresDefault, const char * const *unids, const long *valmin, const long *valmax, const int cantMenus )
        : AmadeusM2_NoDrivers( menus, valoresDefault, unids, valmin, valmax, cantMenus ),
        stepper1(AccelStepper::DRIVER, PIN_STEPPER1_PULSO, PIN_STEPPER1_DIRECCION),
        stepper2(AccelStepper::DRIVER, PIN_STEPPER2_PULSO, PIN_STEPPER2_DIRECCION)
    {   }

    void jogEncoder(uint8_t motorNumber, float defaultSpeed = 2000);
    AccelStepper stepper1, stepper2;
};

#endif