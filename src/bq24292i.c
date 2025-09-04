#include "bq24292i.h"

#include "bq24292i/bq24292i_regs.h"

static inline bool read_reg(bq24292i_t const* dev, uint8_t reg, uint8_t* out) {
    return dev->read(BQ_ADDR, reg, out, 1, dev->context);
}

static inline bool write_reg(bq24292i_t const* dev, uint8_t reg, uint8_t data) {
    return dev->write(BQ_ADDR, reg, &data, 1, dev->context);
}

static inline bool modify_reg(bq24292i_t const* dev, uint8_t reg, uint8_t data, uint8_t mask) {
    uint8_t buf;
    if (!read_reg(dev, reg, &buf)) {
        return false;
    }

    buf = (buf & ~mask) | (data & mask);
    return write_reg(dev, reg, buf);
}

static uint8_t perform_adc(uint16_t in, uint16_t offset, uint16_t incr) {
    if (in < offset) {
        return 0;
    }

    in -= offset;
    uint8_t out = 0;

    for (uint8_t i = 0; i < 8; i++) {
        uint16_t val = incr * (1u << (7 - i));
        if (in >= val) {
            in -= val;
            out |= (1u << (7 - i));
        }
    }

    return out;
}

static uint16_t perform_dac(uint8_t in, uint16_t offset, uint16_t incr) {
    uint16_t out = offset;
    for (uint8_t i = 0; i < 8; i++) {
        if (in & (1u << i)) {
            out += incr * (1u << i);
        }
    }

    return out;
}

bool bq24292i_is_present(bq24292i_t const* dev) {
    uint8_t data;
    if (!read_reg(dev, BQ_VENDOR_PART_REV_STATUS, &data)) {
        return false;
    }

    return (((data & BQ_PART_NUMBER_MSK) >> BQ_PART_NUMBER_POS) == BQ_PART_NUMBER);
}

bool bq24292i_set_iin_max(bq24292i_t const* dev, bq24292i_iin_max_t ma) {
    uint8_t data = (uint8_t)((ma << BQ_IIN_MAX_POS) & BQ_IIN_MAX_MSK);

    return modify_reg(dev, BQ_INPUT_SRC_CTRL, data, BQ_IIN_MAX_MSK);
}

bool bq24292i_get_iin_max(bq24292i_t const* dev, bq24292i_iin_max_t* ma) {
    if (!ma) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_INPUT_SRC_CTRL, &data)) {
        return false;
    }

    *ma = (bq24292i_iin_max_t)((data & BQ_IIN_MAX_MSK) >> BQ_IIN_MAX_POS);
    return true;
}

bool bq24292i_set_vin_max(bq24292i_t const* dev, bq24292i_vin_max_t mv) {
    uint8_t data =
      (perform_adc(mv, BQ_VIN_MAX_OFFSET, BQ_VIN_MAX_INCR) << BQ_VIN_MAX_POS) & BQ_VIN_MAX_MSK;

    return modify_reg(dev, BQ_INPUT_SRC_CTRL, data, BQ_VIN_MAX_MSK);
}

bool bq24292i_get_vin_max(bq24292i_t const* dev, bq24292i_vin_max_t* mv) {
    if (!mv) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_INPUT_SRC_CTRL, &data)) {
        return false;
    }

    *mv = (bq24292i_vin_max_t)(perform_dac(
      ((data & BQ_VIN_MAX_MSK) >> BQ_VIN_MAX_POS), BQ_VIN_MAX_OFFSET, BQ_VIN_MAX_INCR));
    return true;
}

bool bq24292i_set_vsys_min(bq24292i_t const* dev, bq24292i_vsys_min_t mv) {
    uint8_t data =
      (perform_adc(mv, BQ_VSYS_MIN_OFFSET, BQ_VSYS_MIN_INCR) << BQ_VSYS_MIN_POS) & BQ_VSYS_MIN_MSK;

    return modify_reg(dev, BQ_PWR_ON_CONF, data, BQ_VSYS_MIN_MSK);
}

bool bq24292i_get_vsys_min(bq24292i_t const* dev, bq24292i_vsys_min_t* mv) {
    if (!mv) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_PWR_ON_CONF, &data)) {
        return false;
    }

    *mv = (bq24292i_vsys_min_t)(perform_dac(
      ((data & BQ_VSYS_MIN_MSK) >> BQ_VSYS_MIN_POS), BQ_VSYS_MIN_OFFSET, BQ_VSYS_MIN_INCR));
    return true;
}

bool bq24292i_set_charge_config(bq24292i_t const* dev, bq24292i_chg_config_t conf) {
    uint8_t data = (uint8_t)((conf << BQ_CHG_CONFIG_POS) & BQ_CHG_CONFIG_MSK);

    return modify_reg(dev, BQ_PWR_ON_CONF, data, BQ_CHG_CONFIG_MSK);
}

