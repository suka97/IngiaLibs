#include "IngiaAmadeus2_Modbus.h"

bool IngiaAmadeus2_Modbus::begin(uint8_t hmiID, Stream &serial) {
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timer1Isr); 
    
    modbus.begin(hmiID, serial);
    delay(1500);
    return false;
}

bool IngiaAmadeus2_Modbus::sendScreenBuffer() {
    modbus.setTransmitBuffer(0, true);
    for ( uint8_t i=0 ; i<3 ; i++ ) {
        // como se que LETTERS_X_ROW es par no hago quilombo
        for ( uint8_t j=0 ; j<REGS_X_RENGLON ; j++ ) {
            uint16_t var = (((uint16_t)buffer[i][j*2])<<8)|((uint16_t)buffer[i][(j*2)+1]);
            modbus.setTransmitBuffer( (i*REGS_X_RENGLON)+j+1, var );
            //Serial.print(var, HEX); Serial.print(",");
        } //Serial.println("");
    } //Serial.println("#####");
    return ( modbus.writeMultipleRegisters(LCD_PRINT_REG, 1+(3*REGS_X_RENGLON)) == modbus.ku8MBSuccess );
}

void IngiaAmadeus2_Modbus::getKeyboardState(KeyState arr_4x4[][4]) {
    if ( modbus.readHoldingRegisters(BUT_STATE1_REG, 2) == modbus.ku8MBSuccess ) {
        uint32_t reg = ((uint32_t)modbus.getResponseBuffer(0)) | (((uint32_t)modbus.getResponseBuffer(1))<<16);
        arr_4x4[0][0] = (KeyState)((reg & (uint32_t)0x00000003) >> 0);
        arr_4x4[0][1] = (KeyState)((reg & (uint32_t)0x0000000C) >> 2);
        arr_4x4[0][2] = (KeyState)((reg & (uint32_t)0x00000030) >> 4);
        arr_4x4[0][3] = (KeyState)((reg & (uint32_t)0x000000C0) >> 6);
        arr_4x4[1][0] = (KeyState)((reg & (uint32_t)0x00000300) >> 8);
        arr_4x4[1][1] = (KeyState)((reg & (uint32_t)0x00000C00) >> 10);
        arr_4x4[1][2] = (KeyState)((reg & (uint32_t)0x00003000) >> 12);
        arr_4x4[1][3] = (KeyState)((reg & (uint32_t)0x0000C000) >> 14);
        arr_4x4[2][0] = (KeyState)((reg & (uint32_t)0x00030000) >> 16);
        arr_4x4[2][1] = (KeyState)((reg & (uint32_t)0x000C0000) >> 18);
        arr_4x4[2][2] = (KeyState)((reg & (uint32_t)0x00300000) >> 20);
        arr_4x4[2][3] = (KeyState)((reg & (uint32_t)0x00C00000) >> 22);
        arr_4x4[3][0] = (KeyState)((reg & (uint32_t)0x03000000) >> 24);
        arr_4x4[3][1] = (KeyState)((reg & (uint32_t)0x0C000000) >> 26);
        arr_4x4[3][2] = (KeyState)((reg & (uint32_t)0x30000000) >> 28);
        arr_4x4[3][3] = (KeyState)((reg & (uint32_t)0xC0000000) >> 30);
    }
}

KeyboardState IngiaAmadeus2_Modbus::getKeyboardState() {
    KeyboardState salida;
    KeyState arr[4][4]; getKeyboardState(arr);
    salida.key_1 = arr[0][0];
    salida.key_2 = arr[0][1];
    salida.key_3 = arr[0][2];
    salida.key_A = arr[0][3];
    salida.key_4 = arr[1][0];
    salida.key_5 = arr[1][1];
    salida.key_6 = arr[1][2];
    salida.key_B = arr[1][3];
    salida.key_7 = arr[2][0];
    salida.key_8 = arr[2][1];
    salida.key_9 = arr[2][2];
    salida.key_C = arr[2][3];
    salida.key_X = arr[3][0];
    salida.key_0 = arr[3][1];
    salida.key_H = arr[3][2];
    salida.key_D = arr[3][3];
    return salida;
}

KeysClickedReleased IngiaAmadeus2_Modbus::getKeysClickedReleased(bool reset) {
    KeysClickedReleased salida;
    if ( modbus.readHoldingRegisters(BUT_CLICK_REG, 2) == modbus.ku8MBSuccess ) {
        salida.clicked = modbus.getResponseBuffer(0);
        salida.released = modbus.getResponseBuffer(1);
        // y los pongo en cero porque los lei
        if ( reset ) {
            modbus.setTransmitBuffer(BUT_CLICK_REG, 0);
            modbus.setTransmitBuffer(BUT_CLICK_REG+1, 0);
            modbus.writeMultipleRegisters(BUT_CLICK_REG, 2);
        }
    }
    if(lastKeysClickedReleased.clicked||lastKeysClickedReleased.released) 
        Serial.println(lastKeysClickedReleased.clicked, HEX);
    //if(lastKeysClickedReleased.released) Serial.println(lastKeysClickedReleased.released, HEX);
    
    return salida;
}

bool IngiaAmadeus2_Modbus::cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin) {
    if ( lastKeysClickedReleased.clicked ) {
        String strVar = String(*var);
        
        if ( TECLA_MENOS_MASK & lastKeysClickedReleased.clicked ) {
            if ( (*var) < 0 ) 
                strVar.remove(0, 1);
            else 
                strVar = String('-') + strVar;
        }
        else if ( TECLA_BORRAR_MASK & lastKeysClickedReleased.clicked ) {
            if ( abs(*var) >= 10 )
                strVar.remove(strVar.length()-1);
            else
                strVar = String('0');
        }
        else if ( TECLA_1_MASK & lastKeysClickedReleased.clicked ) strVar.concat('1');
        else if ( TECLA_2_MASK & lastKeysClickedReleased.clicked ) strVar.concat('2');
        else if ( TECLA_3_MASK & lastKeysClickedReleased.clicked ) strVar.concat('3');
        else if ( TECLA_4_MASK & lastKeysClickedReleased.clicked ) strVar.concat('4');
        else if ( TECLA_5_MASK & lastKeysClickedReleased.clicked ) strVar.concat('5');
        else if ( TECLA_6_MASK & lastKeysClickedReleased.clicked ) strVar.concat('6');
        else if ( TECLA_7_MASK & lastKeysClickedReleased.clicked ) strVar.concat('7');
        else if ( TECLA_8_MASK & lastKeysClickedReleased.clicked ) strVar.concat('8');
        else if ( TECLA_9_MASK & lastKeysClickedReleased.clicked ) strVar.concat('9');
        else if ( TECLA_0_MASK & lastKeysClickedReleased.clicked ) strVar.concat('0');

        long valNow = strVar.toInt();
        if ( valNow > valmax )
            (*var) = valmax;
        else if ( valNow < valmin )
            (*var) = valmin;
        else
            (*var) = valNow;

        return true;
    }
    return false;
}

bool IngiaAmadeus2_Modbus::cambiarVarIncremental(long *var, long valmin, long valmax, int *flagMaxMin) {
    if ( lastKeysClickedReleased.clicked ) {
        if ( (TECLA_ARRIBA_MASK & lastKeysClickedReleased.clicked) && ((*var) < valmax) ) 
            (*var)++;
        else if ( (TECLA_ABAJO_MASK & lastKeysClickedReleased.clicked) && ((*var) > valmin) )
            (*var)--;
        
        return true;
    }
    return false;
}