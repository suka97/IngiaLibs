#include <AccelStepper.h>

// IngiaAmadeus PINS
#define PIN_STEPPER_ENABLE          12
#define PIN_STEPPER1_PULSO          4
#define PIN_STEPPER1_DIRECCION      5
#define PIN_STEPPER2_PULSO          6
#define PIN_STEPPER2_DIRECCION      7
#define PIN_IO_A                    2 
#define PIN_IO_B                    A2
#define PIN_IO_C                    A4
#define PIN_IO_D                    3 
#define PIN_IO_E                    A3 
#define PIN_IO_F                    A5 
#define PIN_ENCODER_A               A0
#define PIN_ENCODER_B               A1
#define PIN_ENCODER_BUTTON          1
#define PIN_LCD_CS                  10
#define PIN_LCD_DC                  9
#define PIN_LCD_RES                 8

// Custom PINS
#define PIN_PUL PIN_IO_A
#define PIN_DIR PIN_IO_B

// Globals
AccelStepper stepper(AccelStepper::DRIVER, PIN_PUL, PIN_DIR);

void setup() {
    pinMode(PIN_IO_C, OUTPUT);

    stepper.setAcceleration(10000);  
    stepper.setMaxSpeed(2000);
    stepper.move(1000);
    while( stepper.run() );
}

void loop() {
    digitalWrite(PIN_IO_C, LOW);
    delay(2000);
    digitalWrite(PIN_IO_C, HIGH);
    delay(2000);
}