bool bq24292i_get_charge_config(bq24292i_t const* dev, bq24292i_chg_config_t* conf) {
    if (!conf) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_PWR_ON_CONF, &data)) {
        return false;
    }

    *conf = (bq24292i_chg_config_t)((data & BQ_CHG_CONFIG_MSK) >> BQ_CHG_CONFIG_POS);
    return true;
}

bool bq24292i_reset_wdt(bq24292i_t const* dev) {
    return modify_reg(dev, BQ_PWR_ON_CONF, (1u << BQ_WDT_POS), BQ_WDT_MSK);
}

bool bq24292i_set_charge_current(bq24292i_t const* dev, bq24292i_chg_current_t ma) {
    uint8_t data = (perform_adc(ma, BQ_ICHG_OFFSET, BQ_ICHG_INCR) << BQ_ICHG_POS) & BQ_ICHG_MSK;

    return modify_reg(dev, BQ_CHRG_CURRENT_CTRL, data, BQ_ICHG_MSK);
}

bool bq24292i_get_charge_current(bq24292i_t const* dev, bq24292i_chg_current_t* ma) {
    if (!ma) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_CURRENT_CTRL, &data)) {
        return false;
    }

    *ma = (bq24292i_chg_current_t)(perform_dac(
      ((data & BQ_ICHG_MSK) >> BQ_ICHG_POS), BQ_ICHG_OFFSET, BQ_ICHG_INCR));
    return true;
}

bool bq24292i_set_term_current(bq24292i_t const* dev, bq24292i_term_current_t ma) {
    uint8_t data = (perform_adc(ma, BQ_ITERM_OFFSET, BQ_ITERM_INCR) << BQ_ITERM_POS) & BQ_ITERM_MSK;

    return modify_reg(dev, BQ_PRECHRG_TERM_CURRENT_CTRL, data, BQ_ITERM_MSK);
}

bool bq24292i_get_term_current(bq24292i_t const* dev, bq24292i_term_current_t* ma) {
    if (!ma) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_PRECHRG_TERM_CURRENT_CTRL, &data)) {
        return false;
    }

    *ma = (bq24292i_term_current_t)(perform_dac(
      ((data & BQ_ITERM_MSK) >> BQ_ITERM_POS), BQ_ITERM_OFFSET, BQ_ITERM_INCR));
    return true;
}

bool bq24292i_set_precharge_current(bq24292i_t const* dev, bq24292i_prechg_current_t ma) {
    uint8_t data =
      (perform_adc(ma, BQ_IPRECHG_OFFSET, BQ_IPRECHG_INCR) << BQ_IPRECHG_POS) & BQ_IPRECHG_MSK;

    return modify_reg(dev, BQ_PRECHRG_TERM_CURRENT_CTRL, data, BQ_IPRECHG_MSK);
}

bool bq24292i_get_precharge_current(bq24292i_t const* dev, bq24292i_prechg_current_t* ma) {
    if (!ma) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_PRECHRG_TERM_CURRENT_CTRL, &data)) {
        return false;
    }

    *ma = (bq24292i_prechg_current_t)(perform_dac(
      ((data & BQ_IPRECHG_MSK) >> BQ_IPRECHG_POS), BQ_IPRECHG_OFFSET, BQ_IPRECHG_INCR));
    return true;
}

bool bq24292i_set_recharge_offset(bq24292i_t const* dev, bq24292i_vrechg_offset_t offset) {
    uint8_t data = (uint8_t)((offset << BQ_VRECHG_POS) & BQ_VRECHG_MSK);

    return modify_reg(dev, BQ_CHRG_VOLTAGE_CTRL, data, BQ_VRECHG_MSK);
}

bool bq24292i_get_recharge_offset(bq24292i_t const* dev, bq24292i_vrechg_offset_t* offset) {
    if (!offset) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_VOLTAGE_CTRL, &data)) {
        return false;
    }

    *offset = (bq24292i_vrechg_offset_t)((data & BQ_VRECHG_MSK) >> BQ_VRECHG_POS);
    return true;
}

bool bq24292i_set_batlow_voltage(bq24292i_t const* dev, bq24292i_vbatlow_t mv) {
    uint8_t data = (uint8_t)((mv << BQ_VBATLOW_POS) & BQ_VBATLOW_MSK);

    return modify_reg(dev, BQ_CHRG_VOLTAGE_CTRL, data, BQ_VBATLOW_MSK);
}

