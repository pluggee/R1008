/*
 * drv8825.c
 *
 *  Created on: Sep 8, 2016
 *      Author: sherifeid
 */

#include "max31865.h"
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "compiler_defs.h"
#include <intrins.h>
#include "global.h"

// This source includes all control functions for DRV8825 chip

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// MAX31865 Pin Declarations
//-----------------------------------------------------------------------------
// Output pins
SBIT(CS1B, SFR_P1, 0);            					// 1st channel chip select  (output)
SBIT(DRDY1B, SFR_P1, 1);                            // 1st channel data ready   (input)
SBIT(CS2B, SFR_P0, 6);            					// 2nd channel chip select  (output)
SBIT(DRDY2B, SFR_P0, 7);                            // 2nd channel data ready   (input)

// Local function prototypes
void Delay_us (U8 time_us);
void Delay_ms (unsigned int time_ms);

void max31865_init(void)
{
    // this function initializes max31865 RTD to digital sensor
    CS1B = 1;                                       // disable CS1B
    CS2B = 1;                                       // disable CS2B

    // Write to CONFIG register for both channels
    // (D7) Set to 1: turn on VBIAS
    // (D6) Set to 0: select one shot conversion mode
    // (D5) Set to 0: not using single shot conversion
    // (D4) Set to 0: the default PT100 to 4 wire
    // (D3:D2) Set to 00: no manual fault detection action
    // (D1) Set to 0: do not clear fault register
    // (D0) Set to 0: filter at 60Hz
    // therefore CONFIG_VAL = 0x80

    spi_writereg(1, MAX31865_CONFIG, CONFIG_VAL);
    spi_writereg(2, MAX31865_CONFIG, CONFIG_VAL);
}

float calcR(float T){
    // This function calculates the resistance for a given temperature

    // RTD Callendar-Van Dusen equation coefficients
    // a = 3.90830e-3
    // b = -5.77500e-7
    // c =
    //  -4.18301e-12 for -200 < T < 0
    //  0            for 0 <= T < 850
    // R(T) = R0(1 + aT + bT^2 + c(T - 100)T^3)

    if (T < 0){
        return 100.0 * (1 + 3.90830e-3*T - 5.77500e-7*T*T - 4.18301e-12*(T-100.0)*T*T*T);
    }
    else{
        return 100.0 * (1 + 3.90830e-3*T - 5.77500e-7*T*T);
    }

}

//-----------------------------------------------------------------------------
// Delay_us
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_us - time delay in microseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in microseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_us (U8 time_us)
{
   TMR2CN0_TR2 = 0;                     // Stop timer
   TMR2CN0_TF2H = 0;                    // Clear timer overflow flag
   TMR2 = -( (U16)(SYSCLK/1000000) * (U16)(time_us) );
   TMR2CN0_TR2 = 1;                     // Start timer
   while(!TMR2CN0_TF2H);                // Wait till timer overflow occurs
   TMR2CN0_TR2 = 0;                     // Stop timer
}

//-----------------------------------------------------------------------------
// Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_ms - time delay in milliseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in milliseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_ms (unsigned int time_ms)
{
   U8 i;

   while(time_ms--)
   {
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
      {
         Delay_us (100);
      }
   }
}

char spi_readreg(char channel, char address){
    if (channel == 2){                  // Step1.1: Activate Slave Select
        CS2B = 0;
    }
    else{
        CS1B = 0;
    }
    Delay_us (1);                       // Step1.2 : Wait 1 us, CS setup time
    SPI0DAT = address;                  // Step1.3 : Send register address for read
    while(!SPI0CN0_SPIF);               // Step1.4 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.5 : Clear the SPI intr. flag

    SPI0DAT = 0;                        // Step1.6 : Dummy write to output serial clock
    while(!SPI0CN0_SPIF);               // Step1.7 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.8 : Clear the SPI intr. flag

    CS1B = 1;                           // Step1.9 : Deactivate Slave Select
    CS2B = 1;

    Delay_us (10);                      // Step1.10: CS hold time

    return SPI0DAT;
}

