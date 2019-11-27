/*
  IngiaSMPTE.cpp - IngiaSMPTE library - 2019-03-06
  Copyright (c) Andres Sukane, INGIA srl.  All rights reserved.
*/

#include "IngiaSMPTE.h"

IngiaSMPTE * IngiaSMPTE::instance = NULL;

// overFlow handler
/*  lo saque porque se complicaba para armar la libreria sino
ISR(TIMER1_OVF_vect) {
  index = 0;
  TCCR1B &= ~(0x7); // paro el timer
  overFlow = true;
}
*/ 

static void IngiaSMPTE::interrupt_Handler() {
  instance->onInterrupt();
}

void IngiaSMPTE::onInterrupt() {
  static boolean unoMedioRecibido = false;
  
  timerCounter = TCNT1;
  TCCR1B |= 3; TCNT1  = 0xFFFF;  // reinicio el timer
  
  if ( timerCounter < timerTrigger ) {
    if ( !unoMedioRecibido ) {
      dataBuffer[index/32] &= ~(UNO_32BITS << (index%32)); // recibi un 0
      index++;
    }
    else {
      index--; //index = 0;  // error
      unoMedioRecibido = false;
    }
  }
  else {
    if ( unoMedioRecibido ) {
      unoMedioRecibido = false;
      dataBuffer[index/32] |= (UNO_32BITS << (index%32)); // recibi un 1
      index++;
    }
    else {
      unoMedioRecibido = true;
    }
  }

  if ( index == 80 ) {
    if ( tramaOK(dataBuffer) ) {
      index = 0;
      guardarData(dataBuffer);
      _dataReady = true;
    }
    else {
      descartarBit(dataBuffer);
      index--;
    }
  }
}

IngiaSMPTE::IngiaSMPTE(int interruptPin) {
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_Handler, CHANGE);

    // inicializo el timer1
    noInterrupts();           // disable all interrupts
    TCNT1  = 0xFFFF;   // counter en 0
    TCCR1B &= ~(0x7); TCCR1B |= 3; // arranco el timer  //  clkI/O/8 (From prescaler) (200 counter a 1ms)
    // TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt (LO SAQUE AL HACER LA LIBRERIA)
    interrupts();             // enable all interrupts

    instance = this;
}

void IngiaSMPTE::setTimerTrigger(uint16_t triggerTime) {
  timerTrigger = triggerTime;
}

void IngiaSMPTE::guardarData (uint32_t *trama) {
  uint8_t dec, uni;
  uint8_t localHour, localMinute, localSecond, localFrame;
  static boolean vinoUnoAnterior = false;

  uni = (trama[0] >> 0) & 0xF;
  dec = (trama[0] >> 8) & 0x3;
  localFrame = uni + dec * 10;

  uni = (trama[0] >> 16) & 0xF;
  dec = (trama[0] >> 24) & 0x7;
  localSecond = uni + dec * 10;

  uni = (trama[1] >> 0) & 0xF;
  dec = (trama[1] >> 8) & 0x7;
  localMinute = uni + dec * 10;

  uni = (trama[1] >> 16) & 0xF;
  dec = (trama[1] >> 24) & 0x3;
  localHour = uni + dec * 10;

  long diferencia = millisEntre(localHour, localMinute, localSecond, localFrame, hours, minutes, seconds, frames);
  if ( (diferencia < 0) || (diferencia > 1000) ) {
    if(vinoUnoAnterior) {
      vinoUnoAnterior = false;
      hours = localHour; minutes = localMinute; seconds = localSecond; frames = localFrame;
    }
    else
      vinoUnoAnterior = true;
  }
  else {
    hours = localHour; minutes = localMinute; seconds = localSecond; frames = localFrame;
    vinoUnoAnterior = false;
  }
}

long IngiaSMPTE::millisEntre(uint8_t h1, uint8_t m1, uint8_t s1, uint8_t f1, uint8_t h2, uint8_t m2, uint8_t s2, uint8_t f2) {
  long salida = 0;
  salida += (h1 - h2) * (long)60*(long)60*(long)1000; 
  salida += (m1 - m2) * (long)60*(long)1000; 
  salida += (s1 - s2) * (long)1000; 
  salida += (f1 - f2) *(long)1000 / (long)24; // frames x seg
  return salida;
}

boolean IngiaSMPTE::tramaOK (uint32_t *trama) {
  trama[2] &= 0xFFFF;  // le saco cualquier mierda que pueda estar en el buffer
  return (trama[2] == KEY);
}

void IngiaSMPTE::descartarBit(uint32_t *trama) {
  trama[0] = trama[0] >> 1; trama[0] |= trama[1] << 31;
  trama[1] = trama[1] >> 1; trama[1] |= trama[2] << 31;
  trama[2] = trama[2] >> 1; 
}

/*
boolean IngiaSMPTE::overFlowDetected() {
    if (overFlow) {
        overFlow = false;
        return true;
    }
    else {
        return false;
    }
}
*/

boolean IngiaSMPTE::dataReady() {
    if (_dataReady) {
        _dataReady = false;
        return true;
    }
    else {
        return false;
    }
}

uint8_t IngiaSMPTE::getHours() {
    return hours;
}

uint8_t IngiaSMPTE::getMinutes() {
    return minutes;
}

uint8_t IngiaSMPTE::getSeconds() {
    return seconds;
}

uint8_t IngiaSMPTE::getFrames() {
    return frames;
}

