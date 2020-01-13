#ifndef PowerStep01_def_h
#define PowerStep01_def_h

// STATUS struct
struct powerstep01_Status_t {
    bool hiZ;
    bool busy;
    sw_Status_t sw_f;
    bool sw_evn;
    direction_t dir;
    motorStatus_t mot_status;
    bool cmd_error;
    bool stck_mod;
    bool uvlo;
    bool uvlo_adc;
    th_Status_t th_status;
    bool ocd;
    bool stall_B;
    bool stall_A;
};

/// Bit mask for STATUS Register of PowerStep01²
typedef enum {
  POWERSTEP01_STATUS_HIZ          = (((uint16_t)0x0001)),
  POWERSTEP01_STATUS_BUSY         = (((uint16_t)0x0002)),
  POWERSTEP01_STATUS_SW_F         = (((uint16_t)0x0004)),
  POWERSTEP01_STATUS_SW_EVN       = (((uint16_t)0x0008)),
  POWERSTEP01_STATUS_DIR          = (((uint16_t)0x0010)),
  POWERSTEP01_STATUS_MOT_STATUS   = (((uint16_t)0x0060)),
  POWERSTEP01_STATUS_CMD_ERROR    = (((uint16_t)0x0080)),
  POWERSTEP01_STATUS_STCK_MOD     = (((uint16_t)0x0100)),
  POWERSTEP01_STATUS_UVLO         = (((uint16_t)0x0200)),
  POWERSTEP01_STATUS_UVLO_ADC     = (((uint16_t)0x0400)),
  POWERSTEP01_STATUS_TH_STATUS    = (((uint16_t)0x1800)),
  POWERSTEP01_STATUS_OCD          = (((uint16_t)0x2000)),
  POWERSTEP01_STATUS_STALL_A      = (((uint16_t)0x4000)),
  POWERSTEP01_STATUS_STALL_B      = (((uint16_t)0x8000))
} powerstep01_StatusMasks_t;

/// Powerstep01 internal register addresses
typedef enum {
  POWERSTEP01_ABS_POS     = ((uint8_t)0x01),
  POWERSTEP01_EL_POS      = ((uint8_t)0x02),
  POWERSTEP01_MARK        = ((uint8_t)0x03), 
  POWERSTEP01_SPEED       = ((uint8_t)0x04),
  POWERSTEP01_ACC         = ((uint8_t)0x05),
  POWERSTEP01_DEC         = ((uint8_t)0x06),
  POWERSTEP01_MAX_SPEED   = ((uint8_t)0x07),
  POWERSTEP01_MIN_SPEED   = ((uint8_t)0x08),
  POWERSTEP01_FS_SPD      = ((uint8_t)0x15),
  POWERSTEP01_KVAL_HOLD   = ((uint8_t)0x09),
  POWERSTEP01_KVAL_RUN    = ((uint8_t)0x0A),
  POWERSTEP01_KVAL_ACC    = ((uint8_t)0x0B),
  POWERSTEP01_KVAL_DEC    = ((uint8_t)0x0C),
  POWERSTEP01_INT_SPD     = ((uint8_t)0x0D),
  POWERSTEP01_ST_SLP      = ((uint8_t)0x0E),
  POWERSTEP01_FN_SLP_ACC  = ((uint8_t)0x0F),
  POWERSTEP01_FN_SLP_DEC  = ((uint8_t)0x10),
  POWERSTEP01_K_THERM     = ((uint8_t)0x11),
  POWERSTEP01_ADC_OUT     = ((uint8_t)0x12),
  POWERSTEP01_OCD_TH      = ((uint8_t)0x13),
  POWERSTEP01_STALL_TH    = ((uint8_t)0x14),
  POWERSTEP01_STEP_MODE   = ((uint8_t)0x16),
  POWERSTEP01_ALARM_EN    = ((uint8_t)0x17),
  POWERSTEP01_GATECFG1    = ((uint8_t)0x18),
  POWERSTEP01_GATECFG2    = ((uint8_t)0x19),
  POWERSTEP01_CONFIG      = ((uint8_t)0x1A),
  POWERSTEP01_STATUS      = ((uint8_t)0x1B)
} powerstep01_Registers_t;

/// Powerstep01 address of register TVAL_HOLD (Current mode only)
#define   POWERSTEP01_TVAL_HOLD  (POWERSTEP01_KVAL_HOLD )   
/// Powerstep01 address of register TVAL_RUN (Current mode only)
#define   POWERSTEP01_TVAL_RUN   (POWERSTEP01_KVAL_RUN)   
/// Powerstep01 address of register TVAL_HOLD (Current mode only)
#define   POWERSTEP01_TVAL_ACC   (POWERSTEP01_KVAL_ACC)   
/// Powerstep01 address of register TVAL_DEC (Current mode only)
#define   POWERSTEP01_TVAL_DEC   (POWERSTEP01_KVAL_DEC)   
/// Powerstep01 address of register T_FAST  (Current mode only)
#define   POWERSTEP01_T_FAST     (POWERSTEP01_ST_SLP)   
/// Powerstep01 address of register TON_MIN  (Current mode only)
#define   POWERSTEP01_TON_MIN    (POWERSTEP01_FN_SLP_ACC)
/// Powerstep01 address of register TOFF_MIN (Current mode only)
#define   POWERSTEP01_TOFF_MIN   (POWERSTEP01_FN_SLP_DEC)

#endif