bool bq24292i_get_batlow_voltage(bq24292i_t const* dev, bq24292i_vbatlow_t* mv) {
    if (!mv) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_VOLTAGE_CTRL, &data)) {
        return false;
    }

    *mv = (bq24292i_vbatlow_t)((data & BQ_VBATLOW_MSK) >> BQ_VBATLOW_POS);
    return true;
}

bool bq24292i_set_max_charge_voltage(bq24292i_t const* dev, bq24292i_vchg_max_t mv) {
    uint8_t data =
      (perform_adc(mv, BQ_VCHG_MAX_OFFSET, BQ_VCHG_MAX_INCR) << BQ_VCHG_MAX_POS) & BQ_VCHG_MAX_MSK;

    return modify_reg(dev, BQ_CHRG_VOLTAGE_CTRL, data, BQ_VCHG_MAX_MSK);
}

bool bq24292i_get_max_charge_voltage(bq24292i_t const* dev, bq24292i_vchg_max_t* mv) {
    if (!mv) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_VOLTAGE_CTRL, &data)) {
        return false;
    }

    *mv = (bq24292i_vchg_max_t)(perform_dac(
      ((data & BQ_VCHG_MAX_MSK) >> BQ_VCHG_MAX_POS), BQ_VCHG_MAX_OFFSET, BQ_VCHG_MAX_INCR));
    return true;
}

bool bq24292i_set_charge_timer(bq24292i_t const* dev, bq24292i_chg_timer_t conf) {
    if (conf != BQ_CHG_TIMER_DISABLE) {
        uint8_t data = (uint8_t)(((1u << BQ_CHG_TIMER_EN_POS) & BQ_CHG_TIMER_EN_MSK) |
                                 ((conf << BQ_CHG_TIMER_POS) & BQ_CHG_TIMER_MSK));
        return modify_reg(
          dev, BQ_CHRG_TERM_TIMER_CTRL, data, BQ_CHG_TIMER_EN_MSK | BQ_CHG_TIMER_MSK);
    } else {
        return modify_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, 0, BQ_CHG_TIMER_EN_MSK);
    }
}

bool bq24292i_get_charge_timer(bq24292i_t const* dev, bq24292i_chg_timer_t* conf) {
    if (!conf) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, &data)) {
        return false;
    }

    if (!(data & BQ_CHG_TIMER_EN_MSK)) {
        *conf = BQ_CHG_TIMER_DISABLE;
    } else {
        *conf = (bq24292i_chg_timer_t)((data & BQ_CHG_TIMER_MSK) >> BQ_CHG_TIMER_POS);
    }
    return true;
}

bool bq24292i_set_wdt_config(bq24292i_t const* dev, bq24292i_watchdog_conf_t conf) {
    uint8_t data = (uint8_t)((conf << BQ_WDT_CONF_POS) & BQ_WDT_CONF_MSK);

    return modify_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, data, BQ_WDT_CONF_MSK);
}

bool bq24292i_get_wdt_config(bq24292i_t const* dev, bq24292i_watchdog_conf_t* conf) {
    if (!conf) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, &data)) {
        return false;
    }

    *conf = (bq24292i_watchdog_conf_t)((data & BQ_WDT_CONF_MSK) >> BQ_WDT_CONF_POS);
    return true;
}

bool bq24292i_set_charge_termination(bq24292i_t const* dev, bool enable) {
    uint8_t data = (uint8_t)((enable << BQ_TERM_EN_POS) & BQ_TERM_EN_MSK);

    return modify_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, data, BQ_TERM_EN_MSK);
}

bool bq24292i_get_charge_termination(bq24292i_t const* dev, bool* enable) {
    if (!enable) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_CHRG_TERM_TIMER_CTRL, &data)) {
        return false;
    }

    *enable = ((data & BQ_TERM_EN_MSK) >> BQ_TERM_EN_POS);
    return true;
}

bool bq24292i_set_max_temp(bq24292i_t const* dev, bq_24292i_max_temp_t temp) {
    uint8_t data = (uint8_t)((temp << BQ_THERMAL_REG_POS) & BQ_THERMAL_REG_MSK);

    return modify_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, data, BQ_THERMAL_REG_MSK);
}

bool bq24292i_get_max_temp(bq24292i_t const* dev, bq_24292i_max_temp_t* temp) {
    if (!temp) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, &data)) {
        return false;
    }

    *temp = (bq_24292i_max_temp_t)((data & BQ_THERMAL_REG_MSK) >> BQ_THERMAL_REG_POS);
    return true;
}

bool bq24292i_set_voltage_clamp(bq24292i_t const* dev, bq24292i_clamp_voltage_t mv) {
    uint8_t data =
      (perform_adc(mv, BQ_VCLAMP_OFFSET, BQ_VCLAMP_INCR) << BQ_VCLAMP_POS) & BQ_VCLAMP_MSK;

    return modify_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, data, BQ_VCLAMP_MSK);
}

