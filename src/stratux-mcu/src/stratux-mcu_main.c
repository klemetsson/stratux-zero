//=========================================================
// src/stratux-mcu_main.c: generated by Hardware Configurator
//
// This file will be updated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <rtx51tny.h>
#include "InitDevice.h"
#include "DeviceConfig.h"

// $[Generated Includes]
// [Generated Includes]$

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------

void SiLabs_Startup(void) {
  // $[SiLabs Startup]
  // [SiLabs Startup]$
}

//-----------------------------------------------------------------------------
// Macro functions
// ----------------------------------------------------------------------------

// Make the buzzer buzz
#define start_buzzer() {TMR2CN0 |= TMR2CN0_TR2__RUN;}

// Stop the buzzer buzzing
#define stop_buzzer() {TMR2CN0 &= ~TMR2CN0_TR2__RUN; O_BUZZER = false;}

// Enable charging
#define charge_enable() {P0MDIN &= ~P0MDIN_B3__DIGITAL;}

// Disable charging
#define charge_disable() {P0MDIN |= P0MDIN_B3__DIGITAL; O_DISBL = false;}

// Get the INOKB status
#define has_ext_pwr() (CMP0CN0 & CMP0CN0_CPOUT__POS_GREATER_THAN_NEG)

//-----------------------------------------------------------------------------
// Main task
// ----------------------------------------------------------------------------

uint8_t data stratux_state = STATE_BOOT;

void task_main(void) _task_ TASK_ID_MAIN {
  uint8_t timestamp;

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  // Wake the fuel gauge
  O_GPOUT = false;
  os_wait2(K_TMO, 1);
  O_GPOUT = true;

  // Start the other tasks
  os_create_task(TASK_ID_INDICATORS);
  os_create_task(TASK_ID_SENSORS);

  while (true) {
	  switch (stratux_state) {
	  case STATE_BOOT:
		  if (bat_charge == BAT_CHARGE_EMPTY) {
			  stratux_state = STATE_POWEROFF;
		  }
		  else if (bat_charge != BAT_CHARGE_UNKNOWN) {
			  if (I_SHDN) {
				  stratux_state = STATE_START;
				  // Make a 500 ms beep
				  start_buzzer();
				  os_wait2(K_TMO, 50);
				  stop_buzzer();
			  }
			  else {
				  stratux_state = STATE_SHUTDOWN;
			  }
			  continue;
		  }
		  break;
	  case STATE_START:
          O_EN = true;
          O_RST = true;
          O_PISHDN = true;
          os_wait2(K_TMO, 5);
          O_RST = false;
          stratux_state = STATE_STARTING1;
          timestamp = interval;
		  break;
	  case STATE_STARTING1:
	  case STATE_STARTING2:
	  case STATE_STARTING3:
	  case STATE_STARTING4:
	  case STATE_STARTING5:
		  if (stratux_status == STRATUX_RUNNING) {
			  stratux_state = STATE_RUNNING;
		  }
		  else if (stratux_status == STRATUX_READY) {
			  stratux_state = STATE_READY;
		  }
		  // 4 seconds has passed
		  else if ((interval - timestamp) > 128) {
			  timestamp = interval;
			  stratux_state++;
		  }
		  break;
	  case STATE_START_TIMEOUT:
		  stratux_state = STATE_START;
		  continue;
	  case STATE_RUNNING:
	  case STATE_READY:
		  if (bat_charge == BAT_CHARGE_EMPTY) {
			  stratux_state = STATE_STOP;
			  continue;
		  }
		  else if (stratux_status == STRATUX_RUNNING) {
			  stratux_state = STATE_RUNNING;
		  }
		  else if (stratux_status == STRATUX_READY) {
			  stratux_state = STATE_READY;
		  }
		  else if (stratux_status == STRATUX_TIMEOUT) {
			  stratux_state = STATE_STOP;
			  continue;
		  }
		  break;
	  case STATE_STOP:
		  O_PISHDN = false;
		  stratux_state = STATE_STOPPING1;
		  timestamp = interval;
		  break;
	  case STATE_STOPPING1:
	  case STATE_STOPPING2:
	  case STATE_STOPPING3:
		  // 4 seconds has passed
		  if ((interval - timestamp) > 128) {
			  timestamp = interval;
			  stratux_state++;
		  }
		  break;
	  case STATE_SHUTDOWN:
		  O_RST = true;
		  O_PISHDN = true;
		  if (bat_charge == BAT_CHARGE_EMPTY) {
			  stratux_state = STATE_POWEROFF;
		  }
		  else if (I_SHDN) {
			  stratux_state = STATE_BOOT;
			  continue;
		  }
		  else O_EN = false;
		  break;
	  case STATE_POWEROFF:
		  O_RST = true;
		  O_PISHDN = true;
		  if (bat_charge != BAT_CHARGE_EMPTY) {
			  stratux_state = STATE_SHUTDOWN;
			  continue;
		  }
		  else O_EN = false;
		  break;
	  default:
		  stratux_state = STATE_BOOT;
	  }

	  // Reset watchdog
	  WDTCN = 0xA5;
	  // Switch to next task
	  os_switch_task();
  }                             

}

