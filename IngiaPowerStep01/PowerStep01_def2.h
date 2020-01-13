#ifndef PowerStep01_def2_h
#define PowerStep01_def2_h

/// powerSTEP01 max absolute position
#define POWERSTEP01_MAX_POSITION (int32_t)(0x001FFFFF)

/// powerSTEP01 min absolute position
#define POWERSTEP01_MIN_POSITION (int32_t)(0xFFE00000)
    
/// powerSTEP01 error base number
#define POWERSTEP01_ERROR_BASE              (0xB000)

/// powerSTEP01 acceleration and deceleration max value
#define POWERSTEP01_ACC_DEC_MAX_VALUE       (float)(59590)
/// powerSTEP01 max speed max value
#define POWERSTEP01_MAX_SPEED_MAX_VALUE     (float)(15610)
/// powerSTEP01 min speed max value
#define POWERSTEP01_MIN_SPEED_MAX_VALUE     (float)(976.3)
/// powerSTEP01 full step speed max value
#define POWERSTEP01_FS_SPD_MAX_VALUE        (float)(15625)
/// powerSTEP01 intersect speed max value
#define POWERSTEP01_INT_SPD_MAX_VALUE       (float)(976.5)
/// powerSTEP01 thermal compensation max value
#define POWERSTEP01_K_THERM_MAX_VALUE       (float)(1.46875)
/// powerSTEP01 thermal compensation min value
#define POWERSTEP01_K_THERM_MIN_VALUE       (float)(1)
/// powerSTEP01 thermal compensation max value
#define POWERSTEP01_STALL_OCD_TH_MAX_VALUE  (float)(1000)
/// powerSTEP01 thermal compensation max value
#define POWERSTEP01_K_THERM_MAX_VALUE       (float)(1.46875)
/// powerSTEP01 voltage amplitude regulation max value
#define POWERSTEP01_KVAL_MAX_VALUE          (float)(255/256)
/// powerSTEP01 BEMF compensation curve slope max value
#define POWERSTEP01_SLP_MAX_VALUE           (float)(0.4)
/// powerSTEP01 torque regulation DAC reference voltage max value
#define POWERSTEP01_TVAL_MAX_VALUE          (float)(1000)
/// powerSTEP01 minimum off and on time max value
#define POWERSTEP01_TOFF_TON_MIN_MAX_VALUE  (float)(64)
    
///Shift of the low speed optimization bit in MIN_SPEED register
#define POWERSTEP01_LSPD_OPT_SHIFT            (12)
///Shift of the boost mode bit in FS_SPD register
#define POWERSTEP01_BOOST_MODE_SHIFT          (10)
///Maximum fast decay time (TOFF_FAST) unit
#define POWERSTEP01_TOFF_FAST_UNIT_US         (2)
///Shift of the maximum fast decay time (TOFF_FAST) in T_FAST register
#define POWERSTEP01_TOFF_FAST_SHIFT           (4)
///Maximum fall step time (FAST_STEP) unit
#define POWERSTEP01_FAST_STEP_UNIT_US         (2)
///Shift of the maximum fall step time (FAST_STEP) in T_FAST register
#define POWERSTEP01_FAST_STEP_SHIFT           (0)
///Duration unit of constant current phase during gate turn-on and turn-off (TCC)
#define POWERSTEP01_TCC_UNIT_NS               (125)
///Shift of TCC field in GATECFG1 register
#define POWERSTEP01_TCC_SHIFT                 (0)
///Shift of IGATE field in GATECFG1 register
#define POWERSTEP01_IGATE_SHIFT               (5)
///Shift of TBOOST field in GATECFG1 register
#define POWERSTEP01_TBOOST_SHIFT              (8)
///Duration unit of the blanking of the current sensing comparators (TBLANK)
#define POWERSTEP01_TBLANK_UNIT_NS            (125)
///Shift of TBLANK field in GATECFG2 register
#define POWERSTEP01_TBLANK_SHIFT              (5)
///Deadtime duration unit between gate turn-off and opposite gate turn-on (TDT)
#define POWERSTEP01_TDT_UNIT_NS               (125)
///Shift of TDT field in GATECFG2 register
#define POWERSTEP01_TDT_SHIFT                 (0)
///Shift of F_PWM_INT field in CONFIG register for voltage mode
#define POWERSTEP01_CONFIG_PWM_DIV_SHIFT      (13)
///Shift of F_PWM_DEC field in CONFIG register for voltage mode
#define POWERSTEP01_CONFIG_PWM_MUL_SHIFT      (10)
///Target switching period (TSW) unit
#define POWERSTEP01_CONFIG_TSW_UNIT_US        (4)
///Shift of TSW field in CONFIG register for current mode
#define POWERSTEP01_CONFIG_TSW_SHIFT          (10)
///Shift of MOT_STATUS field in STATUS register
#define POWERSTEP01_STATUS_MOT_STATUS_SHIFT   (5)


