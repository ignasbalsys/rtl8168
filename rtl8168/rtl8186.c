/*++

Copyright (c) 1990-98  Microsoft Corporation All Rights Reserved

Module Name:

    sioctl.c

Abstract:

    Purpose of this driver is to demonstrate how the four different types
    of IOCTLs can be used, and how the I/O manager handles the user I/O
    buffers in each case. This sample also helps to understand the usage of
    some of the memory manager functions.

Environment:

    Kernel mode only.

--*/


//
// Include files.
//

#include <ntddk.h>     
#include <string.h>

#include "rtl8168.h"

#define NT_DEVICE_NAME      L"\\Device\\Rtl8168E"
#define DOS_DEVICE_NAME     L"\\DosDevices\\Rtl8168E"

#if DBG
#define RTL_KDPRINT(_x_) \
                DbgPrint("Rtl8168.sys: ");\
                DbgPrint _x_;

#else
#define RTL_KDPRINT(_x_)
#endif

//
// Device driver routine declarations.
//

DRIVER_INITIALIZE DriverEntry;

_Dispatch_type_(IRP_MJ_CREATE)
_Dispatch_type_(IRP_MJ_CLOSE)
DRIVER_DISPATCH Rtl8168CreateClose;

_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH Rtl8168DeviceControl;

DRIVER_UNLOAD Rtl8168UnloadDriver;

VOID
PrintIrpInfo(
    PIRP Irp
);
VOID
PrintChars(
    _In_reads_(CountChars) PCHAR BufferAddress,
    _In_ size_t CountChars
);

#ifdef ALLOC_PRAGMA
#pragma alloc_text( INIT, DriverEntry )
#pragma alloc_text( PAGE, Rtl8168CreateClose)
#pragma alloc_text( PAGE, Rtl8168DeviceControl)
#pragma alloc_text( PAGE, Rtl8168UnloadDriver)
#pragma alloc_text( PAGE, PrintIrpInfo)
#pragma alloc_text( PAGE, PrintChars)
#endif // ALLOC_PRAGMA


NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT   DriverObject,
    _In_ PUNICODE_STRING      RegistryPath
)
/*++

Routine Description:
    This routine is called by the Operating System to initialize the driver.

    It creates the device object, fills in the dispatch entry points and
    completes the initialization.

Arguments:
    DriverObject - a pointer to the object that represents this device
    driver.

    RegistryPath - a pointer to our Services key in the registry.

Return Value:
    STATUS_SUCCESS if initialized; an error otherwise.

--*/

{
    NTSTATUS        Status;
    UNICODE_STRING  NtDeviceName;    
    UNICODE_STRING  Win32DeviceName;    
    PDEVICE_OBJECT  DeviceObject = NULL;    

    UNREFERENCED_PARAMETER(RegistryPath);

    RtlInitUnicodeString(&NtDeviceName, NT_DEVICE_NAME);

    Status = IoCreateDevice(
        DriverObject,                   // Our Driver Object
        0,                              // We don't use a device extension
        &NtDeviceName,                  // Device name "\Device\SIOCTL"
        FILE_DEVICE_UNKNOWN,            // Device type
        FILE_DEVICE_SECURE_OPEN,        // Device characteristics
        FALSE,                          // Not an exclusive device
        &DeviceObject);                // Returned ptr to Device Object

    if (!NT_SUCCESS(Status))
    {
        RTL_KDPRINT(("Couldn't create the device object\n"));
        return Status;
    }

    //
    // Initialize the driver object with this driver's entry points.
    //

    DriverObject->MajorFunction[IRP_MJ_CREATE] = Rtl8168CreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = Rtl8168CreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Rtl8168DeviceControl;
    DriverObject->DriverUnload = Rtl8168UnloadDriver;

    //
    // Initialize a Unicode String containing the Win32 name
    // for our device.
    //

    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);

    //
    // Create a symbolic link between our device name  and the Win32 name
    //

    Status = IoCreateSymbolicLink(
        &Win32DeviceName, &NtDeviceName);

    if (!NT_SUCCESS(Status))
    {
        //
        // Delete everything that this routine has allocated.
        //
        RTL_KDPRINT(("Couldn't create symbolic link\n"));
        IoDeleteDevice(DeviceObject);
    }

    RTL_KDPRINT(("Hello from kernel mode"));

    return Status;
}


