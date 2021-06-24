#include "IngiaEncoder.h"

IngiaEncoder * IngiaEncoder::_instance = NULL;

int32_t IngiaEncoder::handleEncoder() {
   uint8_t enc_new_st = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
    if ( enc_new_st != _enc_st ) {
        uint8_t last_clk = (!digitalRead(_pinB) << 1) | digitalRead(_pinA);    // estado anterior en caso de girar horario
        int8_t inc = (_enc_st == last_clk) ? 1 : (-1);
        _enc_pos += inc; 
        _enc_st = enc_new_st;
    }
    return _enc_pos;
}