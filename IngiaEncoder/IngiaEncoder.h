#ifndef IngiaEncoder_h
#define IngiaEncoder_h

#include <stdint.h>
#include "Arduino.h"


class IngiaEncoder
{
    private:
        uint8_t _pinA, _pinB;
        int32_t _enc_pos;
        uint8_t _enc_st;

        static IngiaEncoder * _instance;
        static void _interrupt() {
            _instance->handleEncoder();
        }

    public:
        IngiaEncoder(uint8_t pinA, uint8_t pinB) {
            _pinA = pinA; _pinB = pinB;
            pinMode(_pinA, INPUT_PULLUP);
            pinMode(_pinB, INPUT_PULLUP);
            _instance = this;
        }

        void setInterrupts() {
            attachInterrupt(digitalPinToInterrupt(_pinA), _interrupt, CHANGE);
            attachInterrupt(digitalPinToInterrupt(_pinB), _interrupt, CHANGE);
        }

        int32_t handleEncoder();
        int32_t getPos() {
            return _enc_pos;
        }
        void setPos(int32_t pos) {
            _enc_pos = pos;
        }
};

#endif