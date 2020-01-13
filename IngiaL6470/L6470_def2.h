#ifndef L6470_def2_h
#define L6470_def2_h

/// masks for MIN_SPEED register of PowerStep01
#define L6470_LSPD_OPT_SHIFT            (12)
typedef enum {
  L6470_LSPD_OPT        = ((uint16_t)((0x1)<<L6470_LSPD_OPT_SHIFT)),
  L6470_MIN_SPEED_MASK  = ((uint16_t)0x0FFF)
} l6470_MinSpeedMasks_t;

/// Low speed optimization (MIN_SPEED register of PowerStep01)
typedef enum {
  L6470_LSPD_OPT_OFF    = ((uint16_t)0x0000),
  L6470_LSPD_OPT_ON     = ((uint16_t)L6470_LSPD_OPT)
} l6470_LspdOpt_t;

/// SYNC_SEL options (STEP_MODE register of PowerStep01)
#define L6470_SYNC_EN   ((0x1) << 7)
typedef enum {
  L6470_SYNC_SEL_DISABLED   = ((uint8_t)0x00),
  L6470_SYNC_SEL_1_2        = ((uint8_t)(L6470_SYNC_EN|0x00)),
  L6470_SYNC_SEL_1          = ((uint8_t)(L6470_SYNC_EN|0x10)),
  L6470_SYNC_SEL_2          = ((uint8_t)(L6470_SYNC_EN|0x20)),
  L6470_SYNC_SEL_4          = ((uint8_t)(L6470_SYNC_EN|0x30)),
  L6470_SYNC_SEL_8          = ((uint8_t)(L6470_SYNC_EN|0x40)),
  L6470_SYNC_SEL_16         = ((uint8_t)(L6470_SYNC_EN|0x50)),
  L6470_SYNC_SEL_32         = ((uint8_t)(L6470_SYNC_EN|0x60)),
  L6470_SYNC_SEL_64         = ((uint8_t)(L6470_SYNC_EN|0x70))
} l6470_SyncSel_t;

/// Alarms conditions (ALARM_EN register of PowerStep01)
typedef enum {
  L6470_ALARM_EN_OVERCURRENT        = ((uint8_t)0x01),
  L6470_ALARM_EN_THERMAL_SHUTDOWN   = ((uint8_t)0x02),
  L6470_ALARM_EN_THERMAL_WARNING    = ((uint8_t)0x04),
  L6470_ALARM_EN_UNDERVOLTAGE       = ((uint8_t)0x08),
  L6470_ALARM_EN_STALL_DETECTION_A  = ((uint8_t)0x10),
  L6470_ALARM_EN_STALL_DETECTION_B  = ((uint8_t)0x20),
  L6470_ALARM_EN_SW_TURN_ON         = ((uint8_t)0x40),
  L6470_ALARM_EN_WRONG_NPERF_CMD    = ((uint8_t)0x80)
} l6470_AlarmEn_t;

/// Oscillator management (EXT_CLK and OSC_SEL fields of CONFIG register of Powerstep01)
typedef enum {
  L6470_CONFIG_INT_16MHZ               = ((uint16_t)0x0000),
  L6470_CONFIG_INT_16MHZ_OSCOUT_2MHZ   = ((uint16_t)0x0008),
  L6470_CONFIG_INT_16MHZ_OSCOUT_4MHZ   = ((uint16_t)0x0009),
  L6470_CONFIG_INT_16MHZ_OSCOUT_8MHZ   = ((uint16_t)0x000A),
  L6470_CONFIG_INT_16MHZ_OSCOUT_16MHZ  = ((uint16_t)0x000B),
  L6470_CONFIG_EXT_8MHZ_XTAL_DRIVE     = ((uint16_t)0x0004),
  L6470_CONFIG_EXT_16MHZ_XTAL_DRIVE    = ((uint16_t)0x0005),
  L6470_CONFIG_EXT_24MHZ_XTAL_DRIVE    = ((uint16_t)0x0006),
  L6470_CONFIG_EXT_32MHZ_XTAL_DRIVE    = ((uint16_t)0x0007),
  L6470_CONFIG_EXT_8MHZ_OSCOUT_INVERT  = ((uint16_t)0x000C),
  L6470_CONFIG_EXT_16MHZ_OSCOUT_INVERT = ((uint16_t)0x000D),
  L6470_CONFIG_EXT_24MHZ_OSCOUT_INVERT = ((uint16_t)0x000E),
  L6470_CONFIG_EXT_32MHZ_OSCOUT_INVERT = ((uint16_t)0x000F)
} l6470_ConfigOscMgmt_t;

/// Oscillator management (EXT_CLK and OSC_SEL fields of CONFIG register of Powerstep01)
typedef enum {
  L6470_CONFIG_SW_HARD_STOP = ((uint16_t)0x0000),
  L6470_CONFIG_SW_USER      = ((uint16_t)0x0010)
} l6470_ConfigSwMode_t;

