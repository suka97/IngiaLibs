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

const uint8_t pins[] = { PIN_IO_A, PIN_IO_B, PIN_IO_C, PIN_IO_D, PIN_IO_E, PIN_IO_F };

const int mode = INPUT_PULLUP;

void setup() {
    Serial.begin(115200);
    for ( uint8_t i=0 ; i<6 ; i++ ) pinMode(pins[i], mode);
}

void loop() {
    if ( mode == INPUT_PULLUP ) {
        for ( uint8_t i=0 ; i<6 ; i++ ) {
            Serial.print(digitalRead(pins[i])); Serial.print(" ; ");
        } Serial.println("");
        delay(500);
    }
    else {
        for ( uint8_t j=0 ; j<6 ; j++ ) {
            for ( uint8_t i=0 ; i<6 ; i++ ) {
                digitalWrite(pins[i], (i==j)?HIGH:LOW);
                Serial.print(digitalRead(pins[i])); Serial.print(" ; ");
            } Serial.println("");
            delay(500);
        }
    }
}