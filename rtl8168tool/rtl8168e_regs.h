#ifndef _RTL8168E_REGS_
#define _RTL8168E_REGS_

/* Ethernet ID/MAC address register */
/* Initialized from EPPROM EthernetID field, 4 byte writes, 1/2/4 byte reads (RW) */
#define IDR0 0x0 /* write this */
#define IDR1 0x1
#define IDR2 0x2
#define IDR3 0x3
#define IDR4 0x4 /* write this */
#define IDR5 0x5
#define IDR6 0x6 /* Reserved? */
#define IDR7 0x7 /* Reserved? */

/* Multicast address register */
/* Initialized by driver, 4 byte writes, 1/2/4 byte reads (RW)*/
#define MAR7 0x8
#define MAR6 0x9
#define MAR5 0xA
#define MAR4 0xB
#define MAR3 0xC
#define MAR2 0xD
#define MAR1 0xE
#define MAR0 0xF

/* Dump tally counter command register (64 byte alignment) (RW) */
#define DTCCR_LOW 0x10 
#define DTCCR_HIGH 0x14

/* Custom LED ? */
#define CUSTOM_LED 0x18
#define RESERVED 0x1C

/* Transmit normal priority descriptiors: start address (64 bit) (256 byte alignment) (RW) */
#define TNPDS_LOW 0x20
#define TNPDS_HIGH 0x24

/* Transmit high priority descriptiors: start address (64 bit) (256 byte alignment) (RW) */
#define THPDS_LOW 0x28
#define THPDS_HIGH 0x2C

#define FLASH 0x30 /* (4-5 bytes??) */
#define ERSR 0x36

/* Command register (RW) */
#define CR 0x37

/* Transmit priority polling register (W) */
#define TPPoll 0x38

#define RESERVED1 0x39 
#define RESERVED2 0x3a
#define RESERVED3 0x3b

/* Interrupt mask register (16 bit) (RW) */
#define IMR 0x3C

/* Interrupt status register (16 bit) (RW) */
#define ISR 0x3E

/* Transmit (Tx) configuration register (32 bit) (RW) */
#define TCR 0x40

/* Receive (Rx) configuration register (32 bit) (RW) */
#define RCR 0x44

/* Timer CounT Register (32 bit) (RW) */
/* Write to reset, general purpose timer */
#define TCTR 0x48

#define RESERVED4 0x4C

/* 93C46 Command register  (RW) */
#define _9346CR 0x50

/* Configuration register  (RW) */
#define CONFIG0 0x51
#define CONFIG1 0x52
#define CONFIG2 0x53
#define CONFIG3 0x54
#define CONFIG4 0x55
#define CONFIG5 0x56

#define TDFNR 0x57

/* Timer interrupt register (32 bit) (RW) */
/* Writing non zero value, timeout bit in ISR will be set when TCTR reaches written value */
#define TimerInt0 0x58
#define TimerInt1 0x5C

/* PHY access register (32 bit) (RW) */
#define PHYAR 0x60

/* (32 bit) */
#define CSIDR 0x64
#define CSIAR 0x68

/* PHY status register (32 bit) (RW) */
#define PHYStatus 0x6C

/* Not documented */
#define MACDBG 0x6D
#define GPIO 0x6E
#define PMCH 0x6F
#define ERIDR 0x70
#define ERIAR 0x74
#define EPHY_RXER_NUM 0x7C
#define EPHYAR 0x80

/* Power management wakeup frame 0 (32 bit) */
#define Wakeup0_LOW 0x84
#define Wakeup0_HIGH 0x88

/* Power management wakeup frame 1 (32 bit) */
#define Wakeup1_LOW 0x8c
#define Wakeup1_HIGH 0x90


/*
0094h–009Bh Wakeup2LD RW Power Management wakeup frame2 (128bit), Low DWord
009Ch–00A3h Wakeup2HD RW Power Management wakeup frame2, High DWord
00A4h–00ABh Wakeup3LD RW Power Management wakeup frame3 (128bit), Low DWord
00ACh–00B3h Wakeup3HD RW Power Management wakeup frame3, High DWord
00B4h–00BBh Wakeup4LD RW Power Management wakeup frame4 (128bit), Low DWord
00BCh–00C3h Wakeup4HD RW Power Management wakeup frame4, High DWord

        IntrMask1       = 0x84,
        IntrMask2       = 0x85,
        IntrStatus1     = 0x86,
        IntrStatus2     = 0x87,
        TimeInt2        = 0x8C,
        Rss_indir_tbl   = 0x90,
        OCPDR           = 0xB0,
        MACOCP          = 0xB0,
        OCPAR           = 0xB4,
        SecMAC0         = 0xB4,
        SecMAC4         = 0xB8,
        PHYOCP          = 0xB8,
        IntrMask3       = 0xC0,
        IntrStatus3     = 0xC1,
        DBG_reg         = 0xD1,
        TwiCmdReg       = 0xD2,
        MCUCmd_reg      = 0xD3,
        RxMaxSize       = 0xDA,
        EFUSEAR         = 0xDC,
        CPlusCmd        = 0xE0,
        IntrMitigate    = 0xE2,
        RxDescAddrLow   = 0xE4,
        RxDescAddrHigh  = 0xE8,
        MTPS            = 0xEC,
        FuncEvent       = 0xF0,
        PPSW            = 0xF2,
        FuncEventMask   = 0xF4,
        TimeInt3        = 0xF4,
        FuncPresetState = 0xF8,
        CMAC_IBCR0      = 0xF8,
        CMAC_IBCR2      = 0xF9,
        CMAC_IBIMR0     = 0xFA,
        CMAC_IBISR0     = 0xFB,
        FuncForceEvent  = 0xFC,
*/

#endif 