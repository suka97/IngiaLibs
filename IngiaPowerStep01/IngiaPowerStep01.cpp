#include "IngiaPowerStep01.h"


//  ---------------------- PowerStep01.ino -------------------------------------------

status_t IngiaPowerStep01::Init(void* pInit)
{

    //  /* configure the step clock */
    //  Board_StepClockInit();

    //  /* Standby-reset deactivation */
    //  Board_ReleaseReset();

  /* Let a delay after reset */
  //digitalWrite(reset, LOW); digitalWrite(reset, HIGH);  // RESET !!!!!!!!

  Board_Delay(1);

  if (pInit == 0)
  {
    // Set all registers to their predefined values
    // from powerstep01_target_config.h
    SetRegisterToPredefinedValues(); 
  }
  else
  {
    SetDeviceParamsToGivenValues((powerstep01_init_u_t*) pInit);
  }

  // Put the Powerstep01 in HiZ state
  CmdHardHiZ();

  FetchAndClearAllStatus();

  return COMPONENT_OK;
}

void IngiaPowerStep01::SetDeviceParamsToGivenValues (powerstep01_init_u_t *initPrm) 
{
  CmdSetParam(POWERSTEP01_ABS_POS, 0);
  CmdSetParam(POWERSTEP01_EL_POS, 0);
  CmdSetParam(POWERSTEP01_MARK, 0);
  CmdSetParam(POWERSTEP01_ACC,
    acc_dec_steps_s2_to_reg_val(initPrm->cm.cp.acceleration));
  CmdSetParam(POWERSTEP01_DEC,
    acc_dec_steps_s2_to_reg_val(initPrm->cm.cp.deceleration));
  CmdSetParam(POWERSTEP01_MAX_SPEED,
    max_spd_steps_s_to_reg_val(initPrm->cm.cp.maxSpeed));
  CmdSetParam(POWERSTEP01_MIN_SPEED,
    initPrm->cm.cp.lowSpeedOptimization|
    max_spd_steps_s_to_reg_val(initPrm->cm.cp.minSpeed));
  CmdSetParam(POWERSTEP01_FS_SPD,
    initPrm->cm.cp.boostMode|
    fs_spd_steps_s_to_reg_val(initPrm->cm.cp.fullStepSpeed));
  CmdSetParam(POWERSTEP01_OCD_TH,
    stall_ocd_th_to_reg_val(initPrm->cm.cp.ocdThreshold));
  CmdSetParam(POWERSTEP01_STEP_MODE,
    (uint8_t)initPrm->cm.cp.syncClockSelection|
    (uint8_t)initPrm->cm.cp.cmVmSelection|
    (uint8_t)(uint8_t)initPrm->cm.cp.stepMode);
  CmdSetParam(POWERSTEP01_ALARM_EN,
    initPrm->cm.cp.alarmsSelection);
  CmdSetParam(POWERSTEP01_GATECFG1,
    (uint16_t)initPrm->cm.cp.iGate|
    (uint16_t)initPrm->cm.cp.tcc|
    (uint16_t)initPrm->cm.cp.tBoost|
    (uint16_t)initPrm->cm.cp.wdEn);
  CmdSetParam(POWERSTEP01_GATECFG2,
    (uint16_t)initPrm->cm.cp.tBlank|
    (uint16_t)initPrm->cm.cp.tdt);
  if (initPrm->cm.cp.cmVmSelection == POWERSTEP01_CM_VM_VOLTAGE)
  {
    //Voltage mode
    CmdSetParam(POWERSTEP01_INT_SPD,
      int_spd_steps_s_to_reg_val(
        initPrm->vm.intersectSpeed));
    CmdSetParam(POWERSTEP01_K_THERM,
      k_therm_comp_to_reg_val(
        initPrm->vm.thermalCompensationFactor));
    CmdSetParam(POWERSTEP01_STALL_TH,
      stall_ocd_th_to_reg_val(
        initPrm->vm.stallThreshold));
    CmdSetParam(POWERSTEP01_KVAL_HOLD,
      k_val_perc_to_reg_val(
        initPrm->vm.kvalHold));
    CmdSetParam(POWERSTEP01_KVAL_RUN,
      k_val_perc_to_reg_val(
        initPrm->vm.kvalRun));
    CmdSetParam(POWERSTEP01_KVAL_ACC,
      k_val_perc_to_reg_val(
        initPrm->vm.kvalAcc));
    CmdSetParam(POWERSTEP01_KVAL_DEC,
      k_val_perc_to_reg_val(
        initPrm->vm.kvalDec));
    CmdSetParam(POWERSTEP01_ST_SLP,
      bemf_slope_perc_to_reg_val(
        initPrm->vm.startSlope));
    CmdSetParam(POWERSTEP01_FN_SLP_ACC,
      bemf_slope_perc_to_reg_val(
        initPrm->vm.accelerationFinalSlope));
    CmdSetParam(POWERSTEP01_FN_SLP_DEC,
      bemf_slope_perc_to_reg_val(
        initPrm->vm.decelerationFinalSlope));
    CmdSetParam(POWERSTEP01_CONFIG,
      (uint16_t)initPrm->vm.oscClkSel|
      (uint16_t)initPrm->vm.swMode |
      (uint16_t)initPrm->vm.enVsComp|
      (uint16_t)initPrm->vm.ocSd|
      (uint16_t)initPrm->vm.uvloVal|
      (uint16_t)initPrm->vm.vccVal|
      (uint16_t)initPrm->vm.fPwmInt|
      (uint16_t)initPrm->vm.fPwmDec);
  }
  else
  {
    // Current mode
        CmdSetParam(POWERSTEP01_TVAL_HOLD,
          t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalHold));
        CmdSetParam(POWERSTEP01_TVAL_RUN,
          t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalRun));
        CmdSetParam(POWERSTEP01_TVAL_ACC,
          t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalAcc));
        CmdSetParam(POWERSTEP01_TVAL_DEC,
          t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalDec));
        CmdSetParam(POWERSTEP01_T_FAST,
          (uint8_t)initPrm->cm.toffFast|
          (uint8_t)initPrm->cm.fastStep);
        CmdSetParam(POWERSTEP01_TON_MIN,
          t_min_time_to_reg_val(
            initPrm->cm.tonMin));
        CmdSetParam(POWERSTEP01_TOFF_MIN,
          t_min_time_to_reg_val(
            initPrm->cm.toffMin));
        CmdSetParam(POWERSTEP01_CONFIG,
          (uint16_t)initPrm->cm.oscClkSel|
          (uint16_t)initPrm->cm.swMode|
          (uint16_t)initPrm->cm.tqReg|
          (uint16_t)initPrm->cm.ocSd|
          (uint16_t)initPrm->cm.uvloVal|
          (uint16_t)initPrm->cm.vccVal|
          (uint16_t)initPrm->cm.tsw|
          (uint16_t)initPrm->cm.predEn);
  }
}

