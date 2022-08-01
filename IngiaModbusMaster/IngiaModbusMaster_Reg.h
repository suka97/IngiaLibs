#ifndef IngiaModbusMaster_Reg_h
#define IngiaModbusMaster_Reg_h

#include "./libs/ModbusMaster/src/ModbusMaster.h"

class IngiaModbusMaster_Reg 
{
// ---------------------------------- Constants -------------------------------------------------
public:
    static const uint32_t ERROR_DELAY = 10;
    static const uint8_t MAX_UPDATES = 20;


// ---------------------------------- Private Members -------------------------------------------------
protected:
    uint16_t * _reg;
    uint16_t _reg_size, _reg_start;
    uint16_t _updates[MAX_UPDATES];
    uint16_t _update_cont = 0;
    ModbusMaster *_modbus;


// ---------------------------------- Public Members -------------------------------------------------
public:
    IngiaModbusMaster_Reg(ModbusMaster *modbusMaster, uint16_t *reg, uint16_t reg_size, uint16_t reg_start) {
        _modbus = modbusMaster;
        _reg = reg;
        _reg_size = reg_size;
        _reg_start = reg_start;
    }

    void write() {
        for ( uint8_t i = 0 ; i < _update_cont ; i++ ) {
            uint16_t reg_index = _updates[i];
            _modbus->setTransmitBuffer(0, _reg[reg_index]);
            uint8_t com_result = _modbus->writeMultipleRegisters(_reg_start+reg_index, 1);
            while ( com_result != _modbus->ku8MBSuccess ) {
                delay(ERROR_DELAY);
                com_result = _modbus->writeMultipleRegisters(_reg_start+reg_index, 1);
            }
        }
        _update_cont = 0;
    }

    void read() {
        uint8_t com_result = _modbus->readHoldingRegisters(_reg_start, _reg_size);
        while ( com_result != _modbus->ku8MBSuccess ) {
            delay(ERROR_DELAY);
            com_result = _modbus->readHoldingRegisters(_reg_start, _reg_size);
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
            // _reg[reg_index] = _modbus->getResponseBuffer(reg_index);
        }
    }

    template < class genericType > 
    genericType get(uint16_t addr, genericType *buffer) {
        *buffer = *(genericType *)(_reg + addr);
        return *buffer;
    }
    uint16_t getWord(uint16_t addr) {
        return _reg[addr];
    }

    template < class genericType > 
    void set(uint16_t addr, genericType val) {
        if ( _update_cont >= (MAX_UPDATES-1) ) return;   
             
        *(genericType *)(_reg + addr) = val;
        // agrego los update indexes solo si ya no estan en el array
        uint8_t var_size = sizeof(genericType) / 2;
        for ( uint8_t i = 0 ; i < var_size ; i++ ) {
            bool addr_updated = false;
            for ( uint8_t update_index = 0 ; update_index < _update_cont ; update_index++ ) {
                if ( _updates[update_index] == (addr + i) ) {
                    addr_updated = true; 
                    break;
                }
            }
            if ( !addr_updated ) {
                _updates[_update_cont] = addr + i;
                _update_cont++;
            }
        }
    }
};

#endif