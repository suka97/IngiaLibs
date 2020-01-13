#ifndef IngiaST_StepperDriver_h
#define IngiaST_StepperDriver_h

#include <Arduino.h>
#include <SPI.h>

#include "ST_StepperDriver_def.h"

#define HW_SPI_SPEED 1000000

class IngiaST_StepperDriver
{
    public:
        IngiaST_StepperDriver(uint8_t csPin, uint8_t rstPin = 0) {
            reset = rstPin;
            ssel = csPin;
        }

        void begin();
        uint32_t getParam(uint32_t param, uint8_t len);
        void setParam(uint8_t param, uint8_t len, uint32_t value);
        void sendCommand(uint8_t command, uint32_t value);
        uint16_t getAndClearStatus(void);

    public:
        void move(direction_t direction, uint32_t stepCount) {
            sendCommand((uint8_t)ST_DRIVER_MOVE|(uint8_t)direction, stepCount);
        }
        void stepClock(direction_t direction) {
            sendCommand((uint8_t)ST_DRIVER_STEP_CLOCK|(uint8_t)direction, 0);
        }
        void hardHiZ(void) {
            sendCommand(ST_DRIVER_HARD_HIZ, 0);
        }
        void goTo(int32_t targetPosition) {
            sendCommand(ST_DRIVER_GO_TO, targetPosition);
        }
        void goToDir(direction_t direction, int32_t abs_pos) {
            sendCommand((uint8_t)ST_DRIVER_GO_TO_DIR|(uint8_t)direction, abs_pos);  
        }
        void goUntil(motorAction_t action,direction_t direction, float speed_steps_s) {
            sendCommand( (uint8_t)ST_DRIVER_GO_UNTIL|(uint8_t)action|(uint8_t)direction, speed_steps_s_to_reg_val(speed_steps_s));
        }
        void hardStop(void) {
            sendCommand(ST_DRIVER_HARD_STOP, 0);
        }
        void run(direction_t direction, float speed_steps_s) {
            sendCommand((uint8_t)ST_DRIVER_RUN|(uint8_t)direction, speed_steps_s_to_reg_val(speed_steps_s));
        }

    protected:
        uint8_t ssel, reset;
        uint8_t spiTxBursts[CMD_ARG_MAX_NB_BYTES][MAX_NUMBER_OF_DEVICES];
        uint8_t spiRxBursts[CMD_ARG_MAX_NB_BYTES][MAX_NUMBER_OF_DEVICES];
        uint8_t numberOfDevices = 1;

        void WriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte);
        uint8_t spi_read_write(uint8_t c);

    public:
        // ------------------ CONVERSIONS ------------------
        /**********************************************************
        * @brief Convert the float formatted speed into a SPEED
        * register value
        * @param[in] steps_s the speed as steps/s, range 0 to 15625 steps/s
        * @retval The speed as steps/tick
        **********************************************************/
        uint32_t speed_steps_s_to_reg_val(float steps_s) {
            return ((uint32_t)(((float)(steps_s)*67.108864f)+0.5f));
        }

        /**********************************************************
        * @brief Convert the SPEED register value into step/s
        * @param[in] regVal The SPEED register value
        * @retval The speed as steps/s
        **********************************************************/
        float speed_reg_val_to_steps_s(uint32_t regVal) {
            return (((float)(regVal))*0.01490116119f);
        }
};

#endif