void IngiaPowerStep01::Board_Delay(uint32_t mdelay)
{
    delay(mdelay);
}

void IngiaPowerStep01::SetRegisterToPredefinedValues(void)
{
    powerstep01_CmVm_t cmVm;

    CmdSetParam(
      POWERSTEP01_ABS_POS,
      0);
    CmdSetParam(
      POWERSTEP01_EL_POS,
      0);
    CmdSetParam(
      POWERSTEP01_MARK,
      0);

        cmVm = POWERSTEP01_CONF_PARAM_CM_VM_DEVICE_0;
        CmdSetParam(POWERSTEP01_ACC,
          acc_dec_steps_s2_to_reg_val(
            POWERSTEP01_CONF_PARAM_ACC_DEVICE_0));
        CmdSetParam(POWERSTEP01_DEC,
          acc_dec_steps_s2_to_reg_val(
            POWERSTEP01_CONF_PARAM_DEC_DEVICE_0));
        CmdSetParam(POWERSTEP01_MAX_SPEED,
          max_spd_steps_s_to_reg_val(
            POWERSTEP01_CONF_PARAM_MAX_SPEED_DEVICE_0));
        CmdSetParam(POWERSTEP01_MIN_SPEED,
          POWERSTEP01_CONF_PARAM_LSPD_BIT_DEVICE_0|
          min_spd_steps_s_to_reg_val(
            POWERSTEP01_CONF_PARAM_MIN_SPEED_DEVICE_0));
        CmdSetParam(POWERSTEP01_FS_SPD,
          POWERSTEP01_CONF_PARAM_BOOST_MODE_DEVICE_0|
          fs_spd_steps_s_to_reg_val(
            POWERSTEP01_CONF_PARAM_FS_SPD_DEVICE_0));
        CmdSetParam(POWERSTEP01_OCD_TH,
          (uint8_t)POWERSTEP01_CONF_PARAM_OCD_TH_DEVICE_0);
        CmdSetParam(POWERSTEP01_STEP_MODE,
          (uint8_t)POWERSTEP01_CONF_PARAM_SYNC_MODE_DEVICE_0 |
          (uint8_t)POWERSTEP01_CONF_PARAM_CM_VM_DEVICE_0|
          (uint8_t)POWERSTEP01_CONF_PARAM_STEP_MODE_DEVICE_0);
        CmdSetParam(POWERSTEP01_ALARM_EN,
          POWERSTEP01_CONF_PARAM_ALARM_EN_DEVICE_0);
        CmdSetParam(POWERSTEP01_GATECFG1,
          (uint16_t)POWERSTEP01_CONF_PARAM_IGATE_DEVICE_0 |
          (uint16_t)POWERSTEP01_CONF_PARAM_TCC_DEVICE_0   |
          (uint16_t)POWERSTEP01_CONF_PARAM_TBOOST_DEVICE_0|
          (uint16_t)POWERSTEP01_CONF_PARAM_WD_EN_DEVICE_0);
        CmdSetParam(POWERSTEP01_GATECFG2,
          (uint16_t)POWERSTEP01_CONF_PARAM_TBLANK_DEVICE_0 |
          (uint16_t)POWERSTEP01_CONF_PARAM_TDT_DEVICE_0);
        // Voltage mode
        if (cmVm == POWERSTEP01_CM_VM_VOLTAGE)
        {
          CmdSetParam(POWERSTEP01_INT_SPD,
            int_spd_steps_s_to_reg_val(
              POWERSTEP01_CONF_PARAM_INT_SPD_DEVICE_0));
          CmdSetParam(POWERSTEP01_K_THERM,
            k_therm_comp_to_reg_val(
              POWERSTEP01_CONF_PARAM_K_THERM_DEVICE_0));
          CmdSetParam(POWERSTEP01_STALL_TH,
            stall_ocd_th_to_reg_val(
              POWERSTEP01_CONF_PARAM_STALL_TH_DEVICE_0));
          CmdSetParam(POWERSTEP01_KVAL_HOLD,
            k_val_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_KVAL_HOLD_DEVICE_0));
          CmdSetParam(POWERSTEP01_KVAL_RUN,
            k_val_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_KVAL_RUN_DEVICE_0));
          CmdSetParam(POWERSTEP01_KVAL_ACC,
            k_val_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_KVAL_ACC_DEVICE_0));
          CmdSetParam(POWERSTEP01_KVAL_DEC,
            k_val_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_KVAL_DEC_DEVICE_0));
          CmdSetParam(POWERSTEP01_ST_SLP,
            bemf_slope_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_ST_SLP_DEVICE_0));
          CmdSetParam(POWERSTEP01_FN_SLP_ACC,
            bemf_slope_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_FN_SLP_ACC_DEVICE_0));
          CmdSetParam(POWERSTEP01_FN_SLP_DEC,
            bemf_slope_perc_to_reg_val(
              POWERSTEP01_CONF_PARAM_FN_SLP_DEC_DEVICE_0));
          CmdSetParam(POWERSTEP01_CONFIG,
            (uint16_t)POWERSTEP01_CONF_PARAM_CLOCK_SETTING_DEVICE_0 |
            (uint16_t)POWERSTEP01_CONF_PARAM_SW_MODE_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_VS_COMP_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_OC_SD_DEVICE_0         |
            (uint16_t)POWERSTEP01_CONF_PARAM_UVLOVAL_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_VCCVAL_DEVICE_0        |
            (uint16_t)POWERSTEP01_CONF_PARAM_PWM_DIV_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_PWM_MUL_DEVICE_0);
        }
        else
        {
          // Current mode
          CmdSetParam(POWERSTEP01_TVAL_HOLD,
            t_val_ref_voltage_to_reg_val(
              POWERSTEP01_CONF_PARAM_TVAL_HOLD_DEVICE_0));
          CmdSetParam(POWERSTEP01_TVAL_RUN,
            t_val_ref_voltage_to_reg_val(
              POWERSTEP01_CONF_PARAM_TVAL_RUN_DEVICE_0));
          CmdSetParam(POWERSTEP01_TVAL_ACC,
            t_val_ref_voltage_to_reg_val(
              POWERSTEP01_CONF_PARAM_TVAL_ACC_DEVICE_0));
          CmdSetParam(POWERSTEP01_TVAL_DEC,
            t_val_ref_voltage_to_reg_val(
              POWERSTEP01_CONF_PARAM_TVAL_DEC_DEVICE_0));
          CmdSetParam(POWERSTEP01_T_FAST,
            (uint8_t)POWERSTEP01_CONF_PARAM_TOFF_FAST_DEVICE_0 |
            (uint8_t)POWERSTEP01_CONF_PARAM_FAST_STEP_DEVICE_0);
          CmdSetParam(POWERSTEP01_TON_MIN,
            t_min_time_to_reg_val(
              POWERSTEP01_CONF_PARAM_TON_MIN_DEVICE_0));
          CmdSetParam(POWERSTEP01_TOFF_MIN,
            t_min_time_to_reg_val(
              POWERSTEP01_CONF_PARAM_TOFF_MIN_DEVICE_0));
          CmdSetParam(POWERSTEP01_CONFIG,
            (uint16_t)POWERSTEP01_CONF_PARAM_CLOCK_SETTING_DEVICE_0 |
            (uint16_t)POWERSTEP01_CONF_PARAM_SW_MODE_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_TQ_REG_DEVICE_0        |
            (uint16_t)POWERSTEP01_CONF_PARAM_OC_SD_DEVICE_0         |
            (uint16_t)POWERSTEP01_CONF_PARAM_UVLOVAL_DEVICE_0       |
            (uint16_t)POWERSTEP01_CONF_PARAM_VCCVAL_DEVICE_0        |
            (uint16_t)POWERSTEP01_CONF_PARAM_TSW_DEVICE_0           |
            (uint16_t)POWERSTEP01_CONF_PARAM_PRED_DEVICE_0);
        }
}

