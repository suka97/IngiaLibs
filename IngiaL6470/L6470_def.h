#ifndef L6470_def_h
#define L6470_def_h

// STATUS struct
struct l6470_Status_t {
    bool hiZ;
    bool busy;
    sw_Status_t sw_f;
    bool sw_evn;
    direction_t dir;
    motorStatus_t mot_status;
    bool notperf_cmd;
    bool wrong_cmd;
    bool uvlo;
    bool th_wrn;
    bool th_sd;
    bool ocd;
    bool step_loss_A;
    bool step_loss_B;
    bool sck_mod;
};

/// Bit mask for STATUS Register of PowerStep01²
typedef enum {
  L6470_STATUS_HIZ          = (((uint16_t)0x0001)),
  L6470_STATUS_BUSY         = (((uint16_t)0x0002)),
  L6470_STATUS_SW_F         = (((uint16_t)0x0004)),
  L6470_STATUS_SW_EVN       = (((uint16_t)0x0008)),
  L6470_STATUS_DIR          = (((uint16_t)0x0010)),
  L6470_STATUS_MOT_STATUS   = (((uint16_t)0x0060)),
  L6470_STATUS_NOTPERF_CMD  = (((uint16_t)0x0080)),
  L6470_STATUS_WRONG_CMD    = (((uint16_t)0x0100)),
  L6470_STATUS_UVLO         = (((uint16_t)0x0200)),
  L6470_STATUS_TH_WRN       = (((uint16_t)0x0400)),
  L6470_STATUS_TH_SD        = (((uint16_t)0x0800)),
  L6470_STATUS_OCD          = (((uint16_t)0x1000)),
  L6470_STATUS_STEP_LOSS_A  = (((uint16_t)0x2000)),
  L6470_STATUS_STEP_LOSS_B  = (((uint16_t)0x4000)),
  L6470_STATUS_SCK_MOD      = (((uint16_t)0x8000))
} l6470_StatusMasks_t;

/// Powerstep01 internal register addresses
typedef enum {
  L6470_ABS_POS     = ((uint8_t)0x01),
  L6470_EL_POS      = ((uint8_t)0x02),
  L6470_MARK        = ((uint8_t)0x03), 
  L6470_SPEED       = ((uint8_t)0x04),
  L6470_ACC         = ((uint8_t)0x05),
  L6470_DEC         = ((uint8_t)0x06),
  L6470_MAX_SPEED   = ((uint8_t)0x07),
  L6470_MIN_SPEED   = ((uint8_t)0x08),
  L6470_FS_SPD      = ((uint8_t)0x15),
  L6470_KVAL_HOLD   = ((uint8_t)0x09),
  L6470_KVAL_RUN    = ((uint8_t)0x0A),
  L6470_KVAL_ACC    = ((uint8_t)0x0B),
  L6470_KVAL_DEC    = ((uint8_t)0x0C),
  L6470_INT_SPD     = ((uint8_t)0x0D),
  L6470_ST_SLP      = ((uint8_t)0x0E),
  L6470_FN_SLP_ACC  = ((uint8_t)0x0F),
  L6470_FN_SLP_DEC  = ((uint8_t)0x10),
  L6470_K_THERM     = ((uint8_t)0x11),
  L6470_ADC_OUT     = ((uint8_t)0x12),
  L6470_OCD_TH      = ((uint8_t)0x13),
  L6470_STALL_TH    = ((uint8_t)0x14),
  L6470_STEP_MODE   = ((uint8_t)0x16),
  L6470_ALARM_EN    = ((uint8_t)0x17),
  L6470_CONFIG      = ((uint8_t)0x18),
  L6470_STATUS      = ((uint8_t)0x19),
  L6470_RESERVED1   = ((uint8_t)0x1A),
  L6470_RESERVED2   = ((uint8_t)0x1B)
} l6470_Registers_t;

#endif