/*
 * DeviceConfig.h
 *
 *  Created on: 28 feb. 2022
 *      Author: Christian Klemetsson
 */

#ifndef INC_DEVICECONFIG_H_
#define INC_DEVICECONFIG_H_

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Convert reference resistance values to ADC raw values based of the ERTJ1VV104
// ADC is 10-bit, 0.5x gain and uses 1.65V reference
#define TEMP_RES_2_ADC(res) ((uint16_t)((1023 * res) / (1 + res)))

// SMBus helpers
#define smbus_write_command(cmd) { \
  smbus_busy = true; \
  smbus_error = false; \
  smbus_read = false; \
  smbus_cmd = cmd; \
  SMB0CN0_STA = true; \
}
#define smbus_read16() { \
  smbus_busy = true; \
  smbus_error = false; \
  smbus_read = true; \
  SMB0CN0_STA = true; \
}

// Make the buzzer buzz
#define start_buzzer() {TMR2CN0 |= TMR2CN0_TR2__RUN; }
#define stop_buzzer() {TMR2CN0 &= ~TMR2CN0_TR2__RUN; O_BUZZER = false;}

// Enable charging
#define charge_enable() {P0MDIN &= ~P0MDIN_B3__DIGITAL;}

// Disable charging
#define charge_disable() {P0MDIN |= P0MDIN_B3__DIGITAL; O_DISBL = false;}

// Get the INOKB status
#define has_ext_pwr() (CMP0CN0 & CMP0CN0_CPOUT__POS_GREATER_THAN_NEG)

// Make a beep with the buzzer in ms
#define beep(delay) {start_buzzer(); os_wait2(K_TMO, delay); stop_buzzer();}

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

// Task IDs for RTX51 Tiny
#define TASK_ID_MAIN       0 // Main task that manages the current state
#define TASK_ID_INDICATORS 1 // Task to manage LEDs and the buzzer
#define TASK_ID_SENSORS    2 // Read temperature, CO level and state of charge

// Bits that need to be cleared for the watchdog to refresh
#define WATCHDOG_MASK_INDICATORS (1)
#define WATCHDOG_MASK_SENSORS    (2)
#define WATCHDOG_MASK_TIMER      (4)
#define WATCHDOG_MASK            (WATCHDOG_MASK_INDICATORS | WATCHDOG_MASK_SENSORS | WATCHDOG_MASK_TIMER)

// Calculated levels for battery temperature
#define BAT_TEMP_LOW_ON   TEMP_RES_2_ADC(3.0150) // Disable charging under +5 degree C
#define BAT_TEMP_LOW_OFF  TEMP_RES_2_ADC(2.6385) // Enable charging over +7.5 degree C
#define BAT_TEMP_HIGH_ON  TEMP_RES_2_ADC(0.3723) // Disable charging over +45 degree C
#define BAT_TEMP_HIGH_OFF TEMP_RES_2_ADC(0.4721) // Enable charging under +40 degree C

// Battery charge states
#define BAT_CHARGE_UNKNOWN  (0) // Unknown charge
#define BAT_CHARGE_OK       (1) // Battery charge is okay
#define BAT_CHARGE_WARNING  (2) // You should charge
#define BAT_CHARGE_CRITICAL (3) // Will shutdown soon
#define BAT_CHARGE_EMPTY    (4) // Shutting down
#define BAT_CHARGE_ERROR    (5) // Could not get battery charge

// Battery state of charge thresholds in %
#define BAT_SOC_WARNING  (20) // Level to trigger BAT_CHARGE_WARNING
#define BAT_SOC_CRITICAL (5)  // Level to trigger BAT_CHARGE_CRITICAL
#define BAT_SOC_EMPTY    (2)  // Level to trigger BAT_CHARGE_EMPTY

// Carbonmonoxide states
#define CO_LEVEL_UNKNOWN  (0) // Unknown CO level
#define CO_LEVEL_OK       (1) // No dangerous levels
#define CO_LEVEL_WARNING  (2) // Symptoms may appear (> 50 ppm)
#define CO_LEVEL_CRITICAL (3) // Headaches and fatigue (> 125 ppm)
#define CO_LEVEL_LETHAL   (4) // Life threatening in hours (> 400 ppm)

// Calculated using /tools/mics4515-ref
#define CO_ADC_WARNING  (0x038c)
#define CO_ADC_CRITICAL (0x03c7)
#define CO_ADC_LETHAL   (0x03ea)

// States for the Stratux software running on the Raspberry
// These are detected using pulse length on a GPIO status pin
#define STRATUX_UNKNOWN (0) // Unknown state of the Stratux
#define STRATUX_RUNNING (1) // Stratux is started
#define STRATUX_READY   (2) // Stratux is started and is ready
#define STRATUX_TIMEOUT (3) // Stratux heartbeat has timed out