void IngiaPowerStep01::CmdSetParam(uint8_t param, uint32_t value)
{
  uint32_t loop;
  uint8_t maxArgumentNbBytes = 0;
  uint8_t spiIndex = numberOfDevices - deviceInstance - 1;
  bool itDisable = FALSE;

  do
  {
    spiPreemptionByIsr = FALSE;
    if (itDisable)
    {
      /* re-enable Board_EnableIrq if disable in previous iteration */
    //      Board_EnableIrq();
      itDisable = FALSE;
    }
    for (loop = 0;loop < numberOfDevices; loop++)
    {
      spiTxBursts[0][loop] = POWERSTEP01_NOP;
      spiTxBursts[1][loop] = POWERSTEP01_NOP;
      spiTxBursts[2][loop] = POWERSTEP01_NOP;
      spiTxBursts[3][loop] = POWERSTEP01_NOP;
    } 
    switch (param)
    {
      case POWERSTEP01_ABS_POS: ;
      case POWERSTEP01_MARK:
        spiTxBursts[0][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param); 
        spiTxBursts[1][spiIndex] = (uint8_t)(value >> 16);
        spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
        maxArgumentNbBytes = 3;
        break;
      case POWERSTEP01_EL_POS:
      case POWERSTEP01_ACC:
      case POWERSTEP01_DEC:
      case POWERSTEP01_MAX_SPEED:
      case POWERSTEP01_MIN_SPEED:
      case POWERSTEP01_FS_SPD:
      case POWERSTEP01_INT_SPD:
      case POWERSTEP01_CONFIG:
      case POWERSTEP01_GATECFG1:
        spiTxBursts[1][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param);
        spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
        maxArgumentNbBytes = 2;
        break;
      default:
        spiTxBursts[2][spiIndex] = ((uint8_t)POWERSTEP01_SET_PARAM )| (param);
        maxArgumentNbBytes = 1;
    }
    spiTxBursts[3][spiIndex] = (uint8_t)(value);
    /* Disable interruption before checking */
    /* pre-emption by ISR and SPI transfers*/
    //Board_DisableIrq();   ////////////////////////////////////
    itDisable = TRUE;
  } while (spiPreemptionByIsr); // check pre-emption by ISR
  /* SPI transfer */
  for (loop = POWERSTEP01_CMD_ARG_MAX_NB_BYTES - 1 - maxArgumentNbBytes;
       loop < POWERSTEP01_CMD_ARG_MAX_NB_BYTES;
       loop++)
  {
    WriteBytes(&spiTxBursts[loop][0],&spiRxBursts[loop][0]); 
  }
  /* re-enable Board_EnableIrq after SPI transfers*/
  //Board_EnableIrq();  /////////////////////////////////////
}

