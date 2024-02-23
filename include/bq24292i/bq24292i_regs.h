#pragma once

// BQ24292i i2c address
#define BQ_ADDR 0x6BU

// BQ24292i registers
#define BQ_INPUT_SRC_CTRL 0x00U            // R/W: Input source control
#define BQ_PWR_ON_CONF 0x01U               // R/W: Power-on configuration
#define BQ_CHRG_CURRENT_CTRL 0x02U         // R/W: Charge current control
#define BQ_PRECHRG_TERM_CURRENT_CTRL 0x03U // R/W: Pre-charge/termination current control
#define BQ_CHRG_VOLTAGE_CTRL 0x04U         // R/W: Charge voltage control
#define BQ_CHRG_TERM_TIMER_CTRL 0x05U      // R/W: Charge termination/timer control
#define BQ_IR_COMP_THERMAL_REG_CTRL 0x06U  // R/W: IR compensation / thermal regulation control
#define BQ_MISC_OPERATION_CTRL 0x07U       // R/W: Misc operation control
#define BQ_SYSTEM_STATUS 0x08U             // R/-: System status
#define BQ_FAULT 0x09U                     // R/-: Fault (cleared after read)
#define BQ_VENDOR_PART_REV_STATUS 0x0AU    // R/-: Vendor / part / revision status

// BQ24292i masks/constants
#define BQ_IIN_MAX_POS 0U
#define BQ_IIN_MAX_MSK (0x07U << BQ_IIN_MAX_POS)

#define BQ_VIN_MAX_POS 3U
#define BQ_VIN_MAX_MSK (0x0FU << BQ_VIN_MAX_POS)
#define BQ_VIN_MAX_OFFSET 3880U
#define BQ_VIN_MAX_INCR 80U

#define BQ_VSYS_MIN_POS 1U
#define BQ_VSYS_MIN_MSK (0x07U << BQ_VSYS_MIN_POS)
#define BQ_VSYS_MIN_OFFSET 3000U
#define BQ_VSYS_MIN_INCR 100U

#define BQ_CHG_CONFIG_POS 4U
#define BQ_CHG_CONFIG_MSK (0x03U << BQ_CHG_CONFIG_POS)

#define BQ_WDT_POS 6U
#define BQ_WDT_MSK (0x01U << BQ_WDT_POS)

#define BQ_ICHG_POS 2U
#define BQ_ICHG_MSK (0x3FU << BQ_ICHG_POS)
#define BQ_ICHG_OFFSET 512U
#define BQ_ICHG_INCR 64U

#define BQ_ITERM_POS 0U
#define BQ_ITERM_MSK (0x0FU << BQ_ITERM_POS)
#define BQ_ITERM_OFFSET 128U
#define BQ_ITERM_INCR 128U

#define BQ_IPRECHG_POS 4U
#define BQ_IPRECHG_MSK (0x0FU << BQ_IPRECHG_POS)
#define BQ_IPRECHG_OFFSET 128U
#define BQ_IPRECHG_INCR 128U

#define BQ_VRECHG_POS 0U
#define BQ_VRECHG_MSK (0x01U << BQ_VRECHG_POS)

#define BQ_VBATLOW_POS 1U
#define BQ_VBATLOW_MSK (0x01 << BQ_VBATLOW_POS)

#define BQ_VCHG_MAX_POS 2U
#define BQ_VCHG_MAX_MSK (0x3FU << BQ_VCHG_MAX_POS)
#define BQ_VCHG_MAX_OFFSET 3504U
#define BQ_VCHG_MAX_INCR 16U

#define BQ_CHG_TIMER_POS 1U
#define BQ_CHG_TIMER_MSK (0x03U << BQ_CHG_TIMER_POS)
#define BQ_CHG_TIMER_EN_POS 3U
#define BQ_CHG_TIMER_EN_MSK (0x01U << BQ_CHG_TIMER_EN_POS)

#define BQ_WDT_CONF_POS 4U
#define BQ_WDT_CONF_MSK (0x03U << BQ_WDT_CONF_POS)

#define BQ_TERM_EN_POS 7U
#define BQ_TERM_EN_MSK (0x01U << BQ_TERM_EN_POS)

#define BQ_THERMAL_REG_POS 0U
#define BQ_THERMAL_REG_MSK (0x03U << BQ_THERMAL_REG_POS)

#define BQ_VCLAMP_POS 2U
#define BQ_VCLAMP_MSK (0x07U << BQ_VCLAMP_POS)
#define BQ_VCLAMP_OFFSET 0U
#define BQ_VCLAMP_INCR 16U

#define BQ_BAT_COMP_POS 5U
#define BQ_BAT_COMP_MSK (0x07U << BQ_BAT_COMP_POS)
#define BQ_BAT_COMP_OFFSET 0U
#define BQ_BAT_COMP_INCR 10U

#define BQ_INT_MASK_POS 0U
#define BQ_INT_MASK_MSK (0x03U << BQ_INT_MASK_POS)

#define BQ_BATFET_POS 5U
#define BQ_BATFET_MSK (0x01U << BQ_BATFET_POS)

#define BQ_VSYS_STAT_POS 0U
#define BQ_VSYS_STAT_MSK (0x01U << BQ_VSYS_STAT_POS)

#define BQ_THERM_STAT_POS 1U
#define BQ_THERM_STAT_MSK (0x01U << BQ_THERM_STAT_POS)

#define BQ_PG_STAT_POS 2U
#define BQ_PG_STAT_MSK (0x01U << BQ_PG_STAT_POS)

#define BQ_DPM_STAT_POS 3U
#define BQ_DPM_STAT_MSK (0x01U << BQ_DPM_STAT_POS)

#define BQ_CHRG_STAT_POS 4U
#define BQ_CHRG_STAT_MSK (0x03U << BQ_CHRG_STAT_POS)

#define BQ_VBUS_STAT_POS 6U
#define BQ_VBUS_STAT_MSK (0x03U << BQ_VBUS_STAT_POS)

#define BQ_PART_NUMBER_POS 3U
#define BQ_PART_NUMBER_MSK (0x07U << BQ_PART_NUMBER_POS)
#define BQ_PART_NUMBER 0b011U
