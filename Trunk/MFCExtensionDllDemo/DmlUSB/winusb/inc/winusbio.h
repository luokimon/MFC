/***************************************************************************

Copyright (c) 2002 Microsoft Corporation

Module Name:

        wusbio.h

Abstract:

        Public header for WINUSB

Environment:

        User and Kernel Mode

Notes:

        THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
        KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
        IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
        PURPOSE.

        Copyright (c) 2001 Microsoft Corporation.  All Rights Reserved.


Revision History:

        11/12/2002 : created


****************************************************************************/

#ifndef __WUSBIO_H__
#define __WUSBIO_H__

#if(NTDDI_VERSION >= NTDDI_WINXP)

#include <usb.h>
                 
// Pipe policy types
#define SHORT_PACKET_TERMINATE  0x01
#define AUTO_CLEAR_STALL        0x02
#define PIPE_TRANSFER_TIMEOUT   0x03
#define IGNORE_SHORT_PACKETS    0x04
#define ALLOW_PARTIAL_READS     0x05
#define AUTO_FLUSH              0x06
#define RAW_IO                  0x07
#define MAXIMUM_TRANSFER_SIZE   0x08
#define RESET_PIPE_ON_RESUME    0x09

// Power policy types
//
// Add 0x80 for Power policy types in order to prevent overlap with 
// Pipe policy types to prevent "accidentally" setting the wrong value for the 
// wrong type.
//
#define AUTO_SUSPEND            0x81
#define SUSPEND_DELAY           0x83

// Device Information types
#define DEVICE_SPEED            0x01

// Device Speeds
#define LowSpeed                0x01
#define FullSpeed               0x02
#define HighSpeed               0x03 

#include <initguid.h>
#if 0
// {DA812BFF-12C3-46a2-8E2B-DBD3B7834C43}
DEFINE_GUID(WinUSB_Guid, 0xda812bff, 0x12c3, 0x46a2, 0x8e, 0x2b, 0xdb, 0xd3, 0xb7, 0x83, 0x4c, 0x43);
#else
// {8365DB52-41FC-4366-A471-4D978CF6F76C}
//DEFINE_GUID(WinUSB_Guid, 0x8365db52, 0x41fc, 0x4366, 0xa4, 0x71, 0x4d, 0x97, 0x8c, 0xf6, 0xf7, 0x6c);
//"7852339B-4992-474C-BB87-B9 3B 2C 94 9C FE"
DEFINE_GUID(WinUSB_Guid, 0x7852339B, 0x4992, 0x474C, 0xBB, 0x87, 0xB9, 0x3B, 0x2C, 0x94, 0x9C, 0xFE);

#endif

typedef struct _WINUSB_PIPE_INFORMATION {
    USBD_PIPE_TYPE  PipeType;
    UCHAR           PipeId;
    USHORT          MaximumPacketSize;
    UCHAR           Interval;
} WINUSB_PIPE_INFORMATION, *PWINUSB_PIPE_INFORMATION;

#endif // (NTDDI_VERSION >= NTDDI_WINXP)

#endif // __WUSBIO_H__


