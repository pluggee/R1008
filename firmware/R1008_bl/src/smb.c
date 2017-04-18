/*
 * smb.c
 *
 *  Created on: Feb 12, 2017
 *      Author: sherifeid
 */

#include <compiler_defs.h>
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "smb.h"
#include "R1008_bl_main.h"


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// ID pin definitions
SBIT(ID0, SFR_P0, 3);
SBIT(ID1, SFR_P0, 2);
SBIT(ID2, SFR_P1, 6);
SBIT(ID3, SFR_P1, 5);


void SetI2CSlaveAddress (void)
{
    // This function reads the ID pins on the MCU to identify the slave address
    unsigned char pid0, pid1, pid2, pid3, i2cid;

    // extract values from GPIO
    if (ID0 == 0){
        pid0 = 0;
    }
    else{
        pid0 = 1;
    }
    if (ID1 == 0){
        pid1 = 0;
    }
    else{
        pid1 = 1;
    }
    if (ID2 == 0){
        pid2 = 0;
    }
    else{
        pid2 = 1;
    }
    if (ID3 == 0){
        pid3 = 0;
    }
    else{
        pid3 = 1;
    }

    i2cid = pid0 | (pid1 << 1) | (pid2 << 2) | (pid3 << 3) | 0x10;

    // I2C addresses range from 0x10 to 0x1F depending on where the module is populated
    SMB0ADR = SMB0ADR_GC__IGNORED | (i2cid << SMB0ADR_SLV__SHIFT);
}

void SMBHandler(void){
    static uint8_t sent_byte_counter;
    static uint8_t rec_byte_counter;

    if (SMB0CN0_SI){

        if (SMB0CN0_ARBLOST == 0)
        {
            switch (SMB0CN0 & 0xF0)          // Decode the SMBus status vector
            {
                // Slave Receiver: Start+Slave Address received
                case SMB_SRADD:
                    SMB0CN0_STA = 0;// Clear SMB0CN0_STA bit

                    sent_byte_counter = 1;// Reinitialize the data counters
                    rec_byte_counter = 1;

                    // need to add an acknowledge here????
                    // added an ack here
                    SMB0CN0_ACK = 1;// send an acknowledge that address and command are received

                    if ((SMB0DAT & 0x01) == READ)// If the transfer is a master READ,
                    {
                        // Prepare outgoing byte
                        SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
                        sent_byte_counter++;
                    }

                    break;

                // Slave Receiver: Data received
                case SMB_SRDB:
                    if (rec_byte_counter < TGT_BL_BUF_SIZE)
                    {
                        // Store incoming data
                        SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT;
                        rec_byte_counter++;
                        SMB0CN0_ACK = 1;         // SMB0CN0_ACK received data
                    }
                    else
                    {
                        // Store incoming data
                        SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT;
                        DATA_READY = 1;         // Indicate new data fully received
                    }
                    break;

                // Slave Receiver: Stop received while either a Slave Receiver or
                // Slave Transmitter
                case SMB_SRSTO:
                    SMB0CN0_STO = 0;            // SMB0CN0_STO must be cleared by software when
                                                // a STOP is detected as a slave

                    DATA_READY = 1;             // Indicates end of transmission
                    break;

                // Slave Transmitter: Data byte transmitted
                case SMB_STDB:
                    if (SMB0CN0_ACK == 1)// If Master SMB0CN0_ACK's, send the next byte
                    {
                        if (sent_byte_counter <= TGT_BL_BUF_SIZE)
                        {
                            // Prepare next outgoing byte
                            SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
                            sent_byte_counter++;
                        }
                    }                          // Otherwise, do nothing
                    break;

                // Slave Transmitter: Arbitration lost, Stop detected
                //
                // This state will only be entered on a bus error condition.
                // In normal operation, the slave is no longer sending data or has
                // data pending when a STOP is received from the master, so the SMB0CN0_TXMODE
                // bit is cleared and the slave goes to the SRSTO state.
                case SMB_STSTO:
                    SMB0CN0_STO = 0;            // SMB0CN0_STO must be cleared by software when
                                                // a STOP is detected as a slave
                    break;

              // Default: all other cases undefined
                default:
                    SMB0CF &= ~0x80;// Reset communication
                    SMB0CF |= 0x80;
                    SMB0CN0_STA = 0;
                    SMB0CN0_STO = 0;
                    SMB0CN0_ACK = 1;
                    break;
            }
        }
        // SMB0CN0_ARBLOST = 1, Abort failed transfer
        else
        {
            SMB0CN0_STA = 0;
            SMB0CN0_STO = 0;
            SMB0CN0_ACK = 1;
        }

    SMB0CN0_SI = 0;                     // Clear SMBus interrupt flag
    }
}
