//-----------------------------------------------------------------------------
// F39x_InfoBlock.c - SMB_BL - Target_BL_FW
//-----------------------------------------------------------------------------
// Copyright (C) 2009 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains a set of bytes that will be placed as the last bytes of
// the App FW. The locating is done by using the linker command line.
//
// Locate starting at address (16*1024-512-7-4)=0X3DF5  
//
// Target:         C8051F390
// Tool chain:     Keil C51 9.02 / Keil EVAL C51
// Command Line:   None
//
//
// Release 1.0 / 1Apr2013 (Mark Ding)
//    -Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "compiler_defs.h"
#include "InfoBlock.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

/*****************************************
Target App FW InfoBlock Format
Byte offset (from last byte) - Field

0	Signature Byte Low (0) (sig bytes are added by the bootloader after successful programming)
-1	Signature Byte (1)
-2	Signature Byte (2)
-3	Signature Byte High (3)
-4	InfoBlock Length = 7 (for CAN BL; count includes this length byte)
-5	MCU Code
-6	BL Type
-7	Flash Page Size Code
-8	Application FW Version Low (0)
-9	Application FW Version High (1)
-10	BL-specific byte (0) - SMBus Device Addr - Not used for read by FW (only present for hex file image)
*****************************************/

// Reverse byte order for Keil:
SEGMENT_VARIABLE(TGT_App_InfoBlock[], const U8, SEG_CODE) =
//SEGMENT_VARIABLE(INFOBLOCK[], const U8, SEG_XDATA) =
{
	SMBUS_DEVICE_ADDR,
	APP_FW_VERSION_HIGH,
	APP_FW_VERSION_LOW,
	TGT_FLASH_PAGE_SIZE_CODE,
	TGT_BL_TYPE,
	TGT_MCU_CODE,
	TGT_APP_INFOBLOCK_LENGTH,
	SIG_BYTE3,
	SIG_BYTE2,
	SIG_BYTE1,
	SIG_BYTE0,
};



//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
