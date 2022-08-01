#ifndef IngiaModbusMaster_Coil_h
#define IngiaModbusMaster_Coil_h

#include "./libs/ModbusMaster/src/ModbusMaster.h"


class IngiaModbusMaster_Coil 
{
// ---------------------------------- Constants -------------------------------------------------
public:
    static const uint32_t ERROR_DELAY = 10;
    static const uint8_t MAX_UPDATES = 20;
    static const uint8_t BIT_GROUP_SIZE = 16;    // si se cambia hay que revisar los uint16_t bitMask y bitGroup


// ---------------------------------- Private Members -------------------------------------------------
protected:
    uint16_t * _reg;
    uint16_t _reg_size, _reg_start;
    uint16_t _updates[MAX_UPDATES];
    uint16_t _update_cont = 0;
    ModbusMaster *_modbus;


// ---------------------------------- Public Members -------------------------------------------------
public:
    IngiaModbusMaster_Coil(ModbusMaster *modbusMaster, uint16_t *reg, uint16_t reg_size, uint16_t reg_start) {
        _modbus = modbusMaster;
        _reg = reg;
        _reg_size = reg_size;
        _reg_start = reg_start;
    }

    void write() {
        for ( uint8_t i = 0 ; i < _update_cont ; i++ ) {
            uint16_t reg_index = _updates[i];
            uint16_t value = _reg[reg_index/BIT_GROUP_SIZE] >> (reg_index%BIT_GROUP_SIZE);
            _modbus->setTransmitBuffer(0, value);
            uint8_t com_result = _modbus->writeMultipleCoils(_reg_start+reg_index, 1);
            while ( com_result != _modbus->ku8MBSuccess ) {
                delay(ERROR_DELAY);
                com_result = _modbus->writeMultipleCoils(_reg_start+reg_index, 1);
            }
        }
        _update_cont = 0;
    }

    void read() {
        uint8_t com_result = _modbus->readCoils(_reg_start, _reg_size*BIT_GROUP_SIZE);
        while ( com_result != _modbus->ku8MBSuccess ) {
            delay(ERROR_DELAY);
            com_result = _modbus->readCoils(_reg_start, _reg_size*BIT_GROUP_SIZE);
        }
        for ( uint16_t reg_index = 0 ; reg_index < _reg_size ; reg_index++ ) {
            _reg[reg_index] = _modbus->getResponseBuffer(reg_index);
            // // only write the read if no changes on master
            // bool master_change = false;
            // for ( uint8_t update_index = 0 ; update_index < _update_cont ; update_index++ ) {
            //     if ( _updates[update_index] == reg_index ) { 
            //         master_change = true;
            //         break;
            //     }
            // }
            // if ( master_change ) continue;
            // uint16_t bitGroup = _modbus->getResponseBuffer(reg_index/BIT_GROUP_SIZE);
            // uint16_t bitMask = ((uint16_t)1 << (reg_index%BIT_GROUP_SIZE));
            // _reg[reg_index/BIT_GROUP_SIZE] &= ~bitMask;
            // _reg[reg_index/BIT_GROUP_SIZE] |= (bitGroup & bitMask);
        }
    }

    bool get(uint16_t addr) {
        uint16_t bitMask = ((uint16_t)1 << (addr%BIT_GROUP_SIZE));
        return _reg[addr/BIT_GROUP_SIZE] & bitMask;
    }

    void set(uint16_t addr, bool val) {
        if ( _update_cont >= (MAX_UPDATES-1) ) return;
        
        uint16_t bitMask = ((uint16_t)1 << (addr%BIT_GROUP_SIZE));
        if ( val ) _reg[addr/BIT_GROUP_SIZE] |= bitMask;
        else _reg[addr/BIT_GROUP_SIZE] &= ~bitMask;
        // agrego los update indexes solo si ya no estan en el array
        bool addr_updated = false;
        for ( uint8_t update_index = 0 ; update_index < _update_cont ; update_index++ ) {
            if ( _updates[update_index] == (addr) ) {
                addr_updated = true; 
                break;
            }
        }
        if ( !addr_updated ) {
            _updates[_update_cont] = addr;
            _update_cont++;
        }
    }
};

#endif