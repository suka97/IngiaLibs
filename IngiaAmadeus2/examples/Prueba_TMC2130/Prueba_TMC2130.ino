#include <TMC2130.h>

#define PIN_CS    10
#define PIN_DIR   A0
#define PIN_PUL   A1
#define PIN_EN    A2

TMC2130 stepper_driver;

void setup() {
  pinMode(PIN_EN,   OUTPUT);
  pinMode(PIN_DIR,  OUTPUT);
  pinMode(PIN_PUL,  OUTPUT);
  stepper_driver.setup(PIN_CS);
  Serial.begin(115200);

  digitalWrite(PIN_EN, LOW);
}

void loop() {
  if (stepper_driver.communicating())
    Serial.println("SPI communicating with stepper driver!\n");
  else
   Serial.println("SPI not communicating with stepper driver!\n");
   
  stepper_driver.initialize();

  stepper_driver.setRunCurrent(20);
  stepper_driver.setHoldCurrent(20);
  stepper_driver.setHoldDelay(20);

  while(1) {
    digitalWrite(PIN_PUL, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_PUL, LOW);
    delayMicroseconds(10);
  }
}
