
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
#include "adctemp.h"

// $[Generated Includes]
// [Generated Includes]$

// Global variables
union temperature ch1temp;
union temperature ch2temp;
//
union temperature ch1res;
union temperature ch2res;

char ch1config;
char ch2config;

char ch1fault;
char ch2fault;

unsigned char rtdmsb1;
unsigned char rtdmsb2;
unsigned char rtdlsb1;
unsigned char rtdlsb2;

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
	max31865_init();                    //// Initialize RTD sensors
	P2MDOUT = 0x01;                     // set P2.0 (C2D) pin to output push pull for debugging
	ch1fault = 0x00;                    // initialize fault status register
	ch2fault = 0x00;                    // initialize fault status register
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
	    // every calculation takes around 50ms
	    STAT = 1;                       // this is to measure the time it takes for floating point calc
	    ch1fault = spi_readreg(1,MAX31865_FAULT);           // read ch1 fault register
	    spi_writereg(1,
	            MAX31865_CONFIG,
	            spi_readreg(1,MAX31865_CONFIG)|0x02);       // clear MAX31865 fault register
	    getRTDtemp(1);
	    CalcRTDTemp(1);
        STAT = 0;

        ch2fault = spi_readreg(1,MAX31865_FAULT);           // read ch2 fault register
        spi_writereg(2,
                MAX31865_CONFIG,
                spi_readreg(2,MAX31865_CONFIG)|0x02);       // clear MAX31865 fault register
	    getRTDtemp(2);
	    CalcRTDTemp(2);

	    calcInternalTemp();


	    Delay_ms(20);                   // delay between reading data from registers

	    // check for any reconfigurations? execute reconfig and reset reconfigflag
        //FIXME add code to reconfigure in case user wants to change notch filter to 50Hz on the fly
	}
}
