/*
 * InfoBlock.h
 *
 *  Created on: Feb 21, 2017
 *      Author: sherifeid
 */

#ifndef INFOBLOCK_H_
#define INFOBLOCK_H_

// Defines for use in the InfoBlock
#define TGT_APP_INFOBLOCK_LENGTH 7

#define TGT_MCU_CODE             0x39  // Code for this MCU partnumber
#define TGT_BL_TYPE              0x04  // SMBUS BL
#define TGT_FLASH_PAGE_SIZE_CODE 0x01  // Flash Page Size Code: 1 => 512 bytes
#define APP_FW_VERSION_HIGH      0x01
#define APP_FW_VERSION_LOW       0x01
#define SMBUS_DEVICE_ADDR        0x10
// Signature Bytes
#define SIG_BYTE0                0xF3
#define SIG_BYTE1                0x32
#define SIG_BYTE2                0xBC
#define SIG_BYTE3                0x87

#endif /* INFOBLOCK_H_ */
