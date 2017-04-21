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
#define REFRES              402     // reference resistor value

// Algorithm constants
#define NITERATIONS         16      // number of search iterations
#define TMIDDLE             150     // initial temperature for PT100 successive search algorithm
#define TSTEP0              350     // initial step size for successive search algorithm
                                    // temperature range covered from (TMIDDLE - TSTEP0) to (TMIDDLE + TSTEP0)
                                    // example: if TMIDDLE = 150 and TSTEP0 = 350
                                    // the covered temperature range is -200C to 500C

// function declarations
void max31865_init(void);
void spi_wr_cfg(char , char);
char spi_rd_cfg(char);
float calcR(float);
float readRTDtemp(char);

char spi_readreg(char, char);
void spi_writereg(char, char, char);

void Delay_ms (unsigned int);       // timer based ms delay

#ifdef DEBUGFW
extern char faultstat;
extern char r_hftlsb,r_hftmsb,r_lftlsb,r_lftmsb;
void dumpreg(void);
#endif


#endif /* MAX31865_H_ */