NTSTATUS
Rtl8168CreateClose(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
/*++

Routine Description:

    This routine is called by the I/O system when the SIOCTL is opened or
    closed.

    No action is performed other than completing the request successfully.

Arguments:

    DeviceObject - a pointer to the object that represents the device
    that I/O is to be done on.

    Irp - a pointer to the I/O Request Packet for this request.

Return Value:

    NT status code

--*/

{
    UNREFERENCED_PARAMETER(DeviceObject);

    PAGED_CODE();

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

VOID
Rtl8168UnloadDriver(
    _In_ PDRIVER_OBJECT DriverObject
)
/*++

Routine Description:

    This routine is called by the I/O system to unload the driver.

    Any resources previously allocated must be freed.

Arguments:

    DriverObject - a pointer to the object that represents our driver.

Return Value:

    None
--*/

{
    PDEVICE_OBJECT DeviceObject = DriverObject->DeviceObject;
    UNICODE_STRING Win32DeviceName;

    PAGED_CODE();

    //
    // Create counted string version of our Win32 device name.
    //

    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);


    //
    // Delete the link from our device name to a name in the Win32 namespace.
    //

    IoDeleteSymbolicLink(&Win32DeviceName);

    if (DeviceObject != NULL)
    {
        IoDeleteDevice(DeviceObject);
    }



}

NTSTATUS
Rtl8168DeviceControl(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)

/*++

Routine Description:

    This routine is called by the I/O system to perform a device I/O
    control function.

Arguments:

    DeviceObject - a pointer to the object that represents the device
        that I/O is to be done on.

    Irp - a pointer to the I/O Request Packet for this request.

Return Value:

    NT status code

--*/

{
    PIO_STACK_LOCATION  IrpSp;
    NTSTATUS            Status = STATUS_SUCCESS;
    ULONG               OutputBufferLength; 
    size_t              DataLength = 0xFF;
    PCHAR               Buffer = NULL;

    UNREFERENCED_PARAMETER(DeviceObject);

    PAGED_CODE();

    IrpSp = IoGetCurrentIrpStackLocation(Irp);
    OutputBufferLength = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;

    if (!OutputBufferLength)
    {
        Status = STATUS_INVALID_PARAMETER;
        goto End;
    }


    //
    // Determine which I/O control code was specified.
    //

    switch (IrpSp->Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_R8168_GET_REGS:

        //
        // In this type of transfer, the I/O manager allocates a system buffer
        // large enough to accommodate the User input buffer, sets the buffer address
        // in Irp->AssociatedIrp.SystemBuffer and copies the content of user input buffer
        // into the SystemBuffer. For the output buffer, the I/O manager
        // probes to see whether the virtual address is writable in the callers
        // access mode, locks the pages in memory and passes the pointer to MDL
        // describing the buffer in Irp->MdlAddress.
        //
        if (OutputBufferLength < DataLength)
        {
            /* Truncate */
            DataLength = OutputBufferLength;
        }

        RTL_KDPRINT(("Called IOCTL_R8168_GET_REGS\n"));

        PrintIrpInfo(Irp);


        //
        // To access the output buffer, just get the system address
        // for the buffer. For this method, this buffer is intended for transfering data
        // from the driver to the application.
        //

        Buffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority | MdlMappingNoExecute);

        if (!Buffer) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        ULONG Rtl8168BAR[6] = { 0 };

        ULONG BytesRead = HalGetBusDataByOffset(
            PCIConfiguration,
            1,
            0,
            (PVOID)Rtl8168BAR,
            0x10,
            6 * 4
        );

        if (BytesRead != 6 * 4)
        {
            Status = STATUS_UNSUCCESSFUL;
            goto End;
        }

        for (int i = 0; i < 6; i++)
        {
            DbgPrint("BAR%u: %x\n", i, Rtl8168BAR[i]);
        }

        /* Check if RTL8168 BAR2 is MMIO */
        if (Rtl8168BAR[2] & 1)
        {
            /* I/O BAR */
            Status = STATUS_UNSUCCESSFUL;
            goto End;
        }

        PHYSICAL_ADDRESS Pa = { 0 };
        if ((Rtl8168BAR[2] & 0x6) == 0x4)
        {
            /* 64 bit BAR */
            Pa.QuadPart = ((ULONGLONG)Rtl8168BAR[3] << 32) | (Rtl8168BAR[2] & 0xFFFFFFF0);
        } 
        else
        {
            /* 32 bit BAR */
            Pa.QuadPart = Rtl8168BAR[2] & 0xFFFFFFF0;
        }

        PVOID RegVa = MmMapIoSpaceEx(Pa, 0xFF, PAGE_READWRITE | PAGE_NOCACHE);
        if ( !RegVa )
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto End;
        }


        //
        // Write data to be sent to the user in this buffer
        //

        RtlCopyBytes(Buffer, RegVa, DataLength);

        MmUnmapIoSpace(RegVa, 0xFF);


        Irp->IoStatus.Information = (OutputBufferLength < DataLength ? OutputBufferLength : DataLength);

        //
        // NOTE: Changes made to the  SystemBuffer are not copied
        // to the user input buffer by the I/O manager
        //

        break;

    default:

        //
        // The specified I/O control code is unrecognized by this driver.
        //

        Status = STATUS_INVALID_DEVICE_REQUEST;
        RTL_KDPRINT(("ERROR: unrecognized IOCTL %x\n",
            IrpSp->Parameters.DeviceIoControl.IoControlCode));
        break;
    }