void spi_writereg(char channel, char address, char value){
    // This function writes a specific value to register through SPI
    if (channel == 2){                  // Step1.1: Activate Slave Select
        CS2B = 0;
    }
    else{
        CS1B = 0;
    }
    Delay_us (1);                       // Step1.2 : Wait 1 us, CS setup time
    SPI0DAT = address | 0x80;           // Step1.3 : Send register address for write
    while(!SPI0CN0_SPIF);               // Step1.4 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.5 : Clear the SPI intr. flag

    SPI0DAT = value;                    // Step1.6 : Send the value to be written to CONFIG register
    while(!SPI0CN0_SPIF);               // Step1.7 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.8 : Clear the SPI intr. flag

    CS1B = 1;                           // Step1.9 : Deactivate Slave Select
    CS2B = 1;

    Delay_us (10);                      // Step1.10: CS hold time
}

void getRTDtemp (char channel){
    spi_writereg(channel, MAX31865_CONFIG, CONFIG_VAL);     // make sure bias is turned on
    Delay_ms(5);
    spi_writereg(channel, MAX31865_CONFIG, CONFIG_VAL | 0x20);     // one shot conversion

    if (channel == 2){
        while(DRDY2B);
    }
    else{
        while(DRDY1B);                              // wait until done
    }

    // reads RTD resistor MSB and LSB registers from SPI
    if (channel == 2){                  // Step1.1: Activate Slave Select
        CS2B = 0;
    }
    else{
        CS1B = 0;
    }
    Delay_us (1);                       // Step1.2 : Wait 1 us, CS setup time
    SPI0DAT = MAX31865_RTDMSB;          // Step1.3 : Send register address for read
    while(!SPI0CN0_SPIF);               // Step1.4 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.5 : Clear the SPI intr. flag

    SPI0DAT = 0;                        // Step1.6 : Dummy write to output serial clock
    while(!SPI0CN0_SPIF);               // Step1.7 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.8 : Clear the SPI intr. flag

    if (channel == 2){
        rtdmsb2 = SPI0DAT;
    }
    else{
        rtdmsb1 = SPI0DAT;
    }

    SPI0DAT = 0;                        // Step1.6 : Dummy write to output serial clock
    while(!SPI0CN0_SPIF);               // Step1.7 : Wait for end of transfer
    SPI0CN0_SPIF = 0;                   // Step1.8 : Clear the SPI intr. flag

    if (channel == 2){
        rtdlsb2 = SPI0DAT;
    }
    else{
        rtdlsb1 = SPI0DAT;
    }

    CS1B = 1;                           // Step1.9 : Deactivate Slave Select
    CS2B = 1;

    Delay_us (10);                      // Step1.10: CS hold time
}

void CalcRTDTemp(char channel){
    // This computes the temperature from respective rtdlsb/rtdmsb variables
    // and assign them to chXtemp variables
    unsigned int rtdreg;
    float resval;
    float rtdtemp;
    float tmpstep;
    float rcalc;
    char i;

    if (channel == 2){
        rtdreg = ((unsigned int)rtdmsb2 << 8);       // load MSB
        rtdreg = rtdreg | rtdlsb2;          // load LSB and adjust
        rtdreg = rtdreg >> 1;               // shift 1-bit to right
    }
    else{
        rtdreg = ((unsigned int)rtdmsb1 << 8);       // load MSB
        rtdreg = rtdreg | rtdlsb1;          // load LSB and adjust
        rtdreg = rtdreg >> 1;               // shift 1-bit to right
    }

    resval = ((float)rtdreg * REFRES)/32768;     // return calculated PT100 resistance value

    // store values in global variables (for I2C readout)
    if (channel == 2){
        ch2res.f = resval;
    }
    else{
        ch1res.f = resval;
    }

    rtdtemp = TMIDDLE;                  // initial temperature value
    tmpstep = TSTEP0;                   // initial step value

    for (i=0;i<NITERATIONS;i++){
        rcalc = calcR(rtdtemp);
        if (rcalc > resval){
            rtdtemp = rtdtemp - tmpstep;
        }
        else if (rcalc < resval){
            rtdtemp = rtdtemp + tmpstep;
        }
        tmpstep = tmpstep/2.0;
    }

    if (channel == 2){
        ch2temp.f = rtdtemp;
    }
    else{
        ch1temp.f = rtdtemp;
    }
}
