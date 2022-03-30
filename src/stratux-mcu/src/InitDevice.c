//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void
enter_DefaultMode_from_RESET (void)
{
  // $[Config Calls]
  WDT_0_enter_DefaultMode_from_RESET ();
  PORTS_0_enter_DefaultMode_from_RESET ();
  PORTS_1_enter_DefaultMode_from_RESET ();
  PBCFG_0_enter_DefaultMode_from_RESET ();
  ADC_0_enter_DefaultMode_from_RESET ();
  CMP_0_enter_DefaultMode_from_RESET ();
  VDDMON_0_enter_DefaultMode_from_RESET ();
  CLOCK_0_enter_DefaultMode_from_RESET ();
  TIMER01_0_enter_DefaultMode_from_RESET ();
  TIMER16_2_enter_DefaultMode_from_RESET ();
  TIMER16_3_enter_DefaultMode_from_RESET ();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET ();
  INTERRUPT_0_enter_DefaultMode_from_RESET ();
  // [Config Calls]$

}

extern void
PBCFG_0_enter_DefaultMode_from_RESET (void)
{
  // $[XBR2 - Port I/O Crossbar 2]
  /***********************************************************************
   - Weak Pullups enabled 
   - Crossbar enabled
   ***********************************************************************/
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
  // [XBR2 - Port I/O Crossbar 2]$

  // $[PRTDRV - Port Drive Strength]
  // [PRTDRV - Port Drive Strength]$

  // $[XBR0 - Port I/O Crossbar 0]
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

}

extern void
SMBUS_0_enter_DefaultMode_from_RESET (void)
{
  // $[SMB0ADR - SMBus 0 Slave Address]
  // [SMB0ADR - SMBus 0 Slave Address]$

  // $[SMB0ADM - SMBus 0 Slave Address Mask]
  // [SMB0ADM - SMBus 0 Slave Address Mask]$

  // $[SMB0TC - SMBus 0 Timing and Pin Control]
  // [SMB0TC - SMBus 0 Timing and Pin Control]$

  // $[SMB0CF - SMBus 0 Configuration]
  /***********************************************************************
   - Timer 1 Overflow
   - Enable the SMBus module
   ***********************************************************************/
  SMB0CF &= ~SMB0CF_SMBCS__FMASK;
  SMB0CF |= SMB0CF_SMBCS__TIMER1 | SMB0CF_ENSMB__ENABLED;
  // [SMB0CF - SMBus 0 Configuration]$

}

extern void
TIMER16_2_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR2CN0_TR2_save;
  TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
  // Stop Timer
  TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
  // [Timer Initialization]$

  // $[TMR2CN0 - Timer 2 Control]
  // [TMR2CN0 - Timer 2 Control]$

  // $[TMR2H - Timer 2 High Byte]
  // [TMR2H - Timer 2 High Byte]$

  // $[TMR2L - Timer 2 Low Byte]
  // [TMR2L - Timer 2 Low Byte]$

  // $[TMR2RLH - Timer 2 Reload High Byte]
  /***********************************************************************
   - Timer 2 Reload High Byte = 0xFE
   ***********************************************************************/
  TMR2RLH = (0xFE << TMR2RLH_TMR2RLH__SHIFT);
  // [TMR2RLH - Timer 2 Reload High Byte]$

  // $[TMR2RLL - Timer 2 Reload Low Byte]
  /***********************************************************************
   - Timer 2 Reload Low Byte = 0x8A
   ***********************************************************************/
  TMR2RLL = (0x8A << TMR2RLL_TMR2RLL__SHIFT);
  // [TMR2RLL - Timer 2 Reload Low Byte]$

  // $[TMR2CN0]
  // [TMR2CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR2CN0 |= TMR2CN0_TR2_save;
  // [Timer Restoration]$

}