/// Voltage supply compensation enabling for voltage mode (EN_VSCOMP field of CONFIG register of Powerstep01)
typedef enum {
  L6470_CONFIG_VS_COMP_DISABLE  = ((uint16_t)0x0000),
  L6470_CONFIG_VS_COMP_ENABLE   = ((uint16_t)0x0020)
} l6470_ConfigEnVscomp_t;

/// Overcurrent shutdown (OC_SD field of CONFIG register of Powerstep01)
typedef enum {
  L6470_CONFIG_OC_SD_DISABLE  = ((uint16_t)0x0000),
  L6470_CONFIG_OC_SD_ENABLE   = ((uint16_t)0x0080)
} l6470_ConfigOcSd_t;

typedef enum {
  L6470_CONFIG_POW_SR_320 = ((uint16_t)0x0000),
  L6470_CONFIG_POW_SR_75  = ((uint16_t)0x0100),
  L6470_CONFIG_POW_SR_110 = ((uint16_t)0x0200),
  L6470_CONFIG_POW_SR_260 = ((uint16_t)0x0300)
} l6470_ConfigPowSR_t;

/// PWM frequency division factor (F_PWM_INT field of CONFIG register of Powerstep01)
#define L6470_CONFIG_PWM_DIV_SHIFT      (13)
typedef enum {
  L6470_CONFIG_PWM_DIV_1    = (((uint16_t)0x00)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_2    = (((uint16_t)0x01)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_3    = (((uint16_t)0x02)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_4    = (((uint16_t)0x03)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_5    = (((uint16_t)0x04)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_6    = (((uint16_t)0x05)<<L6470_CONFIG_PWM_DIV_SHIFT),
  L6470_CONFIG_PWM_DIV_7    = (((uint16_t)0x06)<<L6470_CONFIG_PWM_DIV_SHIFT)
} l6470_ConfigFPwmInt_t;

/// PWM frequency multiplication factor (F_PWM_DEC field of CONFIG register of Powerstep01)
#define L6470_CONFIG_PWM_MUL_SHIFT      (10)
typedef enum {
  L6470_CONFIG_PWM_MUL_0_625  = (((uint16_t)0x00)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_0_75   = (((uint16_t)0x01)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_0_875  = (((uint16_t)0x02)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_1      = (((uint16_t)0x03)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_1_25   = (((uint16_t)0x04)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_1_5    = (((uint16_t)0x05)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_1_75   = (((uint16_t)0x06)<<L6470_CONFIG_PWM_MUL_SHIFT),
  L6470_CONFIG_PWM_MUL_2      = (((uint16_t)0x07)<<L6470_CONFIG_PWM_MUL_SHIFT)
} l6470_ConfigFPwmDec_t;


typedef struct
{
  float     acceleration;           //!< motor acceleration [step/s^2]
  float     deceleration;           //!< motor deceleration [step/s^2]
  float     maxSpeed;               //!< motor maximum speed [step/s]
  float     minSpeed;               //!< motor minimum speed [step/s]
  l6470_LspdOpt_t lowSpeedOptimization;
  float fullStepSpeed;              //!< motor full-step speed threshold [step/s]
  float ocdThreshold;               //!< ocd threshold [ma] (range [375 ma, 6000 ma])
  step_mode_t stepMode;             //!< step mode selection
  l6470_SyncSel_t syncClockSelection;
  uint8_t alarmsSelection;          //!< alarm conditions enable    ######
  float     kvalHold;               //!< holding kval [V]
  float     kvalRun;                //!< constant speed kval [V]
  float     kvalAcc;                //!< acceleration starting kval [V]
  float     kvalDec;                //!< deceleration starting kval [V]
  float intersectSpeed;             //!< intersect speed for bemf compensation curve slope changing [step/s]
  float startSlope;                 //!< start slope [s/step]
  float accelerationFinalSlope;     //!< acceleration final slope [s/step]
  float decelerationFinalSlope;     //!< deceleration final slope [s/step]
  float thermalCompensationFactor;  //!< thermal compensation factor (range [0, 15])
  float stallThreshold;             //!< stall threshold [ma] (range [31.25 ma, 4000 ma])
  ///System clock source management
  l6470_ConfigOscMgmt_t oscClkSel;
  ///External switch to act as hard_stop interrupt or not
  l6470_ConfigSwMode_t swMode;
  ///Motor supply voltage compensation enable bit
  l6470_ConfigEnVscomp_t enVsComp;
  ///Overcurrent event causes or not the bridges to turn-off
  l6470_ConfigOcSd_t ocSd;
  /// Programmable power bridge output slew rate values
  l6470_ConfigPowSR_t powSR;
  ///Integer division factor of PWM frequency generation
  l6470_ConfigFPwmInt_t fPwmInt;
  ///Multiplication factor of PWM frequency generation
  l6470_ConfigFPwmDec_t fPwmDec;
} l6470_init_t;

#endif