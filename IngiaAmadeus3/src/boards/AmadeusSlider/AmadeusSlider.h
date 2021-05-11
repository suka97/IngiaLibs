#ifndef AmadeusSlider_h
#define AmadeusSlider_h

#include "../../libs/amadeus_encoder/AmadeusEncoder.h"
#include "../../libs/accelstepper/AccelStepper.h"
#include "../../libs/amadeus_lcd/AmadeusLCD.h"
#include "../../libs/amadeus_base/AmadeusBase.h"
#include "../../splash_INGIA.h"

class AmadeusSlider_Base
{
// ------------------------------------- PINS -------------------------------------------------
public:
    static const uint8_t 
            PIN_STEPPER_ENABLE     =     13,
            PIN_STEPPER1_PULSO     =     33,
            PIN_STEPPER1_DIRECCION =     35,
            PIN_STEPPER2_PULSO     =     34,
            PIN_STEPPER2_DIRECCION =     14,
            PIN_M_EN               =     16,
            PIN_M_DIR              =     17,
            PIN_M_STEP             =     21,
            PIN_O_BUSY             =     22,
            PIN_I_RESET            =     33,
            PIN_I_IN               =     36,
            PIN_I_SENSOR           =     26, 
            PIN_ENCODER_A          =     32,
            PIN_ENCODER_B          =     15,
            PIN_ENCODER_BUTTON     =     2,
            PIN_LCD_CS             =     5,
            PIN_LCD_DC             =     27,
            PIN_LCD_RES            =     25;

// -------------------------------------------- Public Members -------------------------------------------------
public:
    // Constructor and Initialization
    AmadeusSlider_Base() : stepper(AccelStepper::DRIVER, PIN_M_STEP, PIN_M_DIR) {
        pinMode(PIN_O_BUSY,   OUTPUT);
        pinMode(PIN_I_RESET,  INPUT);
        pinMode(PIN_I_IN,     INPUT);
        pinMode(PIN_I_SENSOR, INPUT);
        pinMode(PIN_M_EN,     OUTPUT);

        stepper.setAcceleration(10000);
        stepper.setMaxSpeed(2000);
    }
    bool begin();

    AccelStepper stepper;
};

#endif