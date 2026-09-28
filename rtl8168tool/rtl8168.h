/*++

Copyright (c) 1997  Microsoft Corporation

Module Name:

    SIOCTL.H

Abstract:


    Defines the IOCTL codes that will be used by this driver.  The IOCTL code
    contains a command identifier, plus other information about the device,
    the type of access with which the file must have been opened,
    and the type of buffering.

Environment:

    Kernel mode only.

--*/

//
// Device type           -- in the "User Defined" range."
//
#define R8168_TYPE 40000
//
// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
//

#define IOCTL_R8168_GET_REGS \
    CTL_CODE( R8168_TYPE, 0x901, METHOD_OUT_DIRECT , FILE_READ_ACCESS  )




#define DRIVER_FUNC_INSTALL     0x01



