#include <phnt_windows.h>
#include <phnt.h>
#include <stdio.h>

#include "rtl8168.h"
#include "rtl8168e_regs.h"

#define PRINT(buf,cnt,fmt,...) memset(buf, 0, cnt*sizeof(wchar_t)); swprintf(buf, cnt, fmt, ...);

NTSTATUS Print(WCHAR *Str, ULONG Cnt, WCHAR *Fmt, ...)
{
    NTSTATUS Status = STATUS_SUCCESS;
    IO_STATUS_BLOCK Iosb = { 0 };

    va_list args;
    va_start(args, Fmt);

    vswprintf(Str, Cnt, Fmt, args);

    Status = NtWriteFile(
        NtCurrentPeb()->ProcessParameters->StandardOutput,
        NULL,
        NULL,
        NULL,
        &Iosb,
        Str,
        (lstrlenW(Str) + 1) * sizeof(WCHAR),
        NULL,
        NULL
    );

    if (Status == STATUS_PENDING)
    {
        Status = NtWaitForSingleObject(NtCurrentPeb()->ProcessParameters->StandardOutput, FALSE, 0);
        if (!NT_SUCCESS(Status))
        {
            /* Wait is not finished */
            return Status;
        }

    }
    else if (!NT_SUCCESS(Status))
    {
        return Status;
    }

    va_end(args);

    return Status;
}

int main()
{
    NTSTATUS Status = STATUS_SUCCESS;
    HANDLE Device = NULL;
    IO_STATUS_BLOCK Iosb = { 0 };
    OBJECT_ATTRIBUTES Oa = { 0 };
    UNICODE_STRING DeviceName;

    RtlInitUnicodeString(&DeviceName, L"\\Device\\Rtl8168E");

    InitializeObjectAttributes(&Oa, &DeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    Status = NtCreateFile(
        &Device,
        FILE_READ_DATA,
        &Oa,
        &Iosb,
        0,
        0,
        FILE_SHARE_READ,
        FILE_OPEN,
        0,
        NULL,
        0);
    
    if (!NT_SUCCESS(Status))
    {
        return Status;
    }

    char buf[0xFF] = { 0 };

    Status = NtDeviceIoControlFile(
        Device,
        NULL,
        NULL,
        NULL,
        &Iosb,
        IOCTL_R8168_GET_REGS,
        NULL,
        0,
        buf,
        0xFF
        );

    if (Status == STATUS_PENDING)
    {
        Status = NtWaitForSingleObject(Device, FALSE, 0);
        if (!NT_SUCCESS(Status))
        {
            /* Wait is not finished */
            return Status;
        }

    }
    else if(!NT_SUCCESS(Status))
    {
        return Status;
    }


    Status = NtWriteFile(
        NtCurrentPeb()->ProcessParameters->StandardOutput,
        NULL,
        NULL,
        NULL,
        &Iosb,
        buf,
        0xFF,
        NULL,
        NULL
    );

    if (Status == STATUS_PENDING)
    {
        Status = NtWaitForSingleObject(NtCurrentPeb()->ProcessParameters->StandardOutput, FALSE, 0);
        if (!NT_SUCCESS(Status))
        {
            /* Wait is not finished */
            return Status;
        }

    }
    else if (!NT_SUCCESS(Status))
    {
        return Status;
    }

    WCHAR Str[4096] = { 0 };
    Print(Str, 4096, L"\nEthernet ID/MAC address register\n", 0);
    Print(Str, 4096, L"IDR0: %02hhx\n", buf[IDR0]);
    Print(Str, 4096, L"IDR1: %02hhx\n", buf[IDR1]);
    Print(Str, 4096, L"IDR2: %02hhx\n", buf[IDR2]);
    Print(Str, 4096, L"IDR3: %02hhx\n", buf[IDR3]);
    Print(Str, 4096, L"IDR4: %02hhx\n", buf[IDR4]);
    Print(Str, 4096, L"IDR5: %02hhx\n", buf[IDR5]);
    Print(Str, 4096, L"IDR6: %02hhx\n", buf[IDR6]);
    Print(Str, 4096, L"IDR7: %02hhx\n", buf[IDR7]);

    Print(Str, 4096, L"Multicast address register\n", 0);
    Print(Str, 4096, L"MAR7: %02hhx\n", buf[MAR7]);
    Print(Str, 4096, L"MAR6: %02hhx\n", buf[MAR6]);
    Print(Str, 4096, L"MAR5: %02hhx\n", buf[MAR5]);
    Print(Str, 4096, L"MAR4: %02hhx\n", buf[MAR4]);
    Print(Str, 4096, L"MAR3: %02hhx\n", buf[MAR3]);
    Print(Str, 4096, L"MAR2: %02hhx\n", buf[MAR2]);
    Print(Str, 4096, L"MAR1: %02hhx\n", buf[MAR1]);
    Print(Str, 4096, L"MAR0: %02hhx\n", buf[MAR0]);

    Print(Str, 4096, L"Dump tally counter command register\n", 0);
    Print(Str, 4096, L"DTCCR_LOW: %08x\n", *((ULONG*)&buf[DTCCR_LOW]));
    Print(Str, 4096, L"DTCCR_HIGH: %08x\n", *((ULONG*)&buf[DTCCR_HIGH]));

    Print(Str, 4096, L"\nCUSTOM_LED: %08x\n\n", *((ULONG*)&buf[CUSTOM_LED]));

    Print(Str, 4096, L"Transmit normal priority descriptiors: start address\n", 0);
    Print(Str, 4096, L"TNPDS_LOW: %08x\n", *((ULONG*)&buf[TNPDS_LOW]));
    Print(Str, 4096, L"TNPDS_HIGH: %08x\n", *((ULONG*)&buf[TNPDS_HIGH]));

    Print(Str, 4096, L"\nTransmit high priority descriptiors: start address\n", 0);
    Print(Str, 4096, L"THPDS_LOW: %08x\n", *((ULONG*)&buf[THPDS_LOW]));
    Print(Str, 4096, L"THPDS_HIGH: %08x\n", *((ULONG*)&buf[THPDS_HIGH]));

    Print(Str, 4096, L"\nFLASH: %08x\n", *((ULONG*)&buf[FLASH]));
    Print(Str, 4096, L"ERSR: %02hhx\n", buf[ERSR]);

    Print(Str, 4096, L"\nCommand register\n", 0);
    Print(Str, 4096, L"CR: %02hhx\n", buf[CR]);

    Print(Str, 4096, L"\nTransmit priority polling register\n", 0);
    Print(Str, 4096, L"TPPoll: %02hhx\n", buf[TPPoll]);

    Print(Str, 4096, L"\nInterrupt mask register\n", 0);
    Print(Str, 4096, L"IMR: %04hx\n", *((SHORT*)&buf[IMR]));

    Print(Str, 4096, L"\nInterrupt status register\n", 0);
    Print(Str, 4096, L"ISR: %04hx\n", *((SHORT*)&buf[ISR]));

    Print(Str, 4096, L"\nTransmit (Tx) configuration register\n", 0);
    Print(Str, 4096, L"TCR: %08x\n", *((ULONG*)&buf[TCR]));

    Print(Str, 4096, L"\nReceive (Rx) configuration register\n", 0);
    Print(Str, 4096, L"RCR: %08x\n", *((ULONG*)&buf[RCR]));

    NtClose(Device);

	return 0;
}