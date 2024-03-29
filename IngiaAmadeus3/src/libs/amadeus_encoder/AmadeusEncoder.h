
#ifndef AmadeusEncoder_h
#define AmadeusEncoder_h

// ----------------------------------------------------------------------------

#include <stdint.h>/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>*/
#include <Arduino.h>

// ----------------------------------------------------------------------------


class AmadeusEncoder
{

public:
  typedef enum ButtonStatus_t {
    Open = 0,
    Closed,
    
    Pressed,
    Held,
    Released,
    
    Clicked,
    DoubleClicked
    
  } Button;

private:
  const uint8_t pinA;
  const uint8_t pinB;
  const uint8_t pinBTN;
  const bool pinsActive;
  volatile int16_t delta;
  volatile int16_t last;
  uint32_t lastMoved;
  uint8_t steps;
  volatile uint16_t acceleration;
  volatile Button button;
  bool doubleClickEnabled;
  bool accelerationEnabled;

  uint16_t ENC_HOLDTIME = 2000;


public:
  AmadeusEncoder(uint8_t A, uint8_t B, uint8_t BTN = -1, uint8_t stepsPerNotch = 1, bool active = LOW);

  void service(void);  
  int16_t getValue(void);
  Button getButton(bool reset = true);
  void setDoubleClickEnabled(const bool &d) {
    doubleClickEnabled = d;
  }
  bool getDoubleClickEnabled() {
    return doubleClickEnabled;
  }
  void setAccelerationEnabled(const bool &a) {
    accelerationEnabled = a;
    if (accelerationEnabled == false) {
      acceleration = 0;
    }
  }
  bool getAccelerationEnabled() {
    return accelerationEnabled;
  }
  bool isPressed() { 
    getButton();
    return (digitalRead(pinBTN) == pinsActive);
  }
  bool isClicked() {
    if ( getButton(false) == Clicked ) {
      getButton();
      return true;
    }
    else
      return false;
  }
  bool isDoubleClicked() {
    if ( getButton(false) == DoubleClicked ) {
      getButton();
      return true;
    }
    else
      return false;
  }
  bool isHeld() {
    if ( getButton(false) == Held ) {
      getButton();
      return true;
    }
    else
      return false;
  }
  void setHoldTime(uint16_t millis) {
    ENC_HOLDTIME = millis;
  }

};

// ----------------------------------------------------------------------------

#endif 