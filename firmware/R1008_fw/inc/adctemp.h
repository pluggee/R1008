/*
 * adctemp.h
 *
 *  Created on: Apr 24, 2017
 *      Author: sherifeid
 */

#ifndef ADCTEMP_H_
#define ADCTEMP_H_



// temperature sensor constants
#define DS_SLOPE                0.00285     // slope from datasheet in V/C
#define DS_OFFSET               0.757       // offset from datasheet in V

#define SAMPLING_2N             4           // number of samples (power of 2)
#define INTERNAL_TEMP_AVG       16          // number of ADC samples to average die temperature measurement
#define TSLOPE                  113         // slope LSB's/C = round(SAMPLING_NUMBER*DS_SLOPE/LSBSize)
#define TOFFSET                 30067       // Offset in LSB's = round(SAMPLING_NUMBER*DS_OFFSET/LSBSize)

void calcInternalTemp(void);

#endif /* ADCTEMP_H_ */
