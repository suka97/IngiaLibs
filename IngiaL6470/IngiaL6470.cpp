#include "IngiaL6470.h"

void IngiaL6470::initDevice (void *deviceConfig) {
    if ( reset != 0 ) {
        digitalWrite(reset, LOW);
        delay(1);
        digitalWrite(reset, HIGH);
    }
    if (deviceConfig != 0)
        setDeviceParams( (l6470_init_t*)deviceConfig );
}

void IngiaL6470::setDeviceParams(l6470_init_t *initPrm) {
    setParam(L6470_ABS_POS, 0);
    setParam(L6470_EL_POS, 0);
    setParam(L6470_MARK, 0);
    setParam(L6470_ACC,
        acc_dec_steps_s2_to_reg_val(initPrm->acceleration));
    setParam(L6470_DEC,
        acc_dec_steps_s2_to_reg_val(initPrm->deceleration));
    setParam(L6470_MAX_SPEED,
        max_spd_steps_s_to_reg_val(initPrm->maxSpeed));
    setParam(L6470_MIN_SPEED,
        initPrm->lowSpeedOptimization|
        max_spd_steps_s_to_reg_val(initPrm->minSpeed));
    setParam(L6470_FS_SPD,
        fs_spd_steps_s_to_reg_val(initPrm->fullStepSpeed));
    setParam(L6470_OCD_TH,
        ocd_th_to_reg_val(initPrm->ocdThreshold));
    setParam(L6470_STEP_MODE,
        (uint8_t)initPrm->syncClockSelection|
        (uint8_t)(uint8_t)initPrm->stepMode);
    setParam(L6470_ALARM_EN,
        initPrm->alarmsSelection);
    setParam(L6470_INT_SPD,
        int_spd_steps_s_to_reg_val(initPrm->intersectSpeed));        // Revisar, no coincide datasheet
    setParam(L6470_K_THERM,
        k_therm_comp_to_reg_val(initPrm->thermalCompensationFactor));
    setParam(L6470_STALL_TH,
        stall_th_to_reg_val(initPrm->stallThreshold));
    setParam(L6470_KVAL_HOLD,
        k_val_perc_to_reg_val(initPrm->kvalHold));
    setParam(L6470_KVAL_RUN,
        k_val_perc_to_reg_val(initPrm->kvalRun));
    setParam(L6470_KVAL_ACC,
        k_val_perc_to_reg_val(initPrm->kvalAcc));
    setParam(L6470_KVAL_DEC,
        k_val_perc_to_reg_val(initPrm->kvalDec));
    setParam(L6470_ST_SLP,
        bemf_slope_perc_to_reg_val(initPrm->startSlope));
    setParam(L6470_FN_SLP_ACC,
        bemf_slope_perc_to_reg_val(initPrm->accelerationFinalSlope));
    setParam(L6470_FN_SLP_DEC,
        bemf_slope_perc_to_reg_val(initPrm->decelerationFinalSlope));
    setParam(L6470_CONFIG,
        (uint16_t)initPrm->oscClkSel|
        (uint16_t)initPrm->swMode |
        (uint16_t)initPrm->enVsComp|
        (uint16_t)initPrm->ocSd|
        (uint16_t)initPrm->powSR|
        (uint16_t)initPrm->fPwmInt|
        (uint16_t)initPrm->fPwmDec);
}

l6470_Status_t IngiaL6470::parseStatus(uint16_t status) {
    l6470_Status_t salida;

    salida.hiZ = (status & L6470_STATUS_HIZ) ? true : false;
    salida.uvlo = (status & L6470_STATUS_UVLO) ? false : true;
    salida.th_sd = (status & L6470_STATUS_TH_SD) ? false : true;
    salida.th_wrn = (status & L6470_STATUS_TH_WRN) ? false : true;
    salida.ocd = (status & L6470_STATUS_OCD) ? false : true;
    salida.step_loss_A = (status & L6470_STATUS_STEP_LOSS_A) ? false : true;
    salida.step_loss_B = (status & L6470_STATUS_STEP_LOSS_B) ? false : true;
    salida.notperf_cmd = (status & L6470_STATUS_NOTPERF_CMD) ? true : false;
    salida.wrong_cmd = (status & L6470_STATUS_WRONG_CMD) ? true : false;
    salida.sw_f = (status & L6470_STATUS_SW_F) ? SW_CLOSED : SW_OPENED;
    salida.sw_evn = (status & L6470_STATUS_SW_EVN) ? true : false;
    salida.busy = (status & L6470_STATUS_BUSY) ? false : true;
    salida.sck_mod = (status & L6470_STATUS_SCK_MOD) ? true : false;
    salida.mot_status = (motorStatus_t)((status & L6470_STATUS_MOT_STATUS) >> 5);
    salida.dir = (direction_t)((status & L6470_STATUS_DIR) >> 4);
    
    salida.cmd_error = salida.notperf_cmd || salida.wrong_cmd;

    return salida;
}

uint8_t IngiaL6470::getParamLen(l6470_Registers_t dir) {
    uint8_t len;
    switch(dir) {
        case L6470_ABS_POS:
        case L6470_MARK:
            len = 22;
            break;
        case L6470_SPEED:
            len = 20;
            break;
        case L6470_STATUS:
        case L6470_CONFIG:
            len = 16;
            break;
        case L6470_INT_SPD:
            len = 14;
            break;
        case L6470_MIN_SPEED:
            len = 13;
            break;
        case L6470_ACC:
        case L6470_DEC:
            len = 12;
            break;
        case L6470_FS_SPD:
        case L6470_MAX_SPEED:
            len = 10;
            break;
        case L6470_EL_POS:
            len = 9;
            break;
        case L6470_KVAL_HOLD:
        case L6470_KVAL_RUN:
        case L6470_KVAL_ACC:
        case L6470_KVAL_DEC:
        case L6470_ST_SLP:
        case L6470_FN_SLP_ACC:
        case L6470_FN_SLP_DEC:
        case L6470_STEP_MODE:
        case L6470_ALARM_EN:
            len = 8;
            break;
        case L6470_STALL_TH:
            len = 7;
            break;
        case L6470_ADC_OUT:
            len = 5;
            break;
        case L6470_K_THERM:
        case L6470_OCD_TH:
            len = 4;
            break;
        default:
            len = 0;
    }
    return len;
}