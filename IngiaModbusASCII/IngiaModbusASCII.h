#ifndef IngiaModbusASCII_h
#define IngiaModbusASCII_h

#include "Arduino.h"

class IngiaModbusASCII {
    public:
        IngiaModbusASCII() { }
        void begin(HardwareSerial* serial, uint32_t baudrate, uint8_t byteFormat);
    private:

};
#endif