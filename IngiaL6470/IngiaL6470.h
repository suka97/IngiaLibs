#ifndef IngiaL6470_h
#define IngiaL6470_h

#include "IngiaST_StepperDriver.h"
#include "L6470_def.h"

#define HW_SPI_SPEED 1000000

class IngiaL6470 : public IngiaST_StepperDriver
{
    public:
        IngiaL6470(uint8_t csPin, uint8_t rstPin = 0)
            : IngiaST_StepperDriver(csPin, rstPin)  {  }
        

    public:
        /*
        int32_t getPosition(void) {
            return ConvertPosition(CmdGetParam(POWERSTEP01_ABS_POS));
        }
        void setHome(void) {
            CmdSetParam(POWERSTEP01_ABS_POS, 0);
        }
        bool setMaxSpeed(unsigned int speed) {
            return SetAnalogValue(POWERSTEP01_MAX_SPEED, (float)speed);
        }
        bool setMinSpeed(unsigned int speed) {
            return SetAnalogValue(POWERSTEP01_MIN_SPEED, (float)speed);
        }
        void setCurrentPosition(int32_t absPos) {
            CmdSetParam(POWERSTEP01_ABS_POS, absPos);
        }
        */
};

#endif