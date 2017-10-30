/*
 * R1008_bl_main.h
 *
 *  Created on: Feb 12, 2017
 *      Author: sherifeid
 */

#ifndef R1008_BL_MAIN_H_
#define R1008_BL_MAIN_H_

// -----------------------------------
// Bootloader Version (BCD encoding)
// -----------------------------------
#define TGT_BL_VER              0x10

#define TGT_BL_BUF_SIZE         40                 // Size of bootloader RX buffer (bytes; should be powers of 2)
// min: 2^5 = 32; max: 2^14 = 16384; 2^15 = RESERVED.

//#define TGT_BL_BUF_SIZE_CODE     0x05  // BL Buffer Size Code.
// If TGT_BL_BUF_SIZE is 2^n, TGT_BL_BUF_SIZE_CODE is n.
// See documentation for encoding details.

// Function prototypes
// This is usually defined in the STARTUP.A51 file.
//void START_APPLICATION(void);


//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
// Global holder for SMBus data.
// All receive data is written here
// NUM_BYTES_WR used because an SMBus write is Master->Slave
extern uint8_t SMB_DATA_IN[TGT_BL_BUF_SIZE];

// Global holder for SMBus data.
// All transmit data is read from here
// NUM_BYTES_RD used because an SMBus read is Slave->Master
extern uint8_t SMB_DATA_OUT[TGT_BL_BUF_SIZE];

extern bit DATA_READY;              // Set to '1' by the SMBus ISR
                                    // when a new data byte has been
                                    // received.

//extern char writelen;               // this is a global variable to store the length of I2C write command
                                    // if it is 1, that means the intention is to set register address
                                    // if it is more, then the intention is to write the value to the register



#endif /* R1008_BL_MAIN_H_ */