// I2C fuel gauge constants
#define BQ72441_I2C_ADDRESS (0x55 << 1)
//#define BQ27441_COMMAND_SOC (0x1c) // StateOfCharge()
#define BQ27441_COMMAND_SOC (0x04)

// SMBus constants
#define SMB0CN0_MTSTA (0xe0) // (MT) start transmitted
#define SMB0CN0_MTDB  (0xc0) // (MT) data byte transmitted
#define SMB0CN0_MRDB  (0x80) // (MT) data byte received

//-----------------------------------------------------------------------------
// Stratux state machine states
//-----------------------------------------------------------------------------

// Initial state
// Read current battery level
// -> STATE_SHUTDOWN if battery ok
// -> STATE_POWEROFF if battery not ok
#define STATE_BOOT (0)

// Start the Stratux
// Toggle RUN/RESET
// -> STATE_STARTING
#define STATE_START (1)
#define STATE_START_GRACE (2)

// Stratux starting up
// Read I_PIRDY
// -> STATE_RUNNING if I_PIRDY goes low
// -> STATE_START if timeout (28s)
#define STATE_STARTING (3)

// Boot has timed out
// -> STATE_START
#define STATE_START_TIMEOUT (4)

// Stratux running
// Read I_PIRDY and power switch
// -> STATE_READY
// -> STATE_START on timeout
// -> STATE_STOP on power off or battery critical
#define STATE_RUNNING (5)

// Stratux running and ready
// Read I_PIRDY and power switch
// -> STATE_RUNNING
// -> STATE_START on timeout
// -> STATE_STOP on power off or battery critical
#define STATE_READY (6)

// Initiate stop of Stratux
// Set O_PISHDN
// -> STATE_STOPPING
#define STATE_STOP (7)

// Stratux shutting down
// Wait 12s
// -> STATE_SHUTDOWN
#define STATE_STOPPING (8)

// Stratux shut down
// Read power switch
// -> STATE_START if power on
// -> STATE_POWEROFF if low battery
#define STATE_SHUTDOWN (9)

// Stratux shutdown because of low battery
// -> STATE_SHUTDOWN if battery ok
#define STATE_POWEROFF (10)

//-----------------------------------------------------------------------------
// I/O
//-----------------------------------------------------------------------------

// Low if external power is connected (must not have PU, use comparator)
SI_SBIT(I_INOKB, SFR_P0, 2);
// Set low to disable charging (open collector, must not have PU, use as analog input until time)
SI_SBIT(O_DISBL, SFR_P0, 3);
// Set low to send shutdown signal to the Raspberry (open collector, PU)
SI_SBIT(O_PISHDN, SFR_P0, 4);
// Toggle low to wake up fuel gauge (open collector, PU)
SI_SBIT(O_GPOUT, SFR_P0, 6);
// Low if power switch in shutdown mode (PU)
SI_SBIT(I_SHDN, SFR_P0, 7);
// Set high to keep power enabled (PU)
SI_SBIT(O_EN, SFR_P1, 0);
// Set low to power up the Raspberry (open collector, PU)
SI_SBIT(O_RST, SFR_P1, 1);
// Set high to pass current through the buzzer (PP)
SI_SBIT(O_BUZZER, SFR_P1, 2);
// Low pulses when the Raspberry is ready (PU)
SI_SBIT(I_PIRDY, SFR_P2, 0);

// SMBus pins for bit bang
SI_SBIT(IO_SDA, SFR_P0, 0);
SI_SBIT(O_SCL, SFR_P0, 1);

// Low to turn LEDs on (open collector)
SI_SBIT(LED_POWER, SFR_P1, 3);
SI_SBIT(LED_READY, SFR_P1, 4);
SI_SBIT(LED_ALARM, SFR_P1, 5);

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

// Flags
extern bit bat_temp_ok;

// Watchdog flags for the tasks
extern uint8_t data watchdog_mask;

// Current state for the Stratux state machine
extern uint8_t data stratux_state;

// Current status of the Stratux
extern uint8_t data stratux_status;

// State of charge
extern uint8_t data bat_charge;

// Detected CO level
extern uint8_t data co_level;

// Overflows every 8000 ms, used for state machine and LED flashing
extern uint8_t data interval;
// Counts up 2 times per second
extern uint8_t data main_countdown;
// Counts down 32 times per second
extern uint8_t data status_countup;

// State for the SMBus interrupt routine
extern bit smbus_read;
extern bit smbus_error;
extern bit smbus_busy;
extern uint8_t data smbus_cmd;
extern uint8_t data smbus_data0;
extern uint8_t data smbus_data1;

#endif /* INC_DEVICECONFIG_H_ */
