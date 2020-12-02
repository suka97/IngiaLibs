#ifdef __AVR__
    #include <TimerOne.h>

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
#else
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
#endif