End:
    //
    // Finish the I/O operation by simply completing the packet and returning
    // the same status as in the packet itself.
    //

    Irp->IoStatus.Status = Status;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Status;
}

VOID
PrintIrpInfo(
    PIRP Irp)
{
    PIO_STACK_LOCATION  irpSp;
    irpSp = IoGetCurrentIrpStackLocation(Irp);

    PAGED_CODE();

    RTL_KDPRINT(("\tIrp->AssociatedIrp.SystemBuffer = 0x%p\n",
        Irp->AssociatedIrp.SystemBuffer));
    RTL_KDPRINT(("\tIrp->UserBuffer = 0x%p\n", Irp->UserBuffer));
    RTL_KDPRINT(("\tirpSp->Parameters.DeviceIoControl.Type3InputBuffer = 0x%p\n",
        irpSp->Parameters.DeviceIoControl.Type3InputBuffer));
    RTL_KDPRINT(("\tirpSp->Parameters.DeviceIoControl.InputBufferLength = %d\n",
        irpSp->Parameters.DeviceIoControl.InputBufferLength));
    RTL_KDPRINT(("\tirpSp->Parameters.DeviceIoControl.OutputBufferLength = %d\n",
        irpSp->Parameters.DeviceIoControl.OutputBufferLength));
    return;
}

VOID
PrintChars(
    _In_reads_(CountChars) PCHAR BufferAddress,
    _In_ size_t CountChars
)
{
    PAGED_CODE();

    if (CountChars) {

        while (CountChars--) {

            if (*BufferAddress > 31
                && *BufferAddress != 127) {

                KdPrint(("%c", *BufferAddress));

            }
            else {

                KdPrint(("."));

            }
            BufferAddress++;
        }
        KdPrint(("\n"));
    }
    return;
}