void IngiaPowerStep01::WriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte)
{
  if (Board_SpiWriteBytes(pByteToTransmit, pReceivedByte) != 0)
  {
    //ErrorHandler(POWERSTEP01_ERROR_1);
  }
  if (isrFlag)
  {
    spiPreemptionByIsr = TRUE;
  }
}

uint8_t IngiaPowerStep01::Board_SpiWriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte)
{
    return (uint8_t) (ReadWrite(pReceivedByte, pByteToTransmit, numberOfDevices) == COMPONENT_OK ? 0 : 1);
}

status_t IngiaPowerStep01::ReadWrite(uint8_t* pBufferToRead, uint8_t* pBufferToWrite, uint16_t NumBytes)
{   
    if (spi_read_write(pBufferToRead, pBufferToWrite, ssel, 1) != 0) {
        return COMPONENT_ERROR;
    }
    return COMPONENT_OK;
}



//  ---------------------- SendCommand.ino -------------------------------------------

void IngiaPowerStep01::FetchAndClearAllStatus(void)
{
  uint8_t loop;

  for (loop = 0; loop < numberOfDevices; loop++)
  {
     spiTxBursts[0][loop] = POWERSTEP01_GET_STATUS;
     spiTxBursts[1][loop] = POWERSTEP01_NOP;
     spiTxBursts[2][loop] = POWERSTEP01_NOP;
     spiTxBursts[3][loop] = POWERSTEP01_NOP;
     spiRxBursts[0][loop] = 0;
     spiRxBursts[1][loop] = 0;
     spiRxBursts[2][loop] = 0;
     spiRxBursts[3][loop] = 0;
  }
  for (loop = 0;
       loop < POWERSTEP01_CMD_ARG_NB_BYTES_GET_STATUS +
              POWERSTEP01_RSP_NB_BYTES_GET_STATUS;
       loop++)
  {
     WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
  }
}


