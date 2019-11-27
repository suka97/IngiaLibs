/*
  IngiaSMPTE.h - IngiaSMPTE library - 2019-03-06
  Copyright (c) Andres Sukane, INGIA srl.  All rights reserved.
*/

#ifndef __IngiaSMPTE_H
#define __IngiaSMPTE_H

#include <Arduino.h>

#define KEY 0xBFFC
#define TIMER_TRIGGER_DEFAULT 175

class IngiaSMPTE
{
    public:
        IngiaSMPTE(int interruptPin);
        // boolean overFlowDetected();
        boolean dataReady();
        uint8_t getHours();
        uint8_t getMinutes();
        uint8_t getSeconds();
        uint8_t getFrames();
        void setTimerTrigger(uint16_t triggerTime);

    private:
        static IngiaSMPTE *instance;
        void onInterrupt();

        uint16_t timerTrigger = TIMER_TRIGGER_DEFAULT;
        uint8_t index = 0;
        uint16_t timerCounter = 0;
        boolean _dataReady = false;
        // boolean overFlow = false;
        uint8_t frames, seconds, minutes, hours;
        uint32_t dataBuffer[3];
        const uint32_t UNO_32BITS = 1;
        long millisEntre(uint8_t h1, uint8_t m1, uint8_t s1, uint8_t f1, uint8_t h2, uint8_t m2, uint8_t s2, uint8_t f2);

        void guardarData (uint32_t *trama);
        boolean tramaOK (uint32_t *trama);
        void descartarBit(uint32_t *trama);
        static void interrupt_Handler();

};

#endif /* #ifndef __L6474_H_INCLUDED */