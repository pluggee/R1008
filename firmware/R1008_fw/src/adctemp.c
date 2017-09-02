/*
 * adctemp.c
 *
 *  Created on: Apr 24, 2017
 *      Author: sherifeid
 */

#include "adctemp.h"
#include "global.h"
#include "compiler_defs.h"
#include <SI_EFM8BB1_Register_Enums.h>

char temp_internal;

void calcInternalTemp(void){
    // This function reads the internal temperature sensor value from ADC
    // and processes the data to evaluate the temperature

    uint32_t accumulator = 0;           // accumulator used to store ADC measurements
    char i;                             // for loop variable

    for (i=0; i<INTERNAL_TEMP_AVG; i++){
        // perform ADC measurement
       ADC0CN0_ADBUSY = 1;              // start ADC conversion
       while (!ADC0CN0_ADINT);          // wait until conversion is complete
       ADC0CN0_ADINT = 0;               // clear ADC interrupt flag
       accumulator += ADC0;             // accumulate data
    }

    temp_internal = (char) ((accumulator - TOFFSET)/TSLOPE);    // calculate and store temperature in register
}


