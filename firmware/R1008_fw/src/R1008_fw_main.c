
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "compiler_defs.h"
#include "max31865.h"
#include "i2c.h"
#include "global.h"
#include "InfoBlock.h"

// $[Generated Includes]
// [Generated Includes]$

// Global variables
float ch1temp;
float ch2temp;

// Output pins
#ifdef DEBUGFW
SBIT(STAT, SFR_P2, 0);                                  // debug output used to measure timing
#endif

// Global holder for SMBus data.
// All receive data is written here
// NUM_BYTES_WR used because an SMBus write is Master->Slave
uint8_t SMB_DATA_IN[NUM_BYTES_WR];

// Global holder for SMBus data.
// All transmit data is read from here
// NUM_BYTES_RD used because an SMBus read is Slave->Master
uint8_t SMB_DATA_OUT[NUM_BYTES_RD];

bit DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

// Driver Initialization function
void Init (void)
{
	SetI2CSlaveAddress();               // Initialize I2C slave address
	// Initialize RTD sensor
#ifdef DEBUGFW
	P2MDOUT = 0x01;                     // set P2.0 (C2D) pin to output push pull for debugging
#endif
}

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
    // Call hardware initialization routine
	enter_DefaultMode_from_RESET();
	Init();				                // Initialize system

#ifdef DEBUGFW
    STAT = 0;
#endif

	while (1) 
   {
	    Delay_ms(DELAYCALC);            // delay 100ms
#ifdef DEBUGFW
	    dumpreg();
	    STAT = 1;
#endif
	    ch1temp = readRTDtemp(1);                   // read and process CH1 PT100 temperature
#ifdef DEBUGFW
	    STAT = 0;
#endif

//	    ch2temp = readRTDtemp(2);                   // read and process CH1 PT100 temperature

	    // read and process CH2 PT100 temperature
	    // read and process internal temperature sensor
	    // check for any reconfigurations? execute reconfig and reset reconfigflag
   }                             
}
