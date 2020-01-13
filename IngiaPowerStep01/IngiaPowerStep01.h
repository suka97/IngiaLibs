#ifndef PowerStep01_h
#define PowerStep01_h

#include <IngiaST_StepperDriver.h>
#include "PowerStep01_def.h"
#include "PowerStep01_def2.h"

#define HW_SPI_SPEED 1000000

class IngiaPowerStep01 : public IngiaST_StepperDriver
{
    public:
        IngiaPowerStep01(uint8_t csPin, uint8_t rstPin = 0)
            : IngiaST_StepperDriver(csPin, rstPin)  {  }
        

    public:
        // ------------------- Metodos Primarios -------------------
        uint8_t getParamLen(powerstep01_Registers_t dir);
        using IngiaST_StepperDriver::getParam;  // avoid name hiding parent class
        uint32_t getParam(powerstep01_Registers_t dir) {
            return getParam((uint32_t)dir, getParamLen(dir));
        }
        using IngiaST_StepperDriver::setParam;  // avoid name hiding parent class
        void setParam(powerstep01_Registers_t dir, uint32_t value) {
            setParam((uint32_t)dir, getParamLen(dir), value);
        }
        void initDevice (void *deviceConfig = (void *)0);
        void setDeviceParams(powerstep01_init_u_t *initPrm);

        // ------------------- Conversions -------------------
        powerstep01_Status_t parseStatus(uint16_t status);

        // ------------------- Metodos Secundarios -------------------
        powerstep01_Status_t getStatus(void) {
            return parseStatus(getParam(POWERSTEP01_STATUS));
        }
        void setposition(int32_t absPos) {
            setParam(POWERSTEP01_ABS_POS, (uint32_t)absPos);
        }
        int32_t getPosition(void) {
            return getParam(POWERSTEP01_ABS_POS);
        }
        
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

       public:
        // ------------------ CONVERSIONS ------------------
        /**********************************************************
        * @brief Convert the ACC or DEC in step/s^2 into register value
        * @param[in] The speed as steps/s
        * @retval regVal The ACC or DEC register value
        **********************************************************/
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