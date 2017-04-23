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

// variable instantiation
char rtdmsb, rtdlsb;                  // 8-bit buffers for register values

void max31865_init(void)
{
    // this function initializes max31865 RTD to digital sensor
    CS1B = 1;                                       // disable CS1B
    CS2B = 1;                                       // disable CS2B

    // Write to CONFIG register for both channels
    // (D7) Set to 1: turn on VBIAS
    // (D6) Set to 1: select automatic conversion mode, rate follows the filter mode 50Hz/60Hz
    // (D5) Set to 0: not using single shot conversion
    // (D4) Set to 0: the default PT100 to 4 wire
    // (D3:D2) Set to 00: no manual fault detection action
    // (D1) Set to 0: do not clear fault register
    // (D0) Set to 0: filter at 60Hz
//    spi_writereg(1, MAX31865_CONFIG, 0xC0);
//    spi_writereg(2, MAX31865_CONFIG, 0xC0);

    spi_writereg(1, MAX31865_CONFIG, 0x80);
    spi_writereg(2, MAX31865_CONFIG, 0x80);
}


float readRTDtemp(float rtdres){
    // This function reads the RTD temperature from the given SPI channel
    // channel : SPI device

    // local variables

    float rcalc;                        // intermediate calculated value for successive search
    float rtdtemp;                      // processed temperature value
    float tmpstep;                      // step used for successive search algorithm
    char i;                             // for loop variable

    // Step3: calculate accurate temperature from resistor value
    rtdtemp = TMIDDLE;                  // initial temperature value
    tmpstep = TSTEP0;                   // initial step value

    for (i=0;i<NITERATIONS;i++){
        rcalc = calcR(rtdtemp);
        if (rcalc > rtdres){
            rtdtemp -= tmpstep;
        }
        else{
            rtdtemp += tmpstep;
        }
        tmpstep = tmpstep/2;
    }
    return rtdtemp;                     // return processed temperature
}

float readRTDres(char channel){
    // This function reads the RTD temperature from the given SPI channel
    // channel : SPI device

    // local variables

    U16 rtdreg;                         // 16-bit calculation


    spi_writereg(channel, MAX31865_CONFIG, 0x80);     // make sure bias is turned on
    Delay_ms(5);
    spi_writereg(channel, MAX31865_CONFIG, 0xA0);     // one shot conversion
    // wait until valid data in registers
    if (channel == 1){
        while(DRDY1B);                              // wait until done
    }
    else{
        while(DRDY2B);                              // wait until done
    }

    rtdmsb = spi_readreg(channel, MAX31865_RTDMSB);
    rtdlsb = spi_readreg(channel, MAX31865_RTDLSB);

    // detect faults
    if (rtdlsb & 0x01){
        if (channel == 1){
            faultstat = faultstat | 0x01;
        }
        else{
            faultstat = faultstat | 0x02;
        }
    }

    // Step2: calculate PT100 resistance value
    rtdreg = ((U16)rtdmsb << 8);        // Step2.1 : load MSB
    rtdreg = rtdreg | rtdlsb;           // Step2.2 : load LSB and adjust
    rtdreg = rtdreg >> 1;               // shift 1

    return ((float)rtdreg * REFRES)/32768;     // return calculated PT100 resistance value
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
        return 100 * (1 + 3.90830e-3*T - 5.77500e-7*T*T - 4.18301e-12*(T-100)*T*T*T);
    }
    else{
        return 100 * (1 + 3.90830e-3*T - 5.77500e-7*T*T);
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

