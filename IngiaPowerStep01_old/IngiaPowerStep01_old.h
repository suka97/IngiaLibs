#ifndef IngiaPowerStep01_old_h
#define IngiaPowerStep01_old_h

#include <Arduino.h>
#include <SPI.h>
#include "motor_def.h"
#include "PowerStep01_def.h"
#include "component_def.h"
#include "StepperMotor.h"
#include "powerstep01_target_config.h"
#include <SoftSPI.h>

#define HW_SPI_SPEED 1000000
#define HW_SPI

class IngiaPowerStep01_old
{

public:
    #ifndef HW_SPI
        SoftSPI mySPI;
    #endif

    IngiaPowerStep01_old(uint8_t csPin, uint8_t resetPin = 0, uint8_t stckPin = 0) 
        #ifndef HW_SPI 
            : mySPI(11, 12, 13) 
        #endif
    {
        reset = resetPin;
        ssel = csPin; 
        stck = stckPin;
    }
    void begin(void *deviceConfig = (void *)0) {
        pinMode(ssel, OUTPUT); digitalWrite(ssel, HIGH);
        if(!reset) {
            pinMode(reset, OUTPUT); digitalWrite(reset, HIGH);
        }
        if(!stck) {
            pinMode(stck, OUTPUT); digitalWrite(stck, HIGH);
        }

        #ifdef HW_SPI            
            SPI.begin();
            SPI.beginTransaction(SPISettings(HW_SPI_SPEED, MSBFIRST, SPI_MODE3)); 
            SPI.endTransaction();
        #else
            mySPI.setMosiIdle(HIGH);
            mySPI.setDataMode(SPI_MODE2);
            mySPI.setBitOrder(MSBFIRST);
            mySPI.begin();
        #endif

        //Init(deviceConfig);
    }
    boolean initDevice(void *deviceConfig = (void *)0) {
        Init(deviceConfig);

        uint16_t status = getStatus();
        boolean error = false;

        // si no esta en alta impedancia
        if ( !(status & POWERSTEP01_STATUS_HIZ) )
            error = true;
        // si hay CMD ERROR
        if ( status & POWERSTEP01_STATUS_CMD_ERROR )
            error = true;
        // si hay overcurrent
        if ( !(status & POWERSTEP01_STATUS_OCD) )
            error = true;
        return error;
    }
    void move(StepperMotor::direction_t direction, uint32_t stepCount) {
        CmdMove((motorDir_t) direction, stepCount);
    }
    void stepClockMode_Enable(StepperMotor::direction_t direction) {
        CmdStepClock((motorDir_t)direction);
    }
    void setHardHiZ() {
        CmdHardHiZ();
    }
    void goTo(int32_t targetPosition) {
        CmdGoTo(targetPosition);
    }
    void gotToDir(StepperMotor::direction_t direction, int32_t abs_pos) {
        CmdGoToDir((motorDir_t)direction, abs_pos);
    }
    void gotUntil(motorAction_t action, StepperMotor::direction_t direction, uint32_t speed) {
        CmdGoUntil(action, (motorDir_t)direction, speed);
    }
    void hardStop() {
        CmdHardStop();
    }
    void run(StepperMotor::direction_t direction, uint32_t speed_stseg) {
        CmdRun((motorDir_t)direction, speed_steps_s_to_reg_val(speed_stseg));
    }
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
    bool isDeviceBusy(void) {
        return (!(CmdGetStatus() & POWERSTEP01_STATUS_BUSY));
    }
    void setCurrentPosition(int32_t absPos) {
        CmdSetParam(POWERSTEP01_ABS_POS, absPos);
    }
    uint16_t getStatus() {
        return CmdGetParam(POWERSTEP01_STATUS);
    }
    uint16_t getAndClearStatus() {
        return CmdGetStatus();
    }
    uint32_t getParam(uint32_t param, uint8_t len) {
        uint32_t spiRxData;
        uint8_t maxArgumentNbBytes = 0;
        uint8_t spiIndex = 0, loop;
        spiTxBursts[0][loop] = POWERSTEP01_NOP;
        spiTxBursts[1][loop] = POWERSTEP01_NOP;
        spiTxBursts[2][loop] = POWERSTEP01_NOP;
        spiTxBursts[3][loop] = POWERSTEP01_NOP;
        spiRxBursts[0][loop] = 0;
        spiRxBursts[1][loop] = 0;
        spiRxBursts[2][loop] = 0;
        spiRxBursts[3][loop] = 0;

        if ( len > 19 ) {
            spiTxBursts[0][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
            maxArgumentNbBytes = 3;
        }
        else if ( len > 8 ) {
            spiTxBursts[1][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
            maxArgumentNbBytes = 2;
        }
        else {
            spiTxBursts[2][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
            maxArgumentNbBytes = 1;
        }

        for (loop = POWERSTEP01_CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes;
            loop < POWERSTEP01_CMD_ARG_MAX_NB_BYTES; loop++)
        {
            WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
        }
        spiRxData = ((uint32_t)spiRxBursts[1][spiIndex] << 16)|
                    (spiRxBursts[2][spiIndex] << 8) | (spiRxBursts[3][spiIndex]);    
        return (spiRxData);
    }
    void setParam(uint8_t param, uint8_t len, uint32_t value) {
        uint8_t spiIndex = 0;
        uint8_t maxArgumentNbBytes = 0, loop;
        spiTxBursts[0][0] = POWERSTEP01_NOP;
        spiTxBursts[1][0] = POWERSTEP01_NOP;
        spiTxBursts[2][0] = POWERSTEP01_NOP;
        spiTxBursts[3][0] = POWERSTEP01_NOP;

        if ( len == 22 ) {
            spiTxBursts[0][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param); 
            spiTxBursts[1][spiIndex] = (uint8_t)(value >> 16);
            spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
            maxArgumentNbBytes = 3;
        }
        else if ( len > 8 ) {
            spiTxBursts[1][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param);
            spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
            maxArgumentNbBytes = 2;
        }
        else {
            spiTxBursts[2][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param);
            maxArgumentNbBytes = 1;
        }
        spiTxBursts[3][spiIndex] = (uint8_t)(value);

        for (loop = POWERSTEP01_CMD_ARG_MAX_NB_BYTES - 1 - maxArgumentNbBytes; 
            loop < POWERSTEP01_CMD_ARG_MAX_NB_BYTES; loop++)
        {
            WriteBytes(&spiTxBursts[loop][0],&spiRxBursts[loop][0]); 
        }
    }



private:
    // ---------------------- variables -------------------------------
    uint8_t numberOfDevices = 1;
    uint8_t spiTxBursts[POWERSTEP01_CMD_ARG_MAX_NB_BYTES][MAX_NUMBER_OF_DEVICES];
    uint8_t spiRxBursts[POWERSTEP01_CMD_ARG_MAX_NB_BYTES][MAX_NUMBER_OF_DEVICES];
    bool spiPreemptionByIsr;
    bool isrFlag;
    uint8_t deviceInstance = 0;
    uint8_t ssel, reset, stck = 0;

    // -------------------  otros metodos -------------------------------------
    void SetDeviceParamsToGivenValues(powerstep01_init_u_t *);
    void SetRegisterToPredefinedValues(void);
    void Board_Delay(uint32_t mdelay);
    status_t Init(void *init);

    // -------------------  base communication -------------------------------------
    status_t ReadWrite(uint8_t*, uint8_t*, uint16_t);
    uint8_t Board_SpiWriteBytes(uint8_t *, uint8_t *);
    void WriteBytes(uint8_t *, uint8_t *);
    int spi_read_write(uint8_t*, uint8_t*, uint8_t, uint16_t);
    void SendCommand(uint8_t, uint32_t);
    uint16_t CmdGetStatus(void);
    bool SetAnalogValue(powerstep01_Registers_t param, float value);
    int32_t ConvertPosition(uint32_t abs_position_reg);
    uint32_t CmdGetParam(powerstep01_Registers_t param);
    void CmdSetParam(uint8_t, uint32_t);
    void FetchAndClearAllStatus(void);
    uint8_t mi_write (uint8_t c) {
        #ifdef HW_SPI
            SPI.beginTransaction(SPISettings(HW_SPI_SPEED, MSBFIRST, SPI_MODE3)); 
            uint8_t salida; salida = SPI.transfer(c);
            SPI.endTransaction(); 
            return salida;
        #else
            return mySPI.transfer(c);
        #endif
    }

    // -------------------  send commands -------------------------------------
    void CmdMove(motorDir_t direction, uint32_t stepCount) {
        SendCommand((uint8_t)POWERSTEP01_MOVE|(uint8_t)direction, stepCount);
    }
    void CmdStepClock(motorDir_t direction) {
        SendCommand((uint8_t)POWERSTEP01_STEP_CLOCK|(uint8_t)direction, 0);
    }
    void CmdHardHiZ(void) {
        SendCommand(POWERSTEP01_HARD_HIZ, 0);
    }
    void CmdGoTo(int32_t targetPosition) {
        SendCommand(POWERSTEP01_GO_TO, targetPosition);
    }
    void CmdGoToDir(motorDir_t direction, int32_t abs_pos) {
        SendCommand((uint8_t)POWERSTEP01_GO_TO_DIR|(uint8_t)direction, abs_pos);  
    }
    void CmdGoUntil(motorAction_t action,motorDir_t direction, uint32_t speed) {
        SendCommand( (uint8_t)POWERSTEP01_GO_UNTIL|(uint8_t)action|(uint8_t)direction, speed);
    }
    void CmdHardStop(void) {
        SendCommand(POWERSTEP01_HARD_STOP, 0);
    }
    void CmdRun(motorDir_t direction, uint32_t speed) {
        SendCommand((uint8_t)POWERSTEP01_RUN|(uint8_t)direction, speed);
    }


    // -------------------- conversiones ----------------------------------
    
    uint16_t acc_dec_steps_s2_to_reg_val(float steps_s2) {
        return ((uint16_t)(((float)(steps_s2)*0.068719476736f)+0.5f));
    }

    /**********************************************************
    * @brief Convert the ACC or DEC register value into step/s^2
    * @param[in] regVal The ACC or DEC register value
    * @retval The speed as steps/s
    **********************************************************/
    float acc_dec_reg_val_to_steps_s2(uint32_t regVal) {
        return (((float)(regVal))*14.5519152283f);
    }

    /**********************************************************
    * @brief Converts BEMF compensation slope to values for ST_SLP,
    * FN_SLP_ACC or FN_SLP_DEC register
    * @param[in] percentage BEMF compensation slope percentage,
    * range 0 to 0.4% (0.004) s/step
    * @retval value for ST_SLP, FN_SLP_ACC or FN_SLP_DEC register
    **********************************************************/
    uint8_t bemf_slope_perc_to_reg_val(float percentage) {
        return ((uint8_t)(((float)(percentage)*637.5f)+0.5f));
    }

    /**********************************************************
    * @brief Converts values from ST_SLP, FN_SLP_ACC or
    * FN_SLP_DEC register to BEMF compensation slope percentage
    * @param[in] regVal The ST_SLP, FN_SLP_ACC or FN_SLP_DEC
    * register value
    * @retval BEMF compensation slope percentage
    **********************************************************/
    float bemf_slope_reg_val_to_perc(uint32_t regVal) {
        return (((float)(regVal))*0.00156862745098f);
    }

    /**********************************************************
    * @brief Convert the float formatted speed into a FS_SPD
    * register value
    * @param[in] steps_s the speed as steps/s, range 15.25 to 15610 steps/s
    * @retval The speed as steps/tick
    **********************************************************/
    uint16_t fs_spd_steps_s_to_reg_val(float steps_s) {
        return ((uint16_t)((float)(steps_s)*0.065536f));
    }

    /**********************************************************
    * @brief Convert the FS_SPD register value into step/s
    * @param[in] regVal The FS_SPD register value
    * @retval The full Step speed as steps/s
    **********************************************************/
    float fs_spd_reg_val_to_steps_s(uint32_t regVal) {
        return (((float)regVal+0.999f)*15.258789f);
    }

    /**********************************************************
    * @brief Convert the float formatted speed into a INT_SPEED
    * register value
    * @param[in] steps_s the speed as steps/s, range 0 to 976.5 steps/s
    * @retval The intersect speed as steps/tick
    **********************************************************/
    uint16_t int_spd_steps_s_to_reg_val(float steps_s) {
        return ((uint16_t)(((float)(steps_s)*16.777216f)+0.5f));
    }

    /**********************************************************
    * @brief Convert the INT_SPEED register value into step/s
    * @param[in] regVal The INT_SPEED register value
    * @retval The speed as steps/s
    **********************************************************/
    float int_spd_reg_val_to_steps_s(uint32_t regVal) {
        return (((float)(regVal))*0.0596045f);
    }

    /**********************************************************
    * @brief Convert the float formatted thermal compensation
    * factor into a K_THERM register value
    * @param[in] compFactor the float formatted thermal
    * compensation factor, range 1 to 1.46875
    * @retval value for K_THERM register
    **********************************************************/
    uint8_t k_therm_comp_to_reg_val(float compFactor) {
        return ((uint8_t)((((float)(compFactor)-1.0f)*32.0f)+0.5f));
    }

    /**********************************************************
    * @brief Convert the K_THERM register value into a float
    * formatted thermal compensation factor
    * @param[in] regVal The K_THERM register value
    * @retval The float formatted thermal compensation factor
    **********************************************************/
    float k_therm_reg_val_to_comp(uint32_t regVal) {
        return (((float)(regVal))*0.03125f+1);
    }

    /**********************************************************
    * @brief Converts voltage in percentage to values for KVAL_RUN,
    * KVAL_HOLD, KVAL_ACC or KVAL_DEC register
    * @param[in] percentage percentage of the power supply voltage
    * applied to the motor windings, range 0.4% to 99.6%
    * @retval value for KVAL_RUN, KVAL_HOLD, KVAL_ACC or
    * KVAL_DEC register
    * @note The voltage applied is sinusoidal
    **********************************************************/
    uint8_t k_val_perc_to_reg_val(float percentage) {
        return ((uint8_t)(((float)(percentage)*2.56f)+0.5f));
    }

    /**********************************************************
    * @brief Converts values from KVAL_RUN, KVAL_HOLD, KVAL_ACC
    * or KVAL_DEC register to percentage
    * @param[in] regVal The KVAL_RUN, KVAL_HOLD, KVAL_ACC
    * or KVAL_DEC register value
    * @retval percentage of the power supply voltage applied to
    * the motor windings
    * @note The voltage applied is sinusoidal
    **********************************************************/
    float k_val_reg_val_to_perc(uint32_t regVal) {
        return (((float)(regVal))*0.390625f);
    }

    /**********************************************************
    * @brief Convert the float formatted speed into a MAX_SPEED
    * register value
    * @param[in] steps_s the speed as steps/s, range 15.25 to 15610 steps/s
    * @retval The speed as steps/tick
    **********************************************************/
    uint16_t max_spd_steps_s_to_reg_val(float steps_s) {
        return ((uint16_t)(((float)(steps_s)*0.065536f)+0.5f));
    }

    /**********************************************************
    * @brief Convert the MAX_SPEED register value into step/s
    * @param[in] regVal The MAX_SPEED register value
    * @retval The speed as steps/s
    **********************************************************/
    float max_spd_reg_val_to_steps_s(uint32_t regVal) {
        return (((float)(regVal))*15.258789f);
    }

    /**********************************************************
    * @brief Convert the float formatted speed into a MIN_SPEED
    * register value
    * @param[in] steps_s the speed as steps/s, range 0 to 976.3 steps/s
    * @retval The speed as steps/tick
    **********************************************************/
    uint16_t min_spd_steps_s_to_reg_val(float steps_s) {
        return ((uint16_t)(((float)(steps_s)*4.194304f)+0.5f));
    }

    /**********************************************************
    * @brief Convert the MIN_SPEED register value into step/s
    * @param[in] regVal The MIN_SPEED register value
    * @retval The speed as steps/s
    **********************************************************/
    float min_spd_reg_val_to_steps_s(uint32_t regVal) {
        return (((float)(regVal))*0.238418579f);
    }

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

    /**********************************************************
    * @brief Converts STALL or OCD Threshold voltage in mV to
    * values for STALL_TH or OCD_TH register
    * @param[in] mV voltage in mV, range 31.25mV to 1000mV
    * @retval value for STALL_TH or OCD_TH register
    **********************************************************/
    uint8_t stall_ocd_th_to_reg_val(float mV) {
        return ((uint8_t)((((float)(mV)-31.25f)*0.032f)+0.5f));
    }

    /**********************************************************
    * @brief Converts values from STALL_TH or OCD_TH register
    * to mV
    * @param[in] regVal The STALL_TH or OCD_TH register value
    * @retval voltage in mV
    **********************************************************/
    float stall_ocd_reg_val_to_th(uint32_t regVal) {
        return (((float)(regVal+1))*31.25f);
    }

    /**********************************************************
    * @brief Converts voltage in mV to values for TVAL_RUN,
    * TVAL_HOLD, TVAL_ACC or TVAL_DEC register
    * @param[in] voltage_mV voltage in mV, range 7.8mV to 1000mV
    * @retval value for TVAL_RUN, TVAL_HOLD, TVAL_ACC or
    * TVAL_DEC register
    * @note The voltage corresponds to a peak output current
    * accross the external sense power resistor
    **********************************************************/
    uint8_t t_val_ref_voltage_to_reg_val(float voltage_mV) {
        return ((uint8_t)((((float)(voltage_mV)-7.8125f)*0.128f)+0.5f));
    }

    /**********************************************************
    * @brief Converts values from TVAL_RUN, TVAL_HOLD, TVAL_ACC
    * or TVAL_DEC register to mV
    * @param[in] regVal The TVAL_RUN, TVAL_HOLD, TVAL_ACC
    * or TVAL_DEC register value
    * @retval voltage in mV
    * @note The voltage corresponds to a peak output current
    * accross the external sense power resistor
    **********************************************************/
    float t_val_reg_val_to_ref_voltage(uint32_t regVal) {
        return (((float)(regVal+1))*7.8125f);
    }

    /**********************************************************
    * @brief Convert time in us to values for TON_MIN register
    * @param[in] tmin_us time in us, range 0.5us to 64us
    * @retval value for TON_MIN register
    **********************************************************/
    uint8_t t_min_time_to_reg_val(float tmin_us) {
        return ((uint8_t)((((float)(tmin_us)-0.5f)*2.0f)+0.5f));
    }

};

#endif