extern void
PORTS_0_enter_DefaultMode_from_RESET (void)
{
  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  /***********************************************************************
   - P0.0 pin is configured for digital mode
   - P0.1 pin is configured for digital mode
   - P0.2 pin is configured for analog mode
   - P0.3 pin is configured for analog mode
   - P0.4 pin is configured for digital mode
   - P0.5 pin is configured for analog mode
   - P0.6 pin is configured for digital mode
   - P0.7 pin is configured for digital mode
   ***********************************************************************/
  P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__ANALOG
      | P0MDIN_B3__ANALOG | P0MDIN_B4__DIGITAL | P0MDIN_B5__ANALOG
      | P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /***********************************************************************
   - P0.0 pin is skipped by the crossbar
   - P0.1 pin is skipped by the crossbar
   - P0.2 pin is skipped by the crossbar
   - P0.3 pin is skipped by the crossbar
   - P0.4 pin is skipped by the crossbar
   - P0.5 pin is skipped by the crossbar
   - P0.6 pin is skipped by the crossbar
   - P0.7 pin is skipped by the crossbar
   ***********************************************************************/
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
      | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
      | P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

}

extern void
WDT_0_enter_DefaultMode_from_RESET (void)
{
  // $[Watchdog Timer Init Variable Declarations]
  uint32_t i;
  bool ea;
  // [Watchdog Timer Init Variable Declarations]$

  // $[WDTCN - Watchdog Timer Control]
  // Deprecated
  // [WDTCN - Watchdog Timer Control]$

  // $[WDTCN_2 - Watchdog Timer Control]

  // Feed WDT timer before disabling (Erratum WDT_E102)
  WDTCN = 0xA5;

  // Add 2 LFO cycle delay before disabling WDT (Erratum WDT_E102)
  for (i = 0; i < (2 * 3062500UL) / (10000 * 3); i++)
    {
      NOP ();
    }

  // Disable WDT before changing interval
  ea = IE_EA;
  IE_EA = 0;
  WDTCN = 0xDE;
  WDTCN = 0xAD;
  IE_EA = ea;

  // Delay 2 LFO cycles to ensure WDT is completely disabled
  for (i = 0; i < (2 * 3062500UL) / (10000 * 3); i++)
    {
      NOP ();
    }

  // Change WDT interval
  WDTCN = 4;

  // Start WDT
  WDTCN = 0xA5;

  // [WDTCN_2 - Watchdog Timer Control]$

}

extern void
PORTS_1_enter_DefaultMode_from_RESET (void)
{
  // $[P1 - Port 1 Pin Latch]
  /***********************************************************************
   - P1.0 is low. Set P1.0 to drive low
   - P1.1 is high. Set P1.1 to drive or float high
   - P1.2 is low. Set P1.2 to drive low
   - P1.3 is high. Set P1.3 to drive or float high
   - P1.4 is high. Set P1.4 to drive or float high
   - P1.5 is high. Set P1.5 to drive or float high
   - P1.6 is high. Set P1.6 to drive or float high
   ***********************************************************************/
  P1 = P1_B0__LOW | P1_B1__HIGH | P1_B2__LOW | P1_B3__HIGH | P1_B4__HIGH
      | P1_B5__HIGH | P1_B6__HIGH;
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /***********************************************************************
   - P1.0 output is push-pull
   - P1.1 output is open-drain
   - P1.2 output is push-pull
   - P1.3 output is open-drain
   - P1.4 output is open-drain
   - P1.5 output is open-drain
   - P1.6 output is open-drain
   ***********************************************************************/
  P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__PUSH_PULL | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN
      | P1MDOUT_B5__OPEN_DRAIN | P1MDOUT_B6__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  /***********************************************************************
   - P1.0 pin is configured for digital mode
   - P1.1 pin is configured for digital mode
   - P1.2 pin is configured for digital mode
   - P1.3 pin is configured for digital mode
   - P1.4 pin is configured for digital mode
   - P1.5 pin is configured for digital mode
   - P1.6 pin is configured for analog mode
   ***********************************************************************/
  P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__ANALOG;
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /***********************************************************************
   - P1.0 pin is skipped by the crossbar
   - P1.1 pin is skipped by the crossbar
   - P1.2 pin is skipped by the crossbar
   - P1.3 pin is skipped by the crossbar
   - P1.4 pin is skipped by the crossbar
   - P1.5 pin is skipped by the crossbar
   - P1.6 pin is skipped by the crossbar
   ***********************************************************************/
  P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
      | P1SKIP_B3__SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED
      | P1SKIP_B6__SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

}

