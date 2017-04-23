/*
 * global.h
 *
 *  Created on: Sep 10, 2016
 *      Author: sherifeid
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK      24500000    // Internal oscillator frequency in Hz
#define PLATFORM    0x01        // platform ID, 0x01 for R1000A
#define DEVID       0x08        // device ID, 0x08 for R1008 stepper motor driver
#define DELAYCALC   200         // delay between calculations

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
extern char temp_val;		    // processed MCU temperature value

// Unions
union temperature {
    float f;
    char c[4];
};

extern union temperature ch1temp;       // channel 1 PT100 temperature
extern union temperature ch2temp;       // channel 2 PT100 temperature

extern union temperature ch1res;        // channel 1 PT100 resistance
extern union temperature ch2res;        // channel 2 PT100 resistance

extern char ch1config;                  // value of config registers
extern char ch2config;                  // value of config registers

extern char faultstat;                  // stores fault status, B0 (CH1 fault), B1 (CH2 fault)

// I2C REGISTER/COMMAND Definitions
// ---------------------------------
#define TGT_CMD_RESET_MCU           0x00
#define TGT_CMD_PLID                0x01
#define TGT_CMD_DVID                0x02
#define TGT_CMD_FWID                0x03

#define TGT_CMD_TMP                 0x10    // read temperature register

#define TGT_CMD_BLSTAT              0x0B    // bootloader/application status, returs 0x03 if in bootloader, 0x00 if in application

#define TGT_CMD_RTD1CFG             0x30    // channel 1 PT100 configuration
#define TGT_CMD_RTD2CFG             0x31    // channel 2 PT100 configuration

#define TGT_CMD_RTD1TEMP            0x32    // channel 1 PT100 temperature readout
#define TGT_CMD_RTD2TEMP            0x33    // channel 2 PT100 temperature readout

#define TGT_CMD_RTD1RES             0x34    // channel 1 PT100 resistance readout
#define TGT_CMD_RTD2RES             0x35    // channel 2 PT100 resistance readout

#define TGT_CMD_CH1CFG              0x36    // channel 1 config register
#define TGT_CMD_CH2CFG              0x37    // channel 2 config register

#define TGT_CMD_RTDFAULT            0x38    // reads out and resets fault status

#endif /* GLOBAL_H_ */
