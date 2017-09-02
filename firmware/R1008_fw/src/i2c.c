/*
 * i2c.c
 *
 *  Created on: Sep 8, 2016
 *      Author: sherifeid
 */

#include "i2c.h"
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "compiler_defs.h"


// ID pin definitions
SBIT(ID0, SFR_P1, 6);
SBIT(ID1, SFR_P1, 5);
SBIT(ID2, SFR_P1, 4);
SBIT(ID3, SFR_P1, 3);


void SetI2CSlaveAddress (void)
{
	// This function reads the ID pins on the MCU to identify the slave address
	unsigned char pid0, pid1, pid2, pid3, i2cid;

	// extract values from GPIO
	if (ID0 == 0){
		pid0 = 0;
	}
	else{
		pid0 = 1;
	}
	if (ID1 == 0){
		pid1 = 0;
	}
	else{
		pid1 = 1;
	}
	if (ID2 == 0){
		pid2 = 0;
	}
	else{
		pid2 = 1;
	}
	if (ID3 == 0){
		pid3 = 0;
	}
	else{
		pid3 = 1;
	}

	//i2cid = ID0 | (ID1 << 1) | (ID2 << 2) | (ID3 << 3) | 0x10;
	i2cid = pid0 | (pid1 << 1) | (pid2 << 2) | (pid3 << 3) | 0x10;


	// I2C addresses range from 0x10 to 0x1F depending on where the module is populated
	SMB0ADR = SMB0ADR_GC__IGNORED | (i2cid << SMB0ADR_SLV__SHIFT);
}