extern void
ADC_0_enter_DefaultMode_from_RESET (void)
{
  // $[ADC0CN1 - ADC0 Control 1]
  // [ADC0CN1 - ADC0 Control 1]$

  // $[ADC0MX - ADC0 Multiplexer Selection]
  // [ADC0MX - ADC0 Multiplexer Selection]$

  // $[ADC0CF - ADC0 Configuration]
  // [ADC0CF - ADC0 Configuration]$

  // $[ADC0AC - ADC0 Accumulator Configuration]
  /***********************************************************************
   - Right justified. Shifted right by 3 bits
   - Disable 12-bit mode
   - ADC0H:ADC0L contain the result of the latest conversion when Burst
   Mode is disabled
   - Perform and Accumulate 8 conversions 
   ***********************************************************************/
  ADC0AC = ADC0AC_ADSJST__RIGHT_SHIFT_3 | ADC0AC_AD12BE__12_BIT_DISABLED
      | ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_8;
  // [ADC0AC - ADC0 Accumulator Configuration]$

  // $[ADC0TK - ADC0 Burst Mode Track Time]
  // [ADC0TK - ADC0 Burst Mode Track Time]$

  // $[ADC0PWR - ADC0 Power Control]
  /***********************************************************************
   - Burst Mode Power Up Time = 0x0F
   - Disable low power mode
   - Low power mode disabled
   - Select bias current mode 1 
   ***********************************************************************/
  ADC0PWR = (0x0F << ADC0PWR_ADPWR__SHIFT) | ADC0PWR_ADLPM__LP_BUFFER_DISABLED
      | ADC0PWR_ADMXLP__LP_MUX_VREF_DISABLED | ADC0PWR_ADBIAS__MODE1;
  // [ADC0PWR - ADC0 Power Control]$

  // $[ADC0GTH - ADC0 Greater-Than High Byte]
  // [ADC0GTH - ADC0 Greater-Than High Byte]$

  // $[ADC0GTL - ADC0 Greater-Than Low Byte]
  // [ADC0GTL - ADC0 Greater-Than Low Byte]$

  // $[ADC0LTH - ADC0 Less-Than High Byte]
  // [ADC0LTH - ADC0 Less-Than High Byte]$

  // $[ADC0LTL - ADC0 Less-Than Low Byte]
  // [ADC0LTL - ADC0 Less-Than Low Byte]$

  // $[ADC0CN0 - ADC0 Control 0]
  /***********************************************************************
   - Enable ADC0 
   - Enable ADC0 burst mode
   ***********************************************************************/
  ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADBMEN__BURST_ENABLED;
  // [ADC0CN0 - ADC0 Control 0]$

}

extern void
TIMER_SETUP_0_enter_DefaultMode_from_RESET (void)
{
  // $[CKCON0 - Clock Control 0]
  // [CKCON0 - Clock Control 0]$

  // $[TMOD - Timer 0/1 Mode]
  /***********************************************************************
   - Mode 0, 13-bit Counter/Timer
   - Mode 2, 8-bit Counter/Timer with Auto-Reload
   - Timer Mode
   - Timer 0 enabled when TR0 = 1 irrespective of INT0 logic level
   - Timer Mode
   - Timer 1 enabled when TR1 = 1 irrespective of INT1 logic level
   ***********************************************************************/
  TMOD = TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
      | TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  /***********************************************************************
   - Start Timer 1 running
   ***********************************************************************/
  TCON |= TCON_TR1__RUN;
  // [TCON - Timer 0/1 Control]$

}

extern void
EXTINT_0_enter_DefaultMode_from_RESET (void)
{
  // $[IT01CF - INT0/INT1 Configuration]
  /***********************************************************************
   - INT0 input is active low
   - Select P0.7
   - INT1 input is active low
   - Select P0.2
   ***********************************************************************/
  IT01CF = IT01CF_IN0PL__ACTIVE_LOW | IT01CF_IN0SL__P0_7
      | IT01CF_IN1PL__ACTIVE_LOW | IT01CF_IN1SL__P0_2;
  // [IT01CF - INT0/INT1 Configuration]$

}

extern void
TIMER01_0_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  /***********************************************************************
   - Timer 1 High Byte = 0xEF
   ***********************************************************************/
  TH1 = (0xEF << TH1_TH1__SHIFT);
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