void IngiaPowerStep01::SendCommand(uint8_t command, uint32_t value)
{
  uint32_t loop;
  uint8_t maxArgumentNbBytes = 0;
  bool itDisable = FALSE;
  uint8_t spiIndex = numberOfDevices - deviceInstance - 1;

  do
  {
    spiPreemptionByIsr = FALSE;
    if (itDisable)
    {
      /* re-enable Board_EnableIrq if disable in previous iteration */
    //      Board_EnableIrq();
      itDisable = FALSE;
    }
    for (loop = 0; loop < numberOfDevices; loop++)
    {
        spiTxBursts[0][loop] = POWERSTEP01_NOP;
        spiTxBursts[1][loop] = POWERSTEP01_NOP;
        spiTxBursts[2][loop] = POWERSTEP01_NOP;
        spiTxBursts[3][loop] = POWERSTEP01_NOP;
    }
    switch (command & DAISY_CHAIN_COMMAND_MASK)
    {
      case POWERSTEP01_GO_TO:
      case POWERSTEP01_GO_TO_DIR:
        value = value & POWERSTEP01_ABS_POS_VALUE_MASK;
      case POWERSTEP01_RUN:
      case POWERSTEP01_MOVE:
      case POWERSTEP01_GO_UNTIL:
      case POWERSTEP01_GO_UNTIL_ACT_CPY:
        spiTxBursts[0][spiIndex] = command;
        spiTxBursts[1][spiIndex] = (uint8_t)(value >> 16);
        spiTxBursts[2][spiIndex] = (uint8_t)(value >> 8);
        spiTxBursts[3][spiIndex] = (uint8_t)(value);
        maxArgumentNbBytes = 3;
        break;
    default:
        spiTxBursts[0][spiIndex] = POWERSTEP01_NOP;
        spiTxBursts[1][spiIndex] = POWERSTEP01_NOP;
        spiTxBursts[2][spiIndex] = POWERSTEP01_NOP;
        spiTxBursts[3][spiIndex] = command;
    }
    /* Disable interruption before checking */
    /* pre-emption by ISR and SPI transfers*/
    //    Board_DisableIrq();
    itDisable = TRUE;
  } while (spiPreemptionByIsr); // check pre-emption by ISR
  for (loop = POWERSTEP01_CMD_ARG_MAX_NB_BYTES - 1 - maxArgumentNbBytes;
       loop < POWERSTEP01_CMD_ARG_MAX_NB_BYTES;
       loop++)
  {
     WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
  }
  /* re-enable Board_EnableIrq after SPI transfers*/
    //  Board_EnableIrq();
}