bool bq24292i_get_voltage_clamp(bq24292i_t const* dev, bq24292i_clamp_voltage_t* mv) {
    if (!mv) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, &data)) {
        return false;
    }

    *mv = (bq24292i_clamp_voltage_t)(perform_dac(
      ((data & BQ_VCLAMP_MSK) >> BQ_VCLAMP_POS), BQ_VCLAMP_OFFSET, BQ_VCLAMP_INCR));
    return true;
}

bool bq24292i_set_comp_resistor(bq24292i_t const* dev, bq24292i_comp_resistor_t mohms) {
    uint8_t data = (perform_adc(mohms, BQ_BAT_COMP_OFFSET, BQ_BAT_COMP_INCR) << BQ_BAT_COMP_POS) &
                   BQ_BAT_COMP_MSK;

    return modify_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, data, BQ_BAT_COMP_MSK);
}

bool bq24292i_get_comp_resistor(bq24292i_t const* dev, bq24292i_comp_resistor_t* mohms) {
    if (!mohms) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_IR_COMP_THERMAL_REG_CTRL, &data)) {
        return false;
    }

    *mohms = (bq24292i_comp_resistor_t)(perform_dac(
      ((data & BQ_BAT_COMP_MSK) >> BQ_BAT_COMP_POS), BQ_BAT_COMP_OFFSET, BQ_BAT_COMP_INCR));
    return true;
}

bool bq24292i_set_interrupt_mask(bq24292i_t const* dev, bq24292i_interrupt_mask_t mask) {
    uint8_t data = (uint8_t)((mask << BQ_INT_MASK_POS) & BQ_INT_MASK_MSK);

    return modify_reg(dev, BQ_MISC_OPERATION_CTRL, data, BQ_INT_MASK_MSK);
}

bool bq24292i_get_interrupt_mask(bq24292i_t const* dev, bq24292i_interrupt_mask_t* mask) {
    if (!mask) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_MISC_OPERATION_CTRL, &data)) {
        return false;
    }

    *mask = (bq24292i_interrupt_mask_t)((data & BQ_INT_MASK_MSK) >> BQ_INT_MASK_POS);
    return true;
}

bool bq24292i_set_batfet_enabled(bq24292i_t const* dev, bool enable) {
    uint8_t data = (uint8_t)((enable << BQ_BATFET_POS) & BQ_BATFET_MSK);

    return modify_reg(dev, BQ_MISC_OPERATION_CTRL, data, BQ_BATFET_MSK);
}

bool bq24292i_get_batfet_enabled(bq24292i_t const* dev, bool* enable) {
    if (!enable) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_MISC_OPERATION_CTRL, &data)) {
        return false;
    }

    *enable = ((data & BQ_BATFET_MSK) >> BQ_BATFET_POS);
    return true;
}

bool bq24292i_is_vsys_boosted(bq24292i_t const* dev, bool* result) {
    if (!result) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *result = ((data & BQ_VSYS_STAT_MSK) >> BQ_VSYS_STAT_POS);
    return true;
}

bool bq24292i_is_overtemp(bq24292i_t const* dev, bool* result) {
    if (!result) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *result = ((data & BQ_THERM_STAT_MSK) >> BQ_THERM_STAT_POS);
    return true;
}

bool bq24292i_is_charger_connected(bq24292i_t const* dev, bool* result) {
    if (!result) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *result = ((data & BQ_PG_STAT_MSK) >> BQ_PG_STAT_POS);
    return true;
}

bool bq24292i_is_in_dpm(bq24292i_t const* dev, bool* result) {
    if (!result) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *result = ((data & BQ_DPM_STAT_MSK) >> BQ_DPM_STAT_POS);
    return true;
}

bool bq24292i_get_charge_state(bq24292i_t const* dev, bq24292i_charge_state_t* state) {
    if (!state) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *state = (bq24292i_charge_state_t)((data & BQ_CHRG_STAT_MSK) >> BQ_CHRG_STAT_POS);
    return true;
}

bool bq24292i_get_source_type(bq24292i_t const* dev, bq24292i_source_type_t* source) {
    if (!source) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_SYSTEM_STATUS, &data)) {
        return false;
    }

    *source = (bq24292i_source_type_t)((data & BQ_VBUS_STAT_MSK) >> BQ_VBUS_STAT_POS);
    return true;
}

bool bq24292i_check_faults(bq24292i_t const* dev, bq24292i_fault_t* faults) {
    if (!faults) return false;

    uint8_t data;
    if (!read_reg(dev, BQ_FAULT, &data)) {
        return false;
    }
    // after read, the fault register is cleared

    *faults = data;
    return true;
}