extern void
RSTSRC_0_enter_DefaultMode_from_RESET (void)
{
  // $[RSTSRC - Reset Source]
  /***********************************************************************
   - A power-on or supply monitor reset occurred
   - A missing clock detector reset did not occur
   - A Comparator 0 reset did not occur
   ***********************************************************************/
  RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__NOT_SET | RSTSRC_C0RSEF__NOT_SET;
  // [RSTSRC - Reset Source]$

}

extern void
INTERRUPT_0_enter_DefaultMode_from_RESET (void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  /***********************************************************************
   - Enable interrupt requests generated by the ADINT flag
   - Disable ADC0 Window Comparison interrupt
   - Disable CP0 interrupts
   - Disable CP1 interrupts
   - Disable all Port Match interrupts
   - Disable all PCA0 interrupts
   - Disable all SMB0 interrupts
   - Enable interrupt requests generated by the TF3L or TF3H flags
   ***********************************************************************/
  EIE1 = EIE1_EADC0__ENABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
      | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
      | EIE1_ESMB0__DISABLED | EIE1_ET3__ENABLED;
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIP1 - Extended Interrupt Priority 1]
  // [EIP1 - Extended Interrupt Priority 1]$

  // $[IE - Interrupt Enable]
  /***********************************************************************
   - Enable each interrupt according to its individual mask setting
   - Disable external interrupt 0
   - Disable external interrupt 1
   - Disable all SPI0 interrupts
   - Disable all Timer 0 interrupt
   - Disable all Timer 1 interrupt
   - Enable interrupt requests generated by the TF2L or TF2H flags
   - Disable UART0 interrupt
   ***********************************************************************/
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

}

extern void
CMP_0_enter_DefaultMode_from_RESET (void)
{
  // $[CMP0MX - Comparator 0 Multiplexer Selection]
  /***********************************************************************
   - External pin CMP0P.8
   - External pin CMP0N.2
   ***********************************************************************/
  uint8_t delay;

  CMP0MX = CMP0MX_CMXP__LDO_OUT | CMP0MX_CMXN__CMP0N2;
  // Delay to allow the comparator inputs to settle
  for (delay = 0; delay < 0x20; delay++)
    ;
  // [CMP0MX - Comparator 0 Multiplexer Selection]$

  // $[CMP0MD - Comparator 0 Mode]
  // [CMP0MD - Comparator 0 Mode]$

  // $[CMP0CN0 - Comparator 0 Control 0]
  /***********************************************************************
   - Comparator enabled
   - Positive Hysteresis = Hysteresis 3 
   - Negative Hysteresis = Hysteresis 3 
   ***********************************************************************/
  CMP0CN0 |= CMP0CN0_CPEN__ENABLED | CMP0CN0_CPHYP__ENABLED_MODE3
      | CMP0CN0_CPHYN__ENABLED_MODE3;
  // [CMP0CN0 - Comparator 0 Control 0]$

}

extern void
TIMER16_3_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR3CN0_TR3_save;
  TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
  // Stop Timer
  TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
  // [Timer Initialization]$

  // $[TMR3CN0 - Timer 3 Control]
  // [TMR3CN0 - Timer 3 Control]$

  // $[TMR3H - Timer 3 High Byte]
  // [TMR3H - Timer 3 High Byte]$

  // $[TMR3L - Timer 3 Low Byte]
  // [TMR3L - Timer 3 Low Byte]$

  // $[TMR3RLH - Timer 3 Reload High Byte]
  // [TMR3RLH - Timer 3 Reload High Byte]$

  // $[TMR3RLL - Timer 3 Reload Low Byte]
  /***********************************************************************
   - Timer 3 Reload Low Byte = 0xCB
   ***********************************************************************/
  TMR3RLL = (0xCB << TMR3RLL_TMR3RLL__SHIFT);
  // [TMR3RLL - Timer 3 Reload Low Byte]$

  // $[TMR3CN0]
  /***********************************************************************
   - Start Timer 3 running
   ***********************************************************************/
  TMR3CN0 |= TMR3CN0_TR3__RUN;
  // [TMR3CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR3CN0 |= TMR3CN0_TR3_save;
  // [Timer Restoration]$

}

extern void
CLOCK_0_enter_DefaultMode_from_RESET (void)
{
  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - Clock derived from the Internal High-Frequency Oscillator
   - SYSCLK is equal to selected clock source divided by 1
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  // [CLKSEL - Clock Select]$

}