//-----------------------------------------------------------------------------
// Task for managing all the indicators
// ----------------------------------------------------------------------------

uint8_t data stratux_status = STRATUX_UNKNOWN;

void task_indicator(void) _task_ TASK_ID_INDICATORS {
	uint8_t offset, local_interval, timestamp;
	bit is_on, pi_ready;

	is_on = false;
	pi_ready = I_PIRDY;
	timestamp = interval;

	while (true) {

		// Handle LEDs and the buzzer
		if (I_SHDN && stratux_state != STATE_POWEROFF) {
			// Sync blinking with the power on event
			if (!is_on) {
				is_on = true;
				offset = interval;
			}
			local_interval = interval - offset;

			// Power LED
			if (bat_charge == BAT_CHARGE_CRITICAL) {
				// 500 ms on, 500 ms off
				LED_POWER = (bit)(local_interval & 0x10);
			}
			else if (bat_charge == BAT_CHARGE_WARNING) {
				// 3500 ms on, 500 ms off
				LED_POWER = (local_interval & 0x70) == 0x70;
			}
			else {
				LED_POWER = false;
			}

			// CO alert LED
			if (co_level == CO_LEVEL_LETHAL) {
				// 500 ms on, 500 ms off
				LED_ALARM = (bit)(local_interval & 0x10);
			}
			else if (co_level == CO_LEVEL_CRITICAL) {
				// 500 ms on, 3500 ms off
				LED_ALARM = (local_interval & 0x70) != 0x00;
			}
			else if (co_level == CO_LEVEL_WARNING) {
				// 500 ms on, 7500 ms off
				LED_ALARM = (local_interval & 0xf0) != 0x00;
			}
			else {
				LED_ALARM = true;
			}

			// Stratux ready LED
			if (stratux_state == STATE_READY)
				LED_READY = true;
			else if (stratux_state == STATE_RUNNING)
				// 1500 ms on, 500 ms off
				LED_READY = (local_interval & 0x30) == 0x30;
			else
				// 500 ms on, 500 ms off
				LED_READY = (bit)(local_interval & 0x10);

		}
		else {
			is_on = false;
			LED_POWER = true;
			LED_READY = true;
			LED_ALARM = true;
			stop_buzzer();
		}

		// Disable charging if temperature out of range
		if (bat_temp_ok || !has_ext_pwr()) {
			charge_enable();
		}
		else {
			charge_disable();
		}

		// Detect Stratux heartbeat
		if (pi_ready != I_PIRDY) {
			pi_ready = I_PIRDY;
			// Raising edge
			if (pi_ready) {
				timestamp = interval;
			}
			// Falling edge
			else {
				// Pulse > 350 ms, 200 ms nominal is Stratux running and ready
				if ((interval - timestamp) > 11) {
					stratux_status = STRATUX_READY;
				}
				// Pulse < 350 ms, 500 ms nominal is Stratux running but waiting for GNSS
				else {
					stratux_status = STRATUX_RUNNING;
				}
				timestamp = interval;
			}
		}
		// Timeout 2 s
		else if ((interval - timestamp) > 64) {
			timestamp = interval;
			stratux_status = STRATUX_TIMEOUT;
		}

		// Switch to next task
		os_switch_task();
	}
}

//-----------------------------------------------------------------------------
// Task for reading sensor values
// ----------------------------------------------------------------------------

bit bat_temp_ok = true;
uint8_t data bat_charge = BAT_CHARGE_UNKNOWN;
uint8_t data co_level = CO_LEVEL_UNKNOWN;

void task_sensors(void) _task_ TASK_ID_SENSORS {
	uint16_t sample;

	while (true) {
		// Read battery temperature and see if it is safe to charge
		ADC0MX = 0x05; // P0.5
		ADC0 = 0x00;
		os_clear_signal(TASK_ID_SENSORS);
		ADC0CN0_ADBUSY = true;
		os_wait1(SIG_EVENT);
		sample = ADC0;
		bat_temp_ok = bat_temp_ok
				? sample >= BAT_TEMP_HIGH_ON && sample <= BAT_TEMP_LOW_ON
			    : sample <= BAT_TEMP_HIGH_OFF && sample <= BAT_TEMP_LOW_OFF;

		// Read CO level
		ADC0MX = 0x0e; // P1.6
		ADC0 = 0x00;
		os_clear_signal(TASK_ID_SENSORS);
		ADC0CN0_ADBUSY = true;
		os_wait1(SIG_EVENT);
		sample = ADC0;
		if (sample >= CO_ADC_LETHAL)
			co_level = CO_LEVEL_LETHAL;
		else if (sample >= CO_ADC_CRITICAL)
			co_level = CO_LEVEL_CRITICAL;
		else if (sample >= CO_ADC_WARNING)
			co_level = CO_LEVEL_WARNING;
		else
			co_level = CO_LEVEL_OK;

		// Read fuel gauge state of charge
		bat_charge = BAT_CHARGE_OK;

		// Pause the task for 1000 ms
		os_wait2(K_IVL, 100);
	}
}
