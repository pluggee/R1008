
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
union temperature ch1temp;
union temperature ch2temp;

union temperature ch1res;
union temperature ch2res;

char ch1config;
char ch2config;

char faultstat;

float ifloat;                   // a temporary variable for storing floating numbers
                                // seems that I2C transactions corrupt variables used by other functions

// Output pins
SBIT(STAT, SFR_P2, 0);                                  // debug output used to measure timing

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
	P2MDOUT = 0x01;                     // set P2.0 (C2D) pin to output push pull for debugging
	faultstat = 0x00;
}

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
    // Call hardware initialization routine
	enter_DefaultMode_from_RESET();
	Init();				                // Initialize system

    STAT = 0;

	while (1) 
   {
//	    Delay_ms(DELAYCALC);            // delay 100ms
	    ch1config = spi_readreg(1, MAX31865_CONFIG);        // read config register CH1
	    STAT = 1;
	    ifloat = readRTDres(1);
	    ch1res.f = ifloat;
	    ifloat = readRTDtemp(ch1res.f);                     // read and process CH1 PT100 temperature
	    ch1temp.f = ifloat;
	    STAT = 0;

	    ch2config = spi_readreg(2, MAX31865_CONFIG);        // read config register CH2
	    ch2res.f = readRTDres(2);
        ch2temp.f = readRTDtemp(ch2res.f);                  // read and process CH2 PT100 temperature

	    // read and process CH2 PT100 temperature
	    // read and process internal temperature sensor
	    // check for any reconfigurations? execute reconfig and reset reconfigflag
   }                             
}
