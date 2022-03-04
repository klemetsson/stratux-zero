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

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

#define TASK_ID_MAIN       0 // Main task that manages the current state
#define TASK_ID_INDICATORS 1 // Task to manage LEDs and the buzzer
#define TASK_ID_SENSORS    2 // Read temperature, CO level and state of charge

#define BAT_TEMP_LOW_ON   TEMP_RES_2_ADC(3.0150) // Disable charging under +5 degree C
#define BAT_TEMP_LOW_OFF  TEMP_RES_2_ADC(2.6385) // Enable charging over +7.5 degree C
#define BAT_TEMP_HIGH_ON  TEMP_RES_2_ADC(0.3723) // Disable charging over +45 degree C
#define BAT_TEMP_HIGH_OFF TEMP_RES_2_ADC(0.4721) // Enable charging under +40 degree C

#define BAT_CHARGE_UNKNOWN  (0) // Unknown charge
#define BAT_CHARGE_OK       (1) // Battery charge is okay
#define BAT_CHARGE_WARNING  (2) // You should charge
#define BAT_CHARGE_CRITICAL (3) // Will shutdown soon
#define BAT_CHARGE_EMPTY    (4) // Shutting down

#define CO_LEVEL_UNKNOWN  (0) // Unknown CO level
#define CO_LEVEL_OK       (1) // No dangerous levels
#define CO_LEVEL_WARNING  (2) // Symptoms may appear (> 50 ppm)
#define CO_LEVEL_CRITICAL (3) // Headaches and fatigue (> 125 ppm)
#define CO_LEVEL_LETHAL   (4) // Life threatening in hours (> 400 ppm)

// Calculated using /tools/mics4515-ref
#define CO_ADC_WARNING  (0x038c)
#define CO_ADC_CRITICAL (0x03c7)
#define CO_ADC_LETHAL   (0x03ea)

#define STRATUX_UNKNOWN (0) // Unknown state of the Stratux
#define STRATUX_RUNNING (1) // Stratux is started
#define STRATUX_READY   (2) // Stratux is started and is ready
#define STRATUX_TIMEOUT (3) // Stratux heartbeat has timed out

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

// Stratux starting up
// Read I_PIRDY
// -> STATE_RUNNING if I_PIRDY goes low
// -> STATE_START if timeout (20s)
#define STATE_STARTING1 (2)
#define STATE_STARTING2 (3)
#define STATE_STARTING3 (4)
#define STATE_STARTING4 (5)
#define STATE_STARTING5 (6)

// Boot has timed out
// -> STATE_START
#define STATE_START_TIMEOUT (7)

// Stratux running
// Read I_PIRDY and power switch
// -> STATE_READY
// -> STATE_START on timeout
// -> STATE_STOP on power off or battery critical
#define STATE_RUNNING (8)

// Stratux running and ready
// Read I_PIRDY and power switch
// -> STATE_RUNNING
// -> STATE_START on timeout
// -> STATE_STOP on power off or battery critical
#define STATE_READY (9)

// Initiate stop of Stratux
// Set O_PISHDN
// -> STATE_STOPPING
#define STATE_STOP (10)

// Stratux shutting down
// Wait 12s
// -> STATE_SHUTDOWN
#define STATE_STOPPING1 (11)
#define STATE_STOPPING2 (12)
#define STATE_STOPPING3 (13)

// Stratux shut down
// Read power switch
// -> STATE_START if power on
// -> STATE_POWEROFF if low battery
#define STATE_SHUTDOWN (14)

// Stratux shutdown because of low battery
// -> STATE_SHUTDOWN if battery ok
#define STATE_POWEROFF (15)

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

// Low to turn LEDs on (open collector)
SI_SBIT(LED_POWER, SFR_P1, 3);
SI_SBIT(LED_READY, SFR_P1, 4);
SI_SBIT(LED_ALARM, SFR_P1, 5);

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

// Flags
extern bit bat_temp_ok;

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

// State machine state for SMBus
extern uint8_t data smbus_state;

#endif /* INC_DEVICECONFIG_H_ */
