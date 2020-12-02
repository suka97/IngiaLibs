#ifdef __AVR__
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
  #define PIN_ENCODER_A               A1
  #define PIN_ENCODER_B               A0
  #define PIN_ENCODER_BUTTON          1
  #define PIN_LCD_CS                  10
  #define PIN_LCD_DC                  9
  #define PIN_LCD_RES                 8
#else
  #define PIN_STEPPER_ENABLE          13
  #define PIN_STEPPER1_PULSO          33
  #define PIN_STEPPER1_DIRECCION      35 //X IO19
  #define PIN_STEPPER2_PULSO          34  // NO VAN LOS ADC
  #define PIN_STEPPER2_DIRECCION      14
  #define PIN_IO_A                    26 
  #define PIN_IO_B                    16
  #define PIN_IO_C                    21
  #define PIN_IO_D                    4 //X IO35 
  #define PIN_IO_E                    17 
  #define PIN_IO_F                    22 
  #define PIN_ENCODER_A               32 //X SD3
  #define PIN_ENCODER_B               15 //X CMD
  #define PIN_ENCODER_BUTTON          2 //X SD2
  #define PIN_LCD_CS                  5
  #define PIN_LCD_DC                  27
  #define PIN_LCD_RES                 25
  // LCD_CLK(D0)                      IO18  
  // LCD_MOSI(D1)                     IO23
#endif