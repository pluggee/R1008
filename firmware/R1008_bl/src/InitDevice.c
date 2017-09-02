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
extern void enter_DefaultMode_from_RESET(void) {
    // $[Config Calls]
    WDT_0_enter_DefaultMode_from_RESET();
    PORTS_0_enter_DefaultMode_from_RESET();
    PBCFG_0_enter_DefaultMode_from_RESET();
    CLOCK_0_enter_DefaultMode_from_RESET();
    TIMER01_0_enter_DefaultMode_from_RESET();
    TIMER16_2_enter_DefaultMode_from_RESET();
    TIMER_SETUP_0_enter_DefaultMode_from_RESET();
    SMBUS_0_enter_DefaultMode_from_RESET();
    // [Config Calls]$

}

extern void WDT_0_enter_DefaultMode_from_RESET(void) {
    // $[WDTCN - Watchdog Timer Control]
    //Disable Watchdog with key sequence
    WDTCN = 0xDE; //First key
    WDTCN = 0xAD; //Second key
    // [WDTCN - Watchdog Timer Control]$

}

extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
    // $[P0 - Port 0 Pin Latch]
    // [P0 - Port 0 Pin Latch]$

    // $[P0MDOUT - Port 0 Output Mode]
    // [P0MDOUT - Port 0 Output Mode]$

    // $[P0MDIN - Port 0 Input Mode]
    // [P0MDIN - Port 0 Input Mode]$

    // $[P0SKIP - Port 0 Skip]
    /*
     // P0.0 pin is skipped by the crossbar
     // P0.1 pin is skipped by the crossbar
     // P0.2 pin is skipped by the crossbar
     // P0.3 pin is not skipped by the crossbar
     // P0.4 pin is not skipped by the crossbar
     // P0.5 pin is not skipped by the crossbar
     // P0.6 pin is not skipped by the crossbar
     // P0.7 pin is not skipped by the crossbar
     */
    P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
            | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED
            | P0SKIP_B5__NOT_SKIPPED | P0SKIP_B6__NOT_SKIPPED
            | P0SKIP_B7__NOT_SKIPPED;
    // [P0SKIP - Port 0 Skip]$

    // $[P0MASK - Port 0 Mask]
    // [P0MASK - Port 0 Mask]$

    // $[P0MAT - Port 0 Match]
    // [P0MAT - Port 0 Match]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
    // $[P1 - Port 1 Pin Latch]
    // [P1 - Port 1 Pin Latch]$

    // $[P1MDOUT - Port 1 Output Mode]
    // [P1MDOUT - Port 1 Output Mode]$

    // $[P1MDIN - Port 1 Input Mode]
    // [P1MDIN - Port 1 Input Mode]$

    // $[P1SKIP - Port 1 Skip]
    /*
     // P1.0 pin is skipped by the crossbar
     // P1.1 pin is skipped by the crossbar
     // P1.2 pin is skipped by the crossbar
     // P1.3 pin is skipped by the crossbar
     // P1.4 pin is skipped by the crossbar
     // P1.5 pin is skipped by the crossbar
     // P1.6 pin is skipped by the crossbar
     */
    P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
            | P1SKIP_B3__SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED
            | P1SKIP_B6__SKIPPED;
    // [P1SKIP - Port 1 Skip]$

    // $[P1MASK - Port 1 Mask]
    // [P1MASK - Port 1 Mask]$

    // $[P1MAT - Port 1 Match]
    // [P1MAT - Port 1 Match]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
    // $[XBR2 - Port I/O Crossbar 2]
    /*
     // Weak Pullups enabled 
     // Crossbar enabled
     */
    XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
    // [XBR2 - Port I/O Crossbar 2]$

    // $[PRTDRV - Port Drive Strength]
    // [PRTDRV - Port Drive Strength]$

    // $[XBR0 - Port I/O Crossbar 0]
    /*
     // UART I/O unavailable at Port pin
     // SPI I/O unavailable at Port pins
     // SMBus 0 I/O routed to Port pins
     // CP0 unavailable at Port pin
     // Asynchronous CP0 unavailable at Port pin
     // CP1 unavailable at Port pin
     // Asynchronous CP1 unavailable at Port pin
     // SYSCLK unavailable at Port pin
     */
    XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__ENABLED
            | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
            | XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
    // [XBR0 - Port I/O Crossbar 0]$

    // $[XBR1 - Port I/O Crossbar 1]
    // [XBR1 - Port I/O Crossbar 1]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
    // $[CLKSEL - Clock Select]
    /*
     // Clock derived from the Internal High-Frequency Oscillator
     // SYSCLK is equal to selected clock source divided by 1
     */
    CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_1;
    // [CLKSEL - Clock Select]$

}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
    // $[Timer Initialization]
    //Save Timer Configuration
    uint8_t TCON_save;
    TCON_save = TCON;
    //Stop Timers
    TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

    // [Timer Initialization]$

    // $[TH0 - Timer 0 High Byte]
    /*
     // Timer 0 High Byte = 0x7F
     */
    TH0 = (0x7F << TH0_TH0__SHIFT);
    // [TH0 - Timer 0 High Byte]$

    // $[TL0 - Timer 0 Low Byte]
    /*
     // Timer 0 Low Byte = 0xFF
     */
    TL0 = (0xFF << TL0_TL0__SHIFT);
    // [TL0 - Timer 0 Low Byte]$

    // $[TH1 - Timer 1 High Byte]
    // [TH1 - Timer 1 High Byte]$

    // $[TL1 - Timer 1 Low Byte]
    /*
     // Timer 1 Low Byte = 0xC8
     */
    TL1 = (0xC8 << TL1_TL1__SHIFT);
    // [TL1 - Timer 1 Low Byte]$

    // $[Timer Restoration]
    //Restore Timer Configuration
    TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

    // [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
    // $[CKCON0 - Clock Control 0]
    /*
     // System clock divided by 4
     // Counter/Timer 0 uses the system clock
     // Timer 2 high byte uses the clock defined by T2XCLK in TMR2CN0
     // Timer 2 low byte uses the clock defined by T2XCLK in TMR2CN0
     // Timer 3 high byte uses the clock defined by T3XCLK in TMR3CN0
     // Timer 3 low byte uses the clock defined by T3XCLK in TMR3CN0
     // Timer 1 uses the system clock
     */
    CKCON0 = CKCON0_SCA__SYSCLK_DIV_4 | CKCON0_T0M__SYSCLK
            | CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
            | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
            | CKCON0_T1M__SYSCLK;
    // [CKCON0 - Clock Control 0]$

    // $[TMOD - Timer 0/1 Mode]
    /*
     // Mode 1, 16-bit Counter/Timer
     // Mode 2, 8-bit Counter/Timer with Auto-Reload
     // Timer Mode. Timer 0 increments on the clock defined by T0M in the
     //     CKCON0 register
     // Timer 0 enabled when TR0 = 1 irrespective of INT0 logic level
     // Timer Mode. Timer 1 increments on the clock defined by T1M in the
     //     CKCON0 register
     // Timer 1 enabled when TR1 = 1 irrespective of INT1 logic level
     */
    TMOD = TMOD_T0M__MODE1 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
            | TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
    // [TMOD - Timer 0/1 Mode]$

    // $[TCON - Timer 0/1 Control]
    // [TCON - Timer 0/1 Control]$

}