extern void
VDDMON_0_enter_DefaultMode_from_RESET (void)
{

  // $[VDM0CN - Supply Monitor Control]
  /***********************************************************************
   - Supply Monitor Disabled
   ***********************************************************************/
  VDM0CN &= ~VDM0CN_VDMEN__BMASK;
  // [VDM0CN - Supply Monitor Control]$

}

extern void
enter_ReadyMode_from_DefaultMode (void)
{
  // $[Config Calls]
  PORTS_0_enter_ReadyMode_from_DefaultMode ();
  PBCFG_0_enter_ReadyMode_from_DefaultMode ();
  TIMER01_0_enter_ReadyMode_from_DefaultMode ();
  SMBUS_0_enter_ReadyMode_from_DefaultMode ();
  INTERRUPT_0_enter_ReadyMode_from_DefaultMode ();
  // [Config Calls]$

}

extern void
PORTS_0_enter_ReadyMode_from_DefaultMode (void)
{
  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /***********************************************************************
   - P0.0 pin is not skipped by the crossbar
   - P0.1 pin is not skipped by the crossbar
   - P0.2 pin is skipped by the crossbar
   - P0.3 pin is skipped by the crossbar
   - P0.4 pin is skipped by the crossbar
   - P0.5 pin is skipped by the crossbar
   - P0.6 pin is skipped by the crossbar
   - P0.7 pin is skipped by the crossbar
   ***********************************************************************/
  P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__SKIPPED
      | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
      | P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

}

extern void
PBCFG_0_enter_ReadyMode_from_DefaultMode (void)
{
  // $[XBR2 - Port I/O Crossbar 2]
  // [XBR2 - Port I/O Crossbar 2]$

  // $[PRTDRV - Port Drive Strength]
  // [PRTDRV - Port Drive Strength]$

  // $[XBR0 - Port I/O Crossbar 0]
  /***********************************************************************
   - UART I/O unavailable at Port pin
   - SPI I/O unavailable at Port pins
   - SMBus 0 I/O routed to Port pins
   - CP0 unavailable at Port pin
   - Asynchronous CP0 unavailable at Port pin
   - CP1 unavailable at Port pin
   - Asynchronous CP1 unavailable at Port pin
   - SYSCLK unavailable at Port pin
   ***********************************************************************/
  XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__ENABLED
      | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
      | XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

}

extern void
SMBUS_0_enter_ReadyMode_from_DefaultMode (void)
{
  // $[SMB0ADR - SMBus 0 Slave Address]
  // [SMB0ADR - SMBus 0 Slave Address]$

  // $[SMB0ADM - SMBus 0 Slave Address Mask]
  // [SMB0ADM - SMBus 0 Slave Address Mask]$

  // $[SMB0TC - SMBus 0 Timing and Pin Control]
  // [SMB0TC - SMBus 0 Timing and Pin Control]$

  // $[SMB0CF - SMBus 0 Configuration]
  /***********************************************************************
   - Enable the SMBus module
   ***********************************************************************/
  SMB0CF |= SMB0CF_ENSMB__ENABLED;
  // [SMB0CF - SMBus 0 Configuration]$

}

extern void
INTERRUPT_0_enter_ReadyMode_from_DefaultMode (void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  /***********************************************************************
   - Enable interrupt requests generated by the ADINT flag
   - Disable ADC0 Window Comparison interrupt
   - Disable CP0 interrupts
   - Disable CP1 interrupts
   - Disable all Port Match interrupts
   - Disable all PCA0 interrupts
   - Enable interrupt requests generated by SMB0
   - Enable interrupt requests generated by the TF3L or TF3H flags
   ***********************************************************************/
  EIE1 = EIE1_EADC0__ENABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
      | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
      | EIE1_ESMB0__ENABLED | EIE1_ET3__ENABLED;
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIP1 - Extended Interrupt Priority 1]
  // [EIP1 - Extended Interrupt Priority 1]$

  // $[IE - Interrupt Enable]
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

}

extern void
TIMER01_0_enter_ReadyMode_from_DefaultMode (void)
{
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  /***********************************************************************
   - Timer 1 High Byte = 0xFA
   ***********************************************************************/
  TH1 = (0xFA << TH1_TH1__SHIFT);
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

