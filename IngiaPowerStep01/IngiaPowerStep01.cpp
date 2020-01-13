#include "IngiaPowerStep01.h"

void IngiaPowerStep01::initDevice (void *deviceConfig) {
    if ( reset != 0 ) {
        digitalWrite(reset, LOW);
        delay(1);
        digitalWrite(reset, HIGH);
    }
    if (deviceConfig != 0)
        setDeviceParams( (powerstep01_init_u_t*)deviceConfig );
}

void IngiaPowerStep01::setDeviceParams(powerstep01_init_u_t *initPrm) {
    setParam(POWERSTEP01_ABS_POS, 0);
    setParam(POWERSTEP01_EL_POS, 0);
    setParam(POWERSTEP01_MARK, 0);
    setParam(POWERSTEP01_ACC,
        acc_dec_steps_s2_to_reg_val(initPrm->cm.cp.acceleration));
    setParam(POWERSTEP01_DEC,
        acc_dec_steps_s2_to_reg_val(initPrm->cm.cp.deceleration));
    setParam(POWERSTEP01_MAX_SPEED,
        max_spd_steps_s_to_reg_val(initPrm->cm.cp.maxSpeed));
    setParam(POWERSTEP01_MIN_SPEED,
        initPrm->cm.cp.lowSpeedOptimization|
        max_spd_steps_s_to_reg_val(initPrm->cm.cp.minSpeed));
    setParam(POWERSTEP01_FS_SPD,
        initPrm->cm.cp.boostMode|
        fs_spd_steps_s_to_reg_val(initPrm->cm.cp.fullStepSpeed));
    setParam(POWERSTEP01_OCD_TH,
        stall_ocd_th_to_reg_val(initPrm->cm.cp.ocdThreshold));
    setParam(POWERSTEP01_STEP_MODE,
        (uint8_t)initPrm->cm.cp.syncClockSelection|
        (uint8_t)initPrm->cm.cp.cmVmSelection|
        (uint8_t)(uint8_t)initPrm->cm.cp.stepMode);
    setParam(POWERSTEP01_ALARM_EN,
        initPrm->cm.cp.alarmsSelection);
    setParam(POWERSTEP01_GATECFG1,
        (uint16_t)initPrm->cm.cp.iGate|
        (uint16_t)initPrm->cm.cp.tcc|
        (uint16_t)initPrm->cm.cp.tBoost|
        (uint16_t)initPrm->cm.cp.wdEn);
    setParam(POWERSTEP01_GATECFG2,
        (uint16_t)initPrm->cm.cp.tBlank|
        (uint16_t)initPrm->cm.cp.tdt);
    if (initPrm->cm.cp.cmVmSelection == POWERSTEP01_CM_VM_VOLTAGE) {
        //Voltage mode
        setParam(POWERSTEP01_INT_SPD,
        int_spd_steps_s_to_reg_val(
            initPrm->vm.intersectSpeed));
        setParam(POWERSTEP01_K_THERM,
        k_therm_comp_to_reg_val(
            initPrm->vm.thermalCompensationFactor));
        setParam(POWERSTEP01_STALL_TH,
        stall_ocd_th_to_reg_val(
            initPrm->vm.stallThreshold));
        setParam(POWERSTEP01_KVAL_HOLD,
        k_val_perc_to_reg_val(
            initPrm->vm.kvalHold));
        setParam(POWERSTEP01_KVAL_RUN,
        k_val_perc_to_reg_val(
            initPrm->vm.kvalRun));
        setParam(POWERSTEP01_KVAL_ACC,
        k_val_perc_to_reg_val(
            initPrm->vm.kvalAcc));
        setParam(POWERSTEP01_KVAL_DEC,
        k_val_perc_to_reg_val(
            initPrm->vm.kvalDec));
        setParam(POWERSTEP01_ST_SLP,
        bemf_slope_perc_to_reg_val(
            initPrm->vm.startSlope));
        setParam(POWERSTEP01_FN_SLP_ACC,
        bemf_slope_perc_to_reg_val(
            initPrm->vm.accelerationFinalSlope));
        setParam(POWERSTEP01_FN_SLP_DEC,
        bemf_slope_perc_to_reg_val(
            initPrm->vm.decelerationFinalSlope));
        setParam(POWERSTEP01_CONFIG,
        (uint16_t)initPrm->vm.oscClkSel|
        (uint16_t)initPrm->vm.swMode |
        (uint16_t)initPrm->vm.enVsComp|
        (uint16_t)initPrm->vm.ocSd|
        (uint16_t)initPrm->vm.uvloVal|
        (uint16_t)initPrm->vm.vccVal|
        (uint16_t)initPrm->vm.fPwmInt|
        (uint16_t)initPrm->vm.fPwmDec);
    }
    else {
        // Current mode
        setParam(POWERSTEP01_TVAL_HOLD,
        t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalHold));
        setParam(POWERSTEP01_TVAL_RUN,
        t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalRun));
        setParam(POWERSTEP01_TVAL_ACC,
        t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalAcc));
        setParam(POWERSTEP01_TVAL_DEC,
        t_val_ref_voltage_to_reg_val(
            initPrm->cm.tvalDec));
        setParam(POWERSTEP01_T_FAST,
        (uint8_t)initPrm->cm.toffFast|
        (uint8_t)initPrm->cm.fastStep);
        setParam(POWERSTEP01_TON_MIN,
        t_min_time_to_reg_val(
            initPrm->cm.tonMin));
        setParam(POWERSTEP01_TOFF_MIN,
        t_min_time_to_reg_val(
            initPrm->cm.toffMin));
        setParam(POWERSTEP01_CONFIG,
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

powerstep01_Status_t IngiaPowerStep01::parseStatus(uint16_t status) {
    powerstep01_Status_t salida;

    salida.hiZ = (status & POWERSTEP01_STATUS_HIZ) ? true : false;
    salida.uvlo = (status & POWERSTEP01_STATUS_UVLO) ? false : true;
    salida.uvlo_adc = (status & POWERSTEP01_STATUS_UVLO_ADC) ? false : true;
    salida.ocd = (status & POWERSTEP01_STATUS_OCD) ? false : true;
    salida.stall_A = (status & POWERSTEP01_STATUS_STALL_A) ? false : true;
    salida.stall_B = (status & POWERSTEP01_STATUS_STALL_B) ? false : true;
    salida.cmd_error = (status & POWERSTEP01_STATUS_CMD_ERROR) ? true : false;
    salida.sw_f = (status & POWERSTEP01_STATUS_SW_F) ? SW_CLOSED : SW_OPENED;
    salida.sw_evn = (status & POWERSTEP01_STATUS_SW_EVN) ? true : false;
    salida.busy = (status & POWERSTEP01_STATUS_BUSY) ? false : true;
    salida.stck_mod = (status & POWERSTEP01_STATUS_STCK_MOD) ? true : false;
    salida.th_status = (th_Status_t)((status & POWERSTEP01_STATUS_TH_STATUS) >> 11);
    salida.mot_status = (motorStatus_t)((status & POWERSTEP01_STATUS_MOT_STATUS) >> 5);

    return salida;
}

uint8_t IngiaPowerStep01::getParamLen(powerstep01_Registers_t dir) {
    uint8_t len;
    switch(dir) {
        case POWERSTEP01_ABS_POS:
        case POWERSTEP01_MARK:
            len = 22;
            break;
        case POWERSTEP01_SPEED:
            len = 20;
            break;
        case POWERSTEP01_STATUS:
        case POWERSTEP01_CONFIG:
            len = 16;
            break;
        case POWERSTEP01_INT_SPD:
            len = 14;
            break;
        case POWERSTEP01_ACC:
        case POWERSTEP01_DEC:
        case POWERSTEP01_MIN_SPEED:
            len = 12;
            break;
        case POWERSTEP01_FS_SPD:
        case POWERSTEP01_GATECFG1:
            len = 11;
            break;
        case POWERSTEP01_MAX_SPEED:
            len = 10;
            break;
        case POWERSTEP01_EL_POS:
            len = 9;
            break;
        case POWERSTEP01_GATECFG2:
        case POWERSTEP01_STEP_MODE:
        case POWERSTEP01_ALARM_EN:
        case POWERSTEP01_KVAL_HOLD:
        case POWERSTEP01_KVAL_RUN:
        case POWERSTEP01_KVAL_ACC:
        case POWERSTEP01_KVAL_DEC:
        case POWERSTEP01_ST_SLP:
        case POWERSTEP01_FN_SLP_ACC:
        case POWERSTEP01_FN_SLP_DEC:
            len = 8;
            break;
        case POWERSTEP01_ADC_OUT:
        case POWERSTEP01_STALL_TH:
        case POWERSTEP01_OCD_TH:
            len = 5;
            break;
        case POWERSTEP01_K_THERM:
            len = 4;
            break;
        default:
            len = 0;
    }
    return len;
}