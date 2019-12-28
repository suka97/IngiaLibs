#include "IngiaModbusASCII.h"

void IngiaModbusASCII::begin(HardwareSerial* serial, uint32_t baudrate, uint8_t byteFormat) {
    serial->begin(baudrate);
    serial->println("hola");
}