/// masks for EL_POS register of PowerStep01
typedef enum {
  POWERSTEP01_ELPOS_STEP_MASK       = ((uint16_t)0x180),
  POWERSTEP01_ELPOS_MICROSTEP_MASK  = ((uint16_t)0x07F)
} powerstep01_ElPosMasks_t;

/// masks for MIN_SPEED register of PowerStep01
typedef enum {
  POWERSTEP01_LSPD_OPT        = ((uint16_t)((0x1)<<POWERSTEP01_LSPD_OPT_SHIFT)),
  POWERSTEP01_MIN_SPEED_MASK  = ((uint16_t)0x0FFF)
} powerstep01_MinSpeedMasks_t;

/// Low speed optimization (MIN_SPEED register of PowerStep01)
typedef enum {
  POWERSTEP01_LSPD_OPT_OFF    = ((uint16_t)0x0000),
  POWERSTEP01_LSPD_OPT_ON     = ((uint16_t)POWERSTEP01_LSPD_OPT)
} powerstep01_LspdOpt_t;

/// masks for FS_SPD register of PowerStep01
typedef enum {
  POWERSTEP01_BOOST_MODE   = ((uint16_t)((0x1)<<POWERSTEP01_BOOST_MODE_SHIFT)),
  POWERSTEP01_FS_SPD_MASK  = ((uint16_t)0x03FF)
} powerstep01_FsSpdMasks_t;

/// Full step boost (FS_SPD register of PowerStep01)
typedef enum {
  POWERSTEP01_BOOST_MODE_OFF    = ((uint16_t)0x0000),
  POWERSTEP01_BOOST_MODE_ON     = ((uint16_t)POWERSTEP01_BOOST_MODE)
} powerstep01_BoostMode_t;

/// masks for T_FAST register of PowerStep01
typedef enum {
  POWERSTEP01_FAST_STEP_MASK  = ((uint16_t) ((0xF)<<POWERSTEP01_FAST_STEP_SHIFT)),
  POWERSTEP01_TOFF_FAST_MASK  = ((uint16_t) ((0xF)<<POWERSTEP01_TOFF_FAST_SHIFT))
} powerstep01_TFastMasks_t;