extern void SMBUS_0_enter_DefaultMode_from_RESET(void) {
    // $[SMB0ADR - SMBus 0 Slave Address]
    /*
     // General Call Address is ignored
     // SMBus Hardware Slave Address = 0x3F
     */
    SMB0ADR = SMB0ADR_GC__IGNORED | (0x3F << SMB0ADR_SLV__SHIFT);
    // [SMB0ADR - SMBus 0 Slave Address]$

    // $[SMB0ADM - SMBus 0 Slave Address Mask]
    /*
     // Automatic slave address recognition and hardware acknowledge is
     //     enabled
     // SMBus Slave Address Mask = 0x7F
     */
    SMB0ADM = SMB0ADM_EHACK__ADR_ACK_AUTOMATIC | (0x7F << SMB0ADM_SLVM__SHIFT);
    // [SMB0ADM - SMBus 0 Slave Address Mask]$

    // $[SMB0TC - SMBus 0 Timing and Pin Control]
    /*
     // SCL is mapped to the lower-numbered port pin, and SDA is mapped to the
     //     higher-numbered port pin
     // No additional hold time window 
     */
    SMB0TC = SMB0TC_SWAP__SDA_HIGH_PIN | SMB0TC_SDD__NONE;
    // [SMB0TC - SMBus 0 Timing and Pin Control]$

    // $[SMB0CF - SMBus 0 Configuration]
    /*
     // Timer 1 Overflow
     // Enable the SMBus module
     */
    SMB0CF &= ~SMB0CF_SMBCS__FMASK;
    SMB0CF |= SMB0CF_SMBCS__TIMER1 | SMB0CF_ENSMB__ENABLED;
    // [SMB0CF - SMBus 0 Configuration]$

}

extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
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
    /*
     // Timer 2 Reload High Byte = 0xFF
     */
    TMR2RLH = (0xFF << TMR2RLH_TMR2RLH__SHIFT);
    // [TMR2RLH - Timer 2 Reload High Byte]$

    // $[TMR2RLL - Timer 2 Reload Low Byte]
    /*
     // Timer 2 Reload Low Byte = 0x34
     */
    TMR2RLL = (0x34 << TMR2RLL_TMR2RLL__SHIFT);
    // [TMR2RLL - Timer 2 Reload Low Byte]$

    // $[TMR2CN0]
    /*
     // Start Timer 2 running
     */
    TMR2CN0 |= TMR2CN0_TR2__RUN;
    // [TMR2CN0]$

    // $[Timer Restoration]
    // Restore Timer Configuration
    TMR2CN0 |= TMR2CN0_TR2_save;
    // [Timer Restoration]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {

}