uint32_t IngiaPowerStep01::CmdGetParam(powerstep01_Registers_t param)
{

  uint32_t spiRxData;
  uint32_t loop;
  uint8_t maxArgumentNbBytes = 0;
  uint8_t spiIndex = numberOfDevices - deviceInstance - 1;
  bool itDisable = FALSE;
  
  do
  {
    spiPreemptionByIsr = FALSE;
    if (itDisable)
    {
      /* re-enable Board_EnableIrq if disable in previous iteration */
      //Board_EnableIrq();
      itDisable = FALSE;
    }
    for (loop = 0; loop < numberOfDevices; loop++)
    {
      spiTxBursts[0][loop] = POWERSTEP01_NOP;
      spiTxBursts[1][loop] = POWERSTEP01_NOP;
      spiTxBursts[2][loop] = POWERSTEP01_NOP;
      spiTxBursts[3][loop] = POWERSTEP01_NOP;
      spiRxBursts[0][loop] = 0;
      spiRxBursts[1][loop] = 0;
      spiRxBursts[2][loop] = 0;
      spiRxBursts[3][loop] = 0;    
    }
    switch (param)
    {
      case POWERSTEP01_ABS_POS: 
      case POWERSTEP01_MARK:
      case POWERSTEP01_SPEED:
        spiTxBursts[0][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
        maxArgumentNbBytes = 3;
        break;
      case POWERSTEP01_EL_POS:
      case POWERSTEP01_ACC:
      case POWERSTEP01_DEC:
      case POWERSTEP01_MAX_SPEED:
      case POWERSTEP01_MIN_SPEED:
      case POWERSTEP01_FS_SPD:
      case POWERSTEP01_INT_SPD:
      case POWERSTEP01_CONFIG:
      case POWERSTEP01_GATECFG1:
      case POWERSTEP01_STATUS:
        spiTxBursts[1][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
        maxArgumentNbBytes = 2;
        break;
    default:
        spiTxBursts[2][spiIndex] = ((uint8_t)POWERSTEP01_GET_PARAM )| (param);
        maxArgumentNbBytes = 1;
    }
    /* Disable interruption before checking */
    /* pre-emption by ISR and SPI transfers*/
    //Board_DisableIrq();
    itDisable = TRUE;
  } while (spiPreemptionByIsr); // check pre-emption by ISR
  for (loop = POWERSTEP01_CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes;
       loop < POWERSTEP01_CMD_ARG_MAX_NB_BYTES;
       loop++)
  {
     WriteBytes(&spiTxBursts[loop][0],
                           &spiRxBursts[loop][0]);
  }
  spiRxData = ((uint32_t)spiRxBursts[1][spiIndex] << 16)|
               (spiRxBursts[2][spiIndex] << 8) |
               (spiRxBursts[3][spiIndex]);    
  /* re-enable Board_EnableIrq after SPI transfers*/
  //Board_EnableIrq();
  return (spiRxData);
}


int32_t IngiaPowerStep01::ConvertPosition(uint32_t abs_position_reg)
{
  int32_t operation_result;

  if (abs_position_reg & POWERSTEP01_ABS_POS_SIGN_BIT_MASK) 
  {
    /* Negative register value */
    abs_position_reg = ~abs_position_reg;
    abs_position_reg += 1;

    operation_result = (int32_t) (abs_position_reg & POWERSTEP01_ABS_POS_VALUE_MASK);
    operation_result = -operation_result;
  } 
  else 
  {
    operation_result = (int32_t) abs_position_reg;
  }
  return operation_result;
}


bool IngiaPowerStep01::SetAnalogValue(powerstep01_Registers_t param, float value)
{
  uint32_t registerValue;
  bool result = TRUE;
  bool voltageMode = ((POWERSTEP01_CM_VM_CURRENT&CmdGetParam(POWERSTEP01_STEP_MODE))==0);
  if ((value < 0)&&((param != POWERSTEP01_ABS_POS)&&(param != POWERSTEP01_MARK)))
  {
    result = FALSE;
  }
  switch (param)
  {
    case POWERSTEP01_EL_POS:
      if ((value > (POWERSTEP01_ELPOS_STEP_MASK|POWERSTEP01_ELPOS_MICROSTEP_MASK))||
          ((value!=0)&&(value < (1<<(7-(POWERSTEP01_STEP_MODE_STEP_SEL&CmdGetParam(POWERSTEP01_STEP_MODE))))))) {
          result = FALSE;
      } else {
          registerValue = ((uint32_t) value)&(POWERSTEP01_ELPOS_STEP_MASK|POWERSTEP01_ELPOS_MICROSTEP_MASK);
      }
      break;        
    case POWERSTEP01_ABS_POS:
    case POWERSTEP01_MARK:
      if (value < 0)
      {
        value=-value;
        if (((uint32_t)value)<=(POWERSTEP01_MAX_POSITION+1)) {
            registerValue = (POWERSTEP01_ABS_POS_VALUE_MASK+1-(uint32_t)value)&POWERSTEP01_ABS_POS_VALUE_MASK;
        } else {
            result = FALSE;  
        }
      }
      else
      {
        if (((uint32_t)value)<=POWERSTEP01_MAX_POSITION) {
            registerValue = ((uint32_t) value)&POWERSTEP01_ABS_POS_VALUE_MASK;
        } else {
            result = FALSE; 
        }
      }
      break;
    case POWERSTEP01_ACC:
    case POWERSTEP01_DEC:
      if (value > POWERSTEP01_ACC_DEC_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = acc_dec_steps_s2_to_reg_val(value);
      }
      break;
    case POWERSTEP01_MAX_SPEED:
      if (value > POWERSTEP01_MAX_SPEED_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = max_spd_steps_s_to_reg_val(value);
      }
      break;
    case POWERSTEP01_MIN_SPEED:
      if (value > POWERSTEP01_MIN_SPEED_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = (POWERSTEP01_LSPD_OPT&CmdGetParam(param))|min_spd_steps_s_to_reg_val(value);
      }
      break;      
    case POWERSTEP01_FS_SPD:
      if (value > POWERSTEP01_FS_SPD_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = (POWERSTEP01_BOOST_MODE&CmdGetParam(param))|fs_spd_steps_s_to_reg_val(value);
      }
      break;
    case POWERSTEP01_INT_SPD:
      if (value > POWERSTEP01_INT_SPD_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = int_spd_steps_s_to_reg_val(value);
      }
      break;
    case POWERSTEP01_K_THERM:
      if ((value < POWERSTEP01_K_THERM_MIN_VALUE)||
          (value > POWERSTEP01_K_THERM_MAX_VALUE)) {
          result = FALSE;
      } else {
          registerValue = k_therm_comp_to_reg_val(value);
      }
      break;
    case POWERSTEP01_OCD_TH:
    case POWERSTEP01_STALL_TH:
      if (value > POWERSTEP01_STALL_OCD_TH_MAX_VALUE) {
          result = FALSE;
      } else {
          registerValue = stall_ocd_th_to_reg_val(value);
      }
      break;
    case POWERSTEP01_KVAL_HOLD:  //POWERSTEP01_TVAL_HOLD
    case POWERSTEP01_KVAL_RUN:   //POWERSTEP01_TVAL_RUN
    case POWERSTEP01_KVAL_ACC:   //POWERSTEP01_TVAL_ACC
    case POWERSTEP01_KVAL_DEC:   //POWERSTEP01_TVAL_DEC
      if (voltageMode==FALSE)
      {
        if (value > POWERSTEP01_TVAL_MAX_VALUE) {
            result = FALSE;
        } else {
            registerValue = t_val_ref_voltage_to_reg_val(value);
        }
      }
      else
      {
        if (value > POWERSTEP01_KVAL_MAX_VALUE) {
            result = FALSE;
        } else {
            registerValue = k_val_perc_to_reg_val(value);
        }
      }
      break;
    case POWERSTEP01_ST_SLP:
      if (voltageMode==FALSE) 
      {
        result = FALSE;
        break;
      }
    case POWERSTEP01_FN_SLP_ACC: //POWERSTEP01_TON_MIN
    case POWERSTEP01_FN_SLP_DEC: //POWERSTEP01_TOFF_MIN
      if (voltageMode==FALSE)
      {
        if (value>POWERSTEP01_TOFF_TON_MIN_MAX_VALUE) {
            result = FALSE;
        } else {
            registerValue = t_min_time_to_reg_val(value);
        }
      }
      else
      {
        if (value > POWERSTEP01_SLP_MAX_VALUE) {
            result = FALSE;
        } else {
            registerValue = bemf_slope_perc_to_reg_val(value);
        }
      }
      break;
    default:
      result = FALSE;
  }
  if (result!=FALSE)
  {
    CmdSetParam(param, registerValue);
  }
  return result;
}


uint16_t IngiaPowerStep01::CmdGetStatus(void)
{
  uint16_t status = 0;
  uint32_t loop;
  uint8_t spiIndex = numberOfDevices - deviceInstance - 1;
  bool itDisable = FALSE;  
 
  do
  {
    spiPreemptionByIsr = FALSE;
    if (itDisable)
    {
      /* re-enable Board_EnableIrq if disable in previous iteration */
      //Board_EnableIrq();
      itDisable = FALSE;
    }    
    for (loop = 0; loop < numberOfDevices; loop++)
    {
       spiTxBursts[0][loop] = POWERSTEP01_NOP;
       spiTxBursts[1][loop] = POWERSTEP01_NOP;
       spiTxBursts[2][loop] = POWERSTEP01_NOP;
       spiTxBursts[3][loop] = POWERSTEP01_NOP;
       spiRxBursts[0][loop] = 0;
       spiRxBursts[1][loop] = 0;
       spiRxBursts[2][loop] = 0;
       spiRxBursts[3][loop] = 0;       
    }
    spiTxBursts[0][spiIndex] = POWERSTEP01_GET_STATUS;
    /* Disable interruption before checking */
    /* pre-emption by ISR and SPI transfers*/
    //Board_DisableIrq();
    itDisable = TRUE;
  } while (spiPreemptionByIsr); // check pre-emption by ISR  
  for (loop = 0; loop < POWERSTEP01_CMD_ARG_NB_BYTES_GET_STATUS + POWERSTEP01_RSP_NB_BYTES_GET_STATUS; loop++)
  {
     WriteBytes(&spiTxBursts[loop][0], &spiRxBursts[loop][0]);
  }
  status = (spiRxBursts[1][spiIndex] << 8) | (spiRxBursts[2][spiIndex]);
  /* re-enable Board_EnableIrq after SPI transfers*/
  //Board_EnableIrq();  

  return (status);
}



//  ---------------------- SPI.ino -------------------------------------------

int IngiaPowerStep01::spi_read_write(uint8_t* pBufferToRead, uint8_t* pBufferToWrite, uint8_t ssel, uint16_t NumBytes)
{
    //    /* Check data format */
    //    if(_bits != 8) return -1;

    /* Select the chip. */
    digitalWrite(ssel, LOW);

    /* Read and write data at the same time. */
    pBufferToRead[0] = mi_write(pBufferToWrite[0]);

    /* Unselect the chip. */
    digitalWrite(ssel, HIGH);

    return 0;
}