/// Maximum fall step times (T_FAST register of PowerStep01)
typedef enum {
  POWERSTEP01_FAST_STEP_2us     = (((uint8_t)0x00)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_4us     = (((uint8_t)0x01)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_6us     = (((uint8_t)0x02)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_8us     = (((uint8_t)0x03)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_10us    = (((uint8_t)0x04)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_12us    = (((uint8_t)0x05)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_14us    = (((uint8_t)0x06)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_16us    = (((uint8_t)0x07)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_18us    = (((uint8_t)0x08)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_20us    = (((uint8_t)0x09)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_22us    = (((uint8_t)0x0A)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_24s     = (((uint8_t)0x0B)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_26us    = (((uint8_t)0x0C)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_28us    = (((uint8_t)0x0D)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_30us    = (((uint8_t)0x0E)<<POWERSTEP01_FAST_STEP_SHIFT),
  POWERSTEP01_FAST_STEP_32us    = (((uint8_t)0x0F)<<POWERSTEP01_FAST_STEP_SHIFT)
} powerstep01_FastStep_t;

/// Maximum fast decay times (T_FAST register of PowerStep01)
typedef enum {
  POWERSTEP01_TOFF_FAST_2us     = (((uint8_t)0x00)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_4us     = (((uint8_t)0x01)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_6us     = (((uint8_t)0x02)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_8us     = (((uint8_t)0x03)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_10us    = (((uint8_t)0x04)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_12us    = (((uint8_t)0x05)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_14us    = (((uint8_t)0x06)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_16us    = (((uint8_t)0x07)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_18us    = (((uint8_t)0x08)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_20us    = (((uint8_t)0x09)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_22us    = (((uint8_t)0x0A)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_24us    = (((uint8_t)0x0B)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_26us    = (((uint8_t)0x0C)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_28us    = (((uint8_t)0x0D)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_30us    = (((uint8_t)0x0E)<<POWERSTEP01_TOFF_FAST_SHIFT),
  POWERSTEP01_TOFF_FAST_32us    = (((uint8_t)0x0F)<<POWERSTEP01_TOFF_FAST_SHIFT)
} powerstep01_ToffFast_t;

/// Overcurrent threshold options (OCD register of PowerStep01)
typedef enum {
  POWERSTEP01_OCD_TH_31_25mV    = ((uint8_t)0x00),
  POWERSTEP01_OCD_TH_62_5mV     = ((uint8_t)0x01),
  POWERSTEP01_OCD_TH_93_75mV    = ((uint8_t)0x02),
  POWERSTEP01_OCD_TH_125mV      = ((uint8_t)0x03),
  POWERSTEP01_OCD_TH_156_25mV   = ((uint8_t)0x04),
  POWERSTEP01_OCD_TH_187_50mV   = ((uint8_t)0x05),
  POWERSTEP01_OCD_TH_218_75mV   = ((uint8_t)0x06),
  POWERSTEP01_OCD_TH_250mV      = ((uint8_t)0x07),
  POWERSTEP01_OCD_TH_281_25mV   = ((uint8_t)0x08),
  POWERSTEP01_OCD_TH_312_5mV    = ((uint8_t)0x09),
  POWERSTEP01_OCD_TH_343_75mV   = ((uint8_t)0x0A),
  POWERSTEP01_OCD_TH_375mV      = ((uint8_t)0x0B),
  POWERSTEP01_OCD_TH_406_25mV   = ((uint8_t)0x0C),
  POWERSTEP01_OCD_TH_437_5mV    = ((uint8_t)0x0D),
  POWERSTEP01_OCD_TH_468_75mV   = ((uint8_t)0x0E),
  POWERSTEP01_OCD_TH_500mV      = ((uint8_t)0x0F),
  POWERSTEP01_OCD_TH_531_25mV   = ((uint8_t)0x10),
  POWERSTEP01_OCD_TH_562_5mV    = ((uint8_t)0x11),
  POWERSTEP01_OCD_TH_593_75mV   = ((uint8_t)0x12),
  POWERSTEP01_OCD_TH_625mV      = ((uint8_t)0x13),
  POWERSTEP01_OCD_TH_656_25mV   = ((uint8_t)0x14),
  POWERSTEP01_OCD_TH_687_5mV    = ((uint8_t)0x15),
  POWERSTEP01_OCD_TH_718_75mV   = ((uint8_t)0x16),
  POWERSTEP01_OCD_TH_750mV      = ((uint8_t)0x17),
  POWERSTEP01_OCD_TH_781_25mV   = ((uint8_t)0x18),
  POWERSTEP01_OCD_TH_812_5mV    = ((uint8_t)0x19),
  POWERSTEP01_OCD_TH_843_75mV   = ((uint8_t)0x1A),
  POWERSTEP01_OCD_TH_875mV      = ((uint8_t)0x1B),
  POWERSTEP01_OCD_TH_906_25mV   = ((uint8_t)0x1C),
  POWERSTEP01_OCD_TH_937_75mV   = ((uint8_t)0x1D),
  POWERSTEP01_OCD_TH_968_75mV   = ((uint8_t)0x1E),
  POWERSTEP01_OCD_TH_1V         = ((uint8_t)0x1F)        
} powerstep01_OcdTh_t;

/// masks for STEP_MODE register of PowerStep01
typedef enum {
  POWERSTEP01_STEP_MODE_STEP_SEL    = ((uint8_t)0x07),
  POWERSTEP01_STEP_MODE_CM_VM       = ((uint8_t)0x08),
  POWERSTEP01_STEP_MODE_SYNC_SEL    = ((uint8_t)0x70),
  POWERSTEP01_STEP_MODE_SYNC_EN     = ((uint8_t)0x80)
} powerstep01_StepModeMasks_t;

/// Voltage or Current mode selection (CM_VM field of STEP_MODE register of PowerStep01)
typedef enum {
  POWERSTEP01_CM_VM_VOLTAGE    = ((uint8_t)0x00),
  POWERSTEP01_CM_VM_CURRENT    = ((uint8_t)0x08)
} powerstep01_CmVm_t;

/// Stepping options (field STEP_SEL of STEP_MODE register of PowerStep01)
typedef enum {
  POWERSTEP01_STEP_SEL_1      = ((uint8_t)0x00),
  POWERSTEP01_STEP_SEL_1_2    = ((uint8_t)0x01),
  POWERSTEP01_STEP_SEL_1_4    = ((uint8_t)0x02),
  POWERSTEP01_STEP_SEL_1_8    = ((uint8_t)0x03),
  POWERSTEP01_STEP_SEL_1_16   = ((uint8_t)0x04),
  POWERSTEP01_STEP_SEL_1_32   = ((uint8_t)0x05),
  POWERSTEP01_STEP_SEL_1_64   = ((uint8_t)0x06),
  POWERSTEP01_STEP_SEL_1_128  = ((uint8_t)0x07)
} powerstep01_StepSel_t;

/// Powerstep01 Sync Output frequency enabling bitw
#define POWERSTEP01_SYNC_EN   ((0x1) << 7)

/// SYNC_SEL options (STEP_MODE register of PowerStep01)
typedef enum {
  POWERSTEP01_SYNC_SEL_DISABLED   = ((uint8_t)0x00),
  POWERSTEP01_SYNC_SEL_1_2        = ((uint8_t)(POWERSTEP01_SYNC_EN|0x00)),
  POWERSTEP01_SYNC_SEL_1          = ((uint8_t)(POWERSTEP01_SYNC_EN|0x10)),
  POWERSTEP01_SYNC_SEL_2          = ((uint8_t)(POWERSTEP01_SYNC_EN|0x20)),
  POWERSTEP01_SYNC_SEL_4          = ((uint8_t)(POWERSTEP01_SYNC_EN|0x30)),
  POWERSTEP01_SYNC_SEL_8          = ((uint8_t)(POWERSTEP01_SYNC_EN|0x40)),
  POWERSTEP01_SYNC_SEL_16         = ((uint8_t)(POWERSTEP01_SYNC_EN|0x50)),
  POWERSTEP01_SYNC_SEL_32         = ((uint8_t)(POWERSTEP01_SYNC_EN|0x60)),
  POWERSTEP01_SYNC_SEL_64         = ((uint8_t)(POWERSTEP01_SYNC_EN|0x70))
} powerstep01_SyncSel_t;

/// Alarms conditions (ALARM_EN register of PowerStep01)
typedef enum {
  POWERSTEP01_ALARM_EN_OVERCURRENT        = ((uint8_t)0x01),
  POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN   = ((uint8_t)0x02),
  POWERSTEP01_ALARM_EN_THERMAL_WARNING    = ((uint8_t)0x04),
  POWERSTEP01_ALARM_EN_UVLO               = ((uint8_t)0x08),
  POWERSTEP01_ALARM_EN_ADC_UVLO           = ((uint8_t)0x10),
  POWERSTEP01_ALARM_EN_STALL_DETECTION    = ((uint8_t)0x20),
  POWERSTEP01_ALARM_EN_SW_TURN_ON         = ((uint8_t)0x40),
  POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD    = ((uint8_t)0x80)
} powerstep01_AlarmEn_t;


/// masks for GATECFG1 register of PowerStep01
typedef enum {
  POWERSTEP01_GATECFG1_TCC_MASK    = ((uint16_t)0x001F),
  POWERSTEP01_GATECFG1_IGATE_MASK  = ((uint16_t)0x00E0),
  POWERSTEP01_GATECFG1_TBOOST_MASK = ((uint16_t)0x0700),
  POWERSTEP01_GATECFG1_WD_EN       = ((uint16_t)0x0800)
} powerstep01_GateCfg1Masks_t;

/// Control current Time (field TCC of GATECFG1 register of PowerStep01)
typedef enum {
  POWERSTEP01_TCC_125ns       = (((uint8_t)0x00)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_250ns       = (((uint8_t)0x01)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_375ns       = (((uint8_t)0x02)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_500ns       = (((uint8_t)0x03)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_625ns       = (((uint8_t)0x04)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_750ns       = (((uint8_t)0x05)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_875ns       = (((uint8_t)0x06)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1000ns      = (((uint8_t)0x07)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1125ns      = (((uint8_t)0x08)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1250ns      = (((uint8_t)0x09)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1375ns      = (((uint8_t)0x0A)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1500ns      = (((uint8_t)0x0B)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1625ns      = (((uint8_t)0x0C)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1750ns      = (((uint8_t)0x0D)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_1875ns      = (((uint8_t)0x0E)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2000ns      = (((uint8_t)0x0F)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2125ns      = (((uint8_t)0x10)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2250ns      = (((uint8_t)0x11)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2375ns      = (((uint8_t)0x12)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2500ns      = (((uint8_t)0x13)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2625ns      = (((uint8_t)0x14)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2750ns      = (((uint8_t)0x15)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_2875ns      = (((uint8_t)0x16)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3000ns      = (((uint8_t)0x17)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3125ns      = (((uint8_t)0x18)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3250ns      = (((uint8_t)0x19)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3375ns      = (((uint8_t)0x1A)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3500ns      = (((uint8_t)0x1B)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3625ns      = (((uint8_t)0x1C)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3750ns      = (((uint8_t)0x1D)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3750ns_bis  = (((uint8_t)0x1E)<<POWERSTEP01_TCC_SHIFT),
  POWERSTEP01_TCC_3750ns_ter  = (((uint8_t)0x1F)<<POWERSTEP01_TCC_SHIFT)
} powerstep01_Tcc_t;

/// Igate options (GATECFG1 register of PowerStep01)
typedef enum {
  POWERSTEP01_IGATE_4mA     = (((uint8_t)0x00)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_4mA_Bis = (((uint8_t)0x01)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_8mA     = (((uint8_t)0x02)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_16mA    = (((uint8_t)0x03)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_24mA    = (((uint8_t)0x04)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_32mA    = (((uint8_t)0x05)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_64mA    = (((uint8_t)0x06)<<POWERSTEP01_IGATE_SHIFT),
  POWERSTEP01_IGATE_96mA    = (((uint8_t)0x07)<<POWERSTEP01_IGATE_SHIFT),
} powerstep01_Igate_t;

/// Turn off boost time (TBOOST field of GATECFG1 register of PowerStep01)
typedef enum {
  POWERSTEP01_TBOOST_0ns                = (((uint8_t)0x00)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_62_5__83_3__125ns  = (((uint8_t)0x01)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_125ns              = (((uint8_t)0x02)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_250ns              = (((uint8_t)0x03)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_375ns              = (((uint8_t)0x04)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_500ns              = (((uint8_t)0x05)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_750ns              = (((uint8_t)0x06)<<POWERSTEP01_TBOOST_SHIFT),
  POWERSTEP01_TBOOST_1000ns             = (((uint8_t)0x07)<<POWERSTEP01_TBOOST_SHIFT),
} powerstep01_Tboost_t;

/// External clock watchdog (WD_EN field of GATECFG1 register of PowerStep01)
typedef enum {
  POWERSTEP01_WD_EN_DISABLE   = ((uint16_t)0x0000),
  POWERSTEP01_WD_EN_ENABLE    = ((uint16_t) ((0x1) << 11))
} powerstep01_WdEn_t;


/// masks for GATECFG2 register of PowerStep01
typedef enum {
  POWERSTEP01_GATECFG2_TDT      = ((uint8_t)0x1F),
  POWERSTEP01_GATECFG2_TBLANK   = ((uint8_t)0xE0)
} powerstep01_GateCfg2Masks_t;

/// Blanking time (TBLANK field of GATECFG2 register of PowerStep01)
typedef enum {
  POWERSTEP01_TBLANK_125ns    = (((uint8_t)0x00)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_250ns    = (((uint8_t)0x01)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_375ns    = (((uint8_t)0x02)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_500ns    = (((uint8_t)0x03)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_625ns    = (((uint8_t)0x04)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_750ns    = (((uint8_t)0x05)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_875ns    = (((uint8_t)0x06)<<POWERSTEP01_TBLANK_SHIFT),
  POWERSTEP01_TBLANK_1000ns   = (((uint8_t)0x07)<<POWERSTEP01_TBLANK_SHIFT),
} powerstep01_TBlank_t;

/// Dead time (TDT field of GATECFG2 register of PowerStep01)
typedef enum {
  POWERSTEP01_TDT_125ns   = (((uint8_t)0x00)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_250ns   = (((uint8_t)0x01)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_375ns   = (((uint8_t)0x02)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_500ns   = (((uint8_t)0x03)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_625ns   = (((uint8_t)0x04)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_750ns   = (((uint8_t)0x05)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_875ns   = (((uint8_t)0x06)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1000ns  = (((uint8_t)0x07)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1125ns  = (((uint8_t)0x08)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1250ns  = (((uint8_t)0x09)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1375ns  = (((uint8_t)0x0A)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1500ns  = (((uint8_t)0x0B)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1625ns  = (((uint8_t)0x0C)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1750ns  = (((uint8_t)0x0D)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_1875ns  = (((uint8_t)0x0E)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2000ns  = (((uint8_t)0x0F)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2125ns  = (((uint8_t)0x10)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2250ns  = (((uint8_t)0x11)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2375ns  = (((uint8_t)0x12)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2500ns  = (((uint8_t)0x13)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2625ns  = (((uint8_t)0x14)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2750ns  = (((uint8_t)0x15)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_2875ns  = (((uint8_t)0x16)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3000ns  = (((uint8_t)0x17)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3125ns  = (((uint8_t)0x18)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3250ns  = (((uint8_t)0x19)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3375ns  = (((uint8_t)0x1A)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3500ns  = (((uint8_t)0x1B)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3625ns  = (((uint8_t)0x1C)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3750ns  = (((uint8_t)0x1D)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_3875ns  = (((uint8_t)0x1E)<<POWERSTEP01_TDT_SHIFT),
  POWERSTEP01_TDT_4000ns  = (((uint8_t)0x1F)<<POWERSTEP01_TDT_SHIFT)
} powerstep01_Tdt_t;

/// Masks for CONFIG register of Powerstep01 
typedef enum {
  POWERSTEP01_CONFIG_OSC_SEL      = ((uint16_t)0x0007),
  POWERSTEP01_CONFIG_EXT_CLK      = ((uint16_t)0x0008),
  POWERSTEP01_CONFIG_SW_MODE      = ((uint16_t)0x0010),
  POWERSTEP01_CONFIG_OC_SD        = ((uint16_t)0x0080),
  POWERSTEP01_CONFIG_UVLOVAL      = ((uint16_t)0x0100),
  POWERSTEP01_CONFIG_VCCVAL       = ((uint16_t)0x0200),
  // Masks specific for voltage mode
  POWERSTEP01_CONFIG_EN_VSCOMP    = ((uint16_t)0x0020),
  POWERSTEP01_CONFIG_F_PWM_DEC    = ((uint16_t)0x1C00),
  POWERSTEP01_CONFIG_F_PWM_INT    = ((uint16_t)0xE000),
  // Masks specific for current mode
  POWERSTEP01_CONFIG_TSW          = ((uint16_t)0x7C00),
  POWERSTEP01_CONFIG_PRED_EN      = ((uint16_t)0x8000)  
} powerstep01_ConfigMasks_t;

/// Masks for CONFIG register of Powerstep01  (specific for current mode)
#define POWERSTEP01_CONFIG_EN_TQREG (POWERSTEP01_CONFIG_EN_VSCOMP) 

/// Oscillator management (EXT_CLK and OSC_SEL fields of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_INT_16MHZ               = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ   = ((uint16_t)0x0008),
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_4MHZ   = ((uint16_t)0x0009),
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_8MHZ   = ((uint16_t)0x000A),
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_16MHZ  = ((uint16_t)0x000B),
  POWERSTEP01_CONFIG_EXT_8MHZ_XTAL_DRIVE     = ((uint16_t)0x0004),
  POWERSTEP01_CONFIG_EXT_16MHZ_XTAL_DRIVE    = ((uint16_t)0x0005),
  POWERSTEP01_CONFIG_EXT_24MHZ_XTAL_DRIVE    = ((uint16_t)0x0006),
  POWERSTEP01_CONFIG_EXT_32MHZ_XTAL_DRIVE    = ((uint16_t)0x0007),
  POWERSTEP01_CONFIG_EXT_8MHZ_OSCOUT_INVERT  = ((uint16_t)0x000C),
  POWERSTEP01_CONFIG_EXT_16MHZ_OSCOUT_INVERT = ((uint16_t)0x000D),
  POWERSTEP01_CONFIG_EXT_24MHZ_OSCOUT_INVERT = ((uint16_t)0x000E),
  POWERSTEP01_CONFIG_EXT_32MHZ_OSCOUT_INVERT = ((uint16_t)0x000F)
} powerstep01_ConfigOscMgmt_t;

/// Oscillator management (EXT_CLK and OSC_SEL fields of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_SW_HARD_STOP = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_SW_USER      = ((uint16_t)0x0010)
} powerstep01_ConfigSwMode_t;

/// Voltage supply compensation enabling for voltage mode (EN_VSCOMP field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_VS_COMP_DISABLE  = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_VS_COMP_ENABLE   = ((uint16_t)0x0020)
} powerstep01_ConfigEnVscomp_t;

/// External torque regulation enabling (EN_TQREG field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_TQ_REG_TVAL_USED = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_TQ_REG_ADC_OUT   = ((uint16_t)0x0020)
} powerstep01_ConfigEnTqReg_t;

/// Overcurrent shutdown (OC_SD field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_OC_SD_DISABLE  = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_OC_SD_ENABLE   = ((uint16_t)0x0080)
} powerstep01_ConfigOcSd_t;

/// UVLO thresholds (UVLOVAL field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_UVLOVAL_LOW      = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_UVLOVAL_HIGH     = ((uint16_t)0x0100),
} powerstep01_ConfigUvLoVal_t;

/// Vcc voltage  (VCCVAL field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_VCCVAL_7_5V    = ((uint16_t)0x0000),
  POWERSTEP01_CONFIG_VCCVAL_15V     = ((uint16_t)0x0200)
} powerstep01_ConfigVccVal_t;

/// PWM frequency division factor (F_PWM_INT field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_PWM_DIV_1    = (((uint16_t)0x00)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_2    = (((uint16_t)0x01)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_3    = (((uint16_t)0x02)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_4    = (((uint16_t)0x03)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_5    = (((uint16_t)0x04)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_6    = (((uint16_t)0x05)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT),
  POWERSTEP01_CONFIG_PWM_DIV_7    = (((uint16_t)0x06)<<POWERSTEP01_CONFIG_PWM_DIV_SHIFT)
} powerstep01_ConfigFPwmInt_t;

/// PWM frequency multiplication factor (F_PWM_DEC field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_PWM_MUL_0_625  = (((uint16_t)0x00)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_0_75   = (((uint16_t)0x01)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_0_875  = (((uint16_t)0x02)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_1      = (((uint16_t)0x03)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_1_25   = (((uint16_t)0x04)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_1_5    = (((uint16_t)0x05)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_1_75   = (((uint16_t)0x06)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT),
  POWERSTEP01_CONFIG_PWM_MUL_2      = (((uint16_t)0x07)<<POWERSTEP01_CONFIG_PWM_MUL_SHIFT)
} powerstep01_ConfigFPwmDec_t;

/// Switching period  (TSW field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_TSW_004us    =(((uint16_t)0x01)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_008us    =(((uint16_t)0x02)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_012us    =(((uint16_t)0x03)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_016us    =(((uint16_t)0x04)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_020us    =(((uint16_t)0x05)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_024us    =(((uint16_t)0x06)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_028us    =(((uint16_t)0x07)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_032us    =(((uint16_t)0x08)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_036us    =(((uint16_t)0x09)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_040us    =(((uint16_t)0x0A)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_044us    =(((uint16_t)0x0B)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_048us    =(((uint16_t)0x0C)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_052us    =(((uint16_t)0x0D)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_056us    =(((uint16_t)0x0E)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_060us    =(((uint16_t)0x0F)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_064us    =(((uint16_t)0x10)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_068us    =(((uint16_t)0x11)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_072us    =(((uint16_t)0x12)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_076us    =(((uint16_t)0x13)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_080us    =(((uint16_t)0x14)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_084us    =(((uint16_t)0x15)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_088us    =(((uint16_t)0x16)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_092us    =(((uint16_t)0x17)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_096us    =(((uint16_t)0x18)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_100us    =(((uint16_t)0x19)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_104us    =(((uint16_t)0x1A)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_108us    =(((uint16_t)0x1B)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_112us    =(((uint16_t)0x1C)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_116us    =(((uint16_t)0x1D)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_120us    =(((uint16_t)0x1E)<<POWERSTEP01_CONFIG_TSW_SHIFT),
  POWERSTEP01_CONFIG_TSW_124us    =(((uint16_t)0x1F)<<POWERSTEP01_CONFIG_TSW_SHIFT)        
} powerstep01_ConfigTsw_t;

/// Voltage supply compensation enabling for current mode(EN_PRED field of CONFIG register of Powerstep01)
typedef enum {
  POWERSTEP01_CONFIG_PRED_DISABLE =((uint16_t)0x0000),
  POWERSTEP01_CONFIG_PRED_ENABLE  =((uint16_t)0x8000)
} powerstep01_ConfigPredEn_t;

/*
/// Motor state (MOT_STATUS filed of STATUS register of PowerStep01)
typedef enum {
  POWERSTEP01_STATUS_MOT_STATUS_STOPPED       = (((uint16_t)0x0000)<<POWERSTEP01_STATUS_MOT_STATUS_SHIFT),
  POWERSTEP01_STATUS_MOT_STATUS_ACCELERATION  = (((uint16_t)0x0001)<<POWERSTEP01_STATUS_MOT_STATUS_SHIFT),
  POWERSTEP01_STATUS_MOT_STATUS_DECELERATION  = (((uint16_t)0x0002)<<POWERSTEP01_STATUS_MOT_STATUS_SHIFT),
  POWERSTEP01_STATUS_MOT_STATUS_CONST_SPD     = (((uint16_t)0x0003)<<POWERSTEP01_STATUS_MOT_STATUS_SHIFT)
} powerstep01_Status_t;
*/

///Initialization parameters structure common to current and voltage modes
typedef struct
{
  ///Current or voltage mode selection
  powerstep01_CmVm_t cmVmSelection;
  ///Acceleration 
  float acceleration;
  ///Deceleration
  float deceleration;
  ///Maximum speed
  float maxSpeed;
  ///Minimum speed
  float minSpeed;
  ///Low speed optimization bit
  powerstep01_LspdOpt_t lowSpeedOptimization;
  ///Full step speed
  float fullStepSpeed;
  ///Boost mode bit
  powerstep01_BoostMode_t boostMode;
  ///Over current detection threshold
  float ocdThreshold;
  ///Step mode
  step_mode_t stepMode;
  ///Sync clock selection
  powerstep01_SyncSel_t syncClockSelection;
  ///Alarm selection
  uint8_t alarmsSelection;
  ///Sink or source current used by gate driving circuitry
  powerstep01_Igate_t iGate;
  ///Duration of the overboost phase during gate turn-off
  powerstep01_Tboost_t tBoost;
  ///Duration of constant current phase during gate turn-on and turn-off
  powerstep01_Tcc_t tcc;
  ///Clock source monitoring enable bit
  powerstep01_WdEn_t wdEn;
  ///Duration of the blanking of the current sensing comparators
  powerstep01_TBlank_t tBlank;
  ///Deadtime duration between gate turn-off and opposite gate turn-on
  powerstep01_Tdt_t tdt;  
} commonParameters_t;

///Initialization parameters structure for voltage mode
typedef struct
{
  ///Parameters common to current and voltage modes
  commonParameters_t cp;
  ///Voltage amplitude regulation when the motor is stopped
  float kvalHold;
  ///Voltage amplitude regulation when the motor is running at constant speed
  float kvalRun;
  ///Voltage amplitude regulation during motor acceleration
  float kvalAcc;
  ///Voltage amplitude regulation during motor deceleration
  float kvalDec;
  ///Speed value at which the BEMF compensation curve changes slope  
  float intersectSpeed;
  ///BEMF compensation curve slope when speed is lower than intersect speed
  float startSlope;
  ///BEMF compensation curve slope when speed is greater than intersect speed during acceleration
  float accelerationFinalSlope;
  ///BEMF compensation curve slope when speed is greater than intersect speed during deceleration
  float decelerationFinalSlope;
  ///Winding resistance thermal drift compensation coefficient
  float thermalCompensationFactor;
  ///Stall detection threshold
  float stallThreshold;
  ///System clock source management
  powerstep01_ConfigOscMgmt_t oscClkSel;
  ///External switch to act as hard_stop interrupt or not
  powerstep01_ConfigSwMode_t swMode;
  ///Motor supply voltage compensation enable bit
  powerstep01_ConfigEnVscomp_t enVsComp;
  ///Overcurrent event causes or not the bridges to turn-off
  powerstep01_ConfigOcSd_t ocSd;
  ///UVLO protection thresholds
  powerstep01_ConfigUvLoVal_t uvloVal;
  ///Internal VCC regulator output voltage
  powerstep01_ConfigVccVal_t vccVal;
  ///Integer division factor of PWM frequency generation
  powerstep01_ConfigFPwmInt_t fPwmInt;
  ///Multiplication factor of PWM frequency generation
  powerstep01_ConfigFPwmDec_t fPwmDec;
} powerstep01_VoltageMode_init_t;

///Initialization parameters structure for current mode
typedef struct
{
  ///Parameters common to current and voltage modes
  commonParameters_t cp;
  ///Torque regulation DAC reference voltage when motor is stopped
  float tvalHold;
  ///Torque regulation DAC reference voltage when motor is runnig at constant speed
  float tvalRun;
  ///Torque regulation DAC reference voltage during motor acceleration
  float tvalAcc;
  ///Torque regulation DAC reference voltage during motor deceleration
  float tvalDec;
  ///Maximum fast decay time
  powerstep01_ToffFast_t toffFast;
  ///Maximum fall step time
  powerstep01_FastStep_t fastStep;
  ///Minimum on-time
  float tonMin;
  ///Minimum off-time
  float toffMin;
  ///System clock source management
  powerstep01_ConfigOscMgmt_t oscClkSel;
  ///External switch to act as hard_stop interrupt or not
  powerstep01_ConfigSwMode_t swMode;
  ///Peak current is adjusted through the ADCIN input or not
  powerstep01_ConfigEnTqReg_t tqReg;
  ///Motor supply voltage compensation enable bit
  powerstep01_ConfigEnVscomp_t enVsComp;
  ///Overcurrent event causes or not the bridges to turn-off
  powerstep01_ConfigOcSd_t ocSd;
  ///UVLO protection thresholds
  powerstep01_ConfigUvLoVal_t uvloVal;
  ///Internal VCC regulator output voltage
  powerstep01_ConfigVccVal_t vccVal;
  ///target switching period
  powerstep01_ConfigTsw_t tsw;
  ///predictive current control method enable bit
  powerstep01_ConfigPredEn_t predEn;

} powerstep01_CurrentMode_init_t;

///Union of current and volatge modes initialization parameters structures
typedef union powerstep01_init_u powerstep01_init_u_t;
union powerstep01_init_u
{
  ///Initialization parameters structure for current mode
  powerstep01_CurrentMode_init_t cm;
  ///Initialization parameters structure for voltage mode
  powerstep01_VoltageMode_init_t vm;
};

#endif