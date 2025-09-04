#pragma once

#include <stdint.h>

typedef enum {
    BQ_IIN_MAX_100MA = 0x00U,
    BQ_IIN_MAX_150MA = 0x01U,
    BQ_IIN_MAX_500MA = 0x02U,
    BQ_IIN_MAX_900MA = 0x03U,
    BQ_IIN_MAX_1200MA = 0x04U,
    BQ_IIN_MAX_1500MA = 0x05U,
    BQ_IIN_MAX_2000MA = 0x06U,
    BQ_IIN_MAX_3000MA = 0x07U,
} bq24292i_iin_max_t;

typedef uint16_t bq24292i_vin_max_t;

typedef uint16_t bq24292i_vsys_min_t;

typedef enum {
    BQ_CHG_CONFIG_DISABLE = 0x00U,
    BQ_CHG_CONFIG_ENABLE = 0x01U,
    BQ_CHG_CONFIG_OTG = 0x02U,
} bq24292i_chg_config_t;

typedef uint16_t bq24292i_chg_current_t;

typedef uint16_t bq24292i_term_current_t;

typedef uint16_t bq24292i_prechg_current_t;

typedef enum {
    BQ_VRECHG_100MV = 0x00U,
    BQ_VRECHG_300MV = 0x01U,
} bq24292i_vrechg_offset_t;

typedef enum {
    BQ_VBATLOW_2800MV = 0x00U,
    BQ_VBATLOW_3000MV = 0x01U,
} bq24292i_vbatlow_t;

typedef uint16_t bq24292i_vchg_max_t;

typedef enum {
    BQ_CHG_TIMER_5H = 0x00U,
    BQ_CHG_TIMER_8H = 0x01U,
    BQ_CHG_TIMER_12H = 0x02U,
    BQ_CHG_TIMER_20H = 0x03U,
    BQ_CHG_TIMER_DISABLE,
} bq24292i_chg_timer_t;

typedef enum {
    BQ_WATCHDOG_DISABLE = 0x00U,
    BQ_WATCHDOG_40S = 0x01U,
    BQ_WATCHDOG_80S = 0x02U,
    BQ_WATCHDOG_160S = 0x03U,
} bq24292i_watchdog_conf_t;

typedef enum {
    BQ_MAX_TEMP_60C = 0x00U,
    BQ_MAX_TEMP_80C = 0x01U,
    BQ_MAX_TEMP_100C = 0x02U,
    BQ_MAX_TEMP_120C = 0x03U,
} bq_24292i_max_temp_t;

typedef uint8_t bq24292i_clamp_voltage_t;

typedef uint8_t bq24292i_comp_resistor_t;

typedef enum {
    BQ_INT_NONE = 0x00U,
    BQ_INT_BAT_FAULT = 0x01U,
    BQ_INT_CHRG_FAULT = 0x02U,
    BQ_INT_BAT_AND_CHRG_FAULT = 0x03U,
} bq24292i_interrupt_mask_t;

typedef enum {
    BQ_STATE_NOT_CHARGING = 0x00U,
    BQ_STATE_PRECHARGE = 0x01U,
    BQ_STATE_FAST_CHARGE = 0x02U,
    BQ_STATE_TERMINATED = 0x03U,
} bq24292i_charge_state_t;

typedef enum {
    BQ_SOURCE_UNKNOWN = 0x00U,
    BQ_SOURCE_USB = 0x01U,
    BQ_SOURCE_ADAPTER = 0x02U,
    BQ_SOURCE_OTG = 0x03U,
} bq24292i_source_type_t;

enum {
    BQ_FAULT_THERM = 0x07U,
    BQ_FAULT_BAT = 0x08U,
    BQ_FAULT_CHG = 0x30U,
    BQ_FAULT_BOOST = 0x40U,
    BQ_FAULT_WATCHDOG = 0x80U,
};
typedef uint8_t bq24292i_fault_t;
