#ifndef ST_StepperDriver_def_h
#define ST_StepperDriver_def_h

typedef enum {
  ST_DRIVER_NOP               = ((uint8_t)0x00),
  ST_DRIVER_SET_PARAM         = ((uint8_t)0x00),
  ST_DRIVER_GET_PARAM         = ((uint8_t)0x20),
  ST_DRIVER_RUN               = ((uint8_t)0x50),
  ST_DRIVER_STEP_CLOCK        = ((uint8_t)0x58),
  ST_DRIVER_MOVE              = ((uint8_t)0x40),
  ST_DRIVER_GO_TO             = ((uint8_t)0x60),
  ST_DRIVER_GO_TO_DIR         = ((uint8_t)0x68),
  ST_DRIVER_GO_UNTIL          = ((uint8_t)0x82),
  ST_DRIVER_GO_UNTIL_ACT_CPY  = ((uint8_t)0x8A),
  ST_DRIVER_RELEASE_SW        = ((uint8_t)0x92),
  ST_DRIVER_GO_HOME           = ((uint8_t)0x70),
  ST_DRIVER_GO_MARK           = ((uint8_t)0x78),
  ST_DRIVER_RESET_POS         = ((uint8_t)0xD8),
  ST_DRIVER_RESET_DEVICE      = ((uint8_t)0xC0),
  ST_DRIVER_SOFT_STOP         = ((uint8_t)0xB0),
  ST_DRIVER_HARD_STOP         = ((uint8_t)0xB8),
  ST_DRIVER_SOFT_HIZ          = ((uint8_t)0xA0),
  ST_DRIVER_HARD_HIZ          = ((uint8_t)0xA8),
  ST_DRIVER_GET_STATUS        = ((uint8_t)0xD0),
  ST_DRIVER_RESERVED_CMD1     = ((uint8_t)0xEB),
  ST_DRIVER_RESERVED_CMD2     = ((uint8_t)0xF8)
} st_StepperDriver_Commands_t;


typedef enum {
  BWD = 0, /* Backward. */
  FWD = 1  /* Forward. */
} direction_t;

typedef enum {
  ACTION_RESET = ((uint8_t)0x00),
  ACTION_COPY  = ((uint8_t)0x08)
} motorAction_t;


typedef enum {
  STEP_MODE_FULL   = ((uint8_t)0x00), 
  STEP_MODE_HALF   = ((uint8_t)0x01),
  STEP_MODE_1_4    = ((uint8_t)0x02),
  STEP_MODE_1_8    = ((uint8_t)0x03),
  STEP_MODE_1_16   = ((uint8_t)0x04),
  STEP_MODE_1_32   = ((uint8_t)0x05),
  STEP_MODE_1_64   = ((uint8_t)0x06),
  STEP_MODE_1_128  = ((uint8_t)0x07),
  STEP_MODE_1_256  = ((uint8_t)0x08),
  STEP_MODE_UNKNOW = ((uint8_t)0xFE),
  STEP_MODE_WAVE   = ((uint8_t)0xFF)  
} step_mode_t;


/// masks for ABS_POS register of ST_DRIVER
typedef enum {
  ST_DRIVER_ABS_POS_VALUE_MASK        = ((uint32_t)0x003FFFFF),
  ST_DRIVER_ABS_POS_SIGN_BIT_MASK     = ((uint32_t)0x00200000)
} st_StepperDriver_AbsPosMasks_t;


#define CMD_ARG_MAX_NB_BYTES              (4)
///  command + argument bytes number for NOP command
#define CMD_ARG_NB_BYTES_NOP              (1)
///  command + argument bytes number for RUN command
#define CMD_ARG_NB_BYTES_RUN              (4)
///  command + argument bytes number for STEP_CLOCK command
#define CMD_ARG_NB_BYTES_STEP_CLOCK       (1)
///  command + argument bytes number for MOVE command
#define CMD_ARG_NB_BYTES_MOVE             (4)
///  command + argument bytes number for GO_TO command
#define CMD_ARG_NB_BYTES_GO_TO            (4)
///  command + argument bytes number for GO_TO_DIR command
#define CMD_ARG_NB_BYTES_GO_TO_DIR        (4)
///  command + argument bytes number for GO_UNTIL command
#define CMD_ARG_NB_BYTES_GO_UNTIL         (4)
///  command + argument bytes number for RELEASE_SW command
#define CMD_ARG_NB_BYTES_RELEASE_SW       (1)
///  command + argument bytes number for GO_HOME command
#define CMD_ARG_NB_BYTES_GO_HOME          (1)
///  command + argument bytes number for GO_MARK command
#define CMD_ARG_NB_BYTES_GO_MARK          (1)
///  command + argument bytes number for RESET_POS command
#define CMD_ARG_NB_BYTES_RESET_POS        (1)
///  command + argument bytes number for RESET_DEVICE command
#define CMD_ARG_NB_BYTES_RESET_DEVICE     (1)
///  command + argument bytes number for NOP command
#define CMD_ARG_NB_BYTES_SOFT_STOP        (1)
///  command + argument bytes number for HARD_STOP command
#define CMD_ARG_NB_BYTES_HARD_STOP        (1)
///  command + argument bytes number for SOFT_HIZ command
#define CMD_ARG_NB_BYTES_SOFT_HIZ         (1)
///  command + argument bytes number for ARD_HIZ command
#define CMD_ARG_NB_BYTES_HARD_HIZ         (1)
///  command + argument bytes number for GET_STATUS command
#define CMD_ARG_NB_BYTES_GET_STATUS       (1)
///  response bytes number 
#define RSP_NB_BYTES_GET_STATUS           (2) 

#define MAX_NUMBER_OF_DEVICES           (1)
#define DAISY_CHAIN_COMMAND_MASK        (0xFA)

#endif