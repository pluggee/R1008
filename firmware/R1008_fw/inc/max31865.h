/*
 * drv8825.h
 *
 *  Created on: Sep 8, 2016
 *      Author: sherifeid
 */

#ifndef MAX31865_H_
#define MAX31865_H_

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "global.h"

// MAX31865 Addresses
#define  MAX31865_CONFIG    0x00    // config register read address
#define  MAX31865_RTDMSB    0x01    // RTD MSB register
#define  MAX31865_RTDLSB    0x02    // RTD LSB register
#define  MAX31865_HFTMSB    0x03    // High fault threshold MSB
#define  MAX31865_HFTLSB    0x04    // High fault threshold LSB
#define  MAX31865_LFTMSB    0x05    // Low fault threshold MSB
#define  MAX31865_LFTLSB    0x06    // Low fault threshold LSB
#define  MAX31865_FAULT     0x07    // Fault status

// Board components
#define REFRES              402.0   // reference resistor value (note .0 at the end)

// Configuration
//#define CONFIG_VAL          0x80    // this is the default config for one-shot/60Hz filter
#define CONFIG_VAL          0xC0    // continous mode/60Hz filter


// Algorithm constants
#define NITERATIONS         20      // number of search iterations
#define TMIDDLE             150.1     // initial temperature for PT100 successive search algorithm
#define TSTEP0              350.1     // initial step size for successive search algorithm
                                    // temperature range covered from (TMIDDLE - TSTEP0) to (TMIDDLE + TSTEP0)
                                    // example: if TMIDDLE = 150 and TSTEP0 = 350
                                    // the covered temperature range is -200C to 500C

// function declarations
void max31865_init(void);
float calcR(float);
//float readRTDtemp(float);
//float readRTDres(char);

void getRTDtemp(char);              // reads MSB and LSB temperature registers through SPI
void CalcRTDTemp(char);             // calculate temperature from SPI readout

char spi_readreg(char, char);
void spi_writereg(char, char, char);

void Delay_ms (unsigned int);       // timer based ms delay



#endif /* MAX31865_H_ */
