#include "IngiaST_StepperDriver.h"

void IngiaST_StepperDriver::begin() {
    // pin definitions
    pinMode(ssel, OUTPUT); digitalWrite(ssel, HIGH);
    if(!reset) {
        pinMode(reset, OUTPUT); digitalWrite(reset, HIGH);
    }

    // spin initialization
    SPI.begin();
    SPI.beginTransaction(SPISettings(HW_SPI_SPEED, MSBFIRST, SPI_MODE3)); 
    SPI.endTransaction();
}


void IngiaST_StepperDriver::sendCommand(uint8_t command, uint32_t value) {
    uint32_t loop;
    uint8_t maxArgumentNbBytes = 0;
    uint8_t spiIndex = 0;

    for (loop = 0; loop < numberOfDevices; loop++) {
        spiTxBursts[0][loop] = ST_DRIVER_NOP;
        spiTxBursts[1][loop] = ST_DRIVER_NOP;
        spiTxBursts[2][loop] = ST_DRIVER_NOP;
        spiTxBursts[3][loop] = ST_DRIVER_NOP;
    }

    switch (command & DAISY_CHAIN_COMMAND_MASK) {
        case ST_DRIVER_GO_TO:
        case ST_DRIVER_GO_TO_DIR:
            value = value & ST_DRIVER_ABS_POS_VALUE_MASK;
            [[fallthrough]];
        case ST_DRIVER_RUN:
        case ST_DRIVER_MOVE:
        case ST_DRIVER_GO_UNTIL:
        case ST_DRIVER_GO_UNTIL_ACT_CPY:
            spiTxBursts[0][spiIndex] = command;
            spiTxBursts[1][spiIndex] = (uint8_t)(value >> 16);
            spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
            spiTxBursts[3][spiIndex] = (uint8_t)(value);
            maxArgumentNbBytes = 3;
            break;
        default:
            spiTxBursts[0][spiIndex] = ST_DRIVER_NOP;
            spiTxBursts[1][spiIndex] = ST_DRIVER_NOP;
            spiTxBursts[2][spiIndex] = ST_DRIVER_NOP;
            spiTxBursts[3][spiIndex] = command;
    }

    for (loop = CMD_ARG_MAX_NB_BYTES - 1 - maxArgumentNbBytes;
        loop < CMD_ARG_MAX_NB_BYTES;
        loop++)
    {
        WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
    }
}


uint16_t IngiaST_StepperDriver::getAndClearStatus(void) {
    uint16_t status = 0;
    uint32_t loop;
    uint8_t spiIndex = 0;
    
    for (loop = 0; loop < numberOfDevices; loop++) {
        spiTxBursts[0][loop] = ST_DRIVER_NOP;
        spiTxBursts[1][loop] = ST_DRIVER_NOP;
        spiTxBursts[2][loop] = ST_DRIVER_NOP;
        spiTxBursts[3][loop] = ST_DRIVER_NOP;
        spiRxBursts[0][loop] = 0;
        spiRxBursts[1][loop] = 0;
        spiRxBursts[2][loop] = 0;
        spiRxBursts[3][loop] = 0;       
    }
    spiTxBursts[0][spiIndex] = ST_DRIVER_GET_STATUS;

    for (loop = 0; loop < CMD_ARG_NB_BYTES_GET_STATUS + RSP_NB_BYTES_GET_STATUS; loop++)
    {
        WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
    }
    status = (spiRxBursts[1][spiIndex] << 8) | (spiRxBursts[2][spiIndex]);;  

    return (status);
}


uint32_t IngiaST_StepperDriver::getParam(uint32_t param, uint8_t len) {
    uint32_t spiRxData;
    uint8_t maxArgumentNbBytes = 0;
    uint8_t spiIndex = 0, loop;
    spiTxBursts[0][0] = ST_DRIVER_NOP;
    spiTxBursts[1][0] = ST_DRIVER_NOP;
    spiTxBursts[2][0] = ST_DRIVER_NOP;
    spiTxBursts[3][0] = ST_DRIVER_NOP;
    spiRxBursts[0][0] = 0;
    spiRxBursts[1][0] = 0;
    spiRxBursts[2][0] = 0;
    spiRxBursts[3][0] = 0;

    if ( len > 19 ) {
        spiTxBursts[0][spiIndex] = ((uint8_t)ST_DRIVER_GET_PARAM )| (param);
        maxArgumentNbBytes = 3;
    }
    else if ( len > 8 ) {
        spiTxBursts[1][spiIndex] = ((uint8_t)ST_DRIVER_GET_PARAM )| (param);
        maxArgumentNbBytes = 2;
    }
    else {
        spiTxBursts[2][spiIndex] = ((uint8_t)ST_DRIVER_GET_PARAM )| (param);
        maxArgumentNbBytes = 1;
    }

    for (loop = CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes;
        loop < CMD_ARG_MAX_NB_BYTES; loop++)
    {
        WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
    }
    spiRxData = ((uint32_t)spiRxBursts[1][spiIndex] << 16)|
                (spiRxBursts[2][spiIndex] << 8) | (spiRxBursts[3][spiIndex]);    
    return (spiRxData);
}


void IngiaST_StepperDriver::setParam(uint8_t param, uint8_t len, uint32_t value) {
    uint8_t spiIndex = 0;
    uint8_t maxArgumentNbBytes = 0, loop;
    spiTxBursts[0][0] = ST_DRIVER_NOP;
    spiTxBursts[1][0] = ST_DRIVER_NOP;
    spiTxBursts[2][0] = ST_DRIVER_NOP;
    spiTxBursts[3][0] = ST_DRIVER_NOP;

    if ( len == 22 ) {
        spiTxBursts[0][spiIndex] = ((uint8_t)ST_DRIVER_SET_PARAM )| (param); 
        spiTxBursts[1][spiIndex] = (uint8_t)(value >> 16);
        spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
        maxArgumentNbBytes = 3;
    }
    else if ( len > 8 ) {
        spiTxBursts[1][spiIndex] = ((uint8_t)ST_DRIVER_SET_PARAM )| (param);
        spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
        maxArgumentNbBytes = 2;
    }
    else {
        spiTxBursts[2][spiIndex] = ((uint8_t)ST_DRIVER_SET_PARAM )| (param);
        maxArgumentNbBytes = 1;
    }
    spiTxBursts[3][spiIndex] = (uint8_t)(value);

    for (loop = CMD_ARG_MAX_NB_BYTES - 1 - maxArgumentNbBytes; 
        loop < CMD_ARG_MAX_NB_BYTES; loop++)
    {
        WriteBytes(&spiTxBursts[loop][0],&spiRxBursts[loop][0]); 
    }
}


void IngiaST_StepperDriver::WriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte) {
    digitalWrite(ssel, LOW);
    pReceivedByte[0] = spi_read_write(pByteToTransmit[0]);
    digitalWrite(ssel, HIGH);
}

uint8_t IngiaST_StepperDriver::spi_read_write(uint8_t c) {
    SPI.beginTransaction(SPISettings(HW_SPI_SPEED, MSBFIRST, SPI_MODE3)); 
    uint8_t salida; salida = SPI.transfer(c);
    SPI.endTransaction(); 
    return salida;
}