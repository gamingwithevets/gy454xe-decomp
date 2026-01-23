/**************************************************************

	 ML610CASESplus.H - SFR define Header file for CCU8

	 #CORE			  "nX-U8/100"
	 #TARGET			"ML610CASESplus"
	 #VERSION		  "1.01"
	 Last Edition  January 23, 2026
	 Copyright (C) 2026 GamingWithEvets Inc.
**************************************************************/
#ifndef _ML610CASESplus_H_
#define _ML610CASESplus_H_

typedef struct{
  unsigned char b0  : 1 ;
  unsigned char b1  : 1 ;
  unsigned char b2  : 1 ;
  unsigned char b3  : 1 ;
  unsigned char b4  : 1 ;
  unsigned char b5  : 1 ;
  unsigned char b6  : 1 ;
  unsigned char b7  : 1 ;
} _BYTE_FIELD;

/*---------------------------------------------------
	CODE ADDRESS SYMBOLS
---------------------------------------------------*/
#define WDTINT	0x0008
#define XI0INT	0x000A
#define XI1INT	0x000C
#define XI2INT	0x000E
#define XI3INT	0x0010
#define TM0INT	0x0012
#define L256SINT	0x0014
#define L1024SINT	0x0016
#define L4096SINT	0x0018
#define L16384SINT	0x001A
#define SIO0INT	0x001C
#define I2C0INT	0x001E
#define I2C1INT	0x0020
/*---------------------------------------------------
	END OF CODE ADDRESS SYMBOLS
---------------------------------------------------*/

/*---------------------------------------------------
	DATA ADDRESS SYMBOLS
---------------------------------------------------*/
#define DSR (*(volatile unsigned char __near *)0xF000)
#define _B_DSR (*(volatile _BYTE_FIELD __near *)0xF000)

#define LTBADJ (*(volatile unsigned int __near *)0xF006)

#define LTBADJL (*(volatile unsigned char __near *)0xF006)
#define _B_LTBADJL (*(volatile _BYTE_FIELD __near *)0xF006)

#define LTBADJH (*(volatile unsigned char __near *)0xF007)
#define _B_LTBADJH (*(volatile _BYTE_FIELD __near *)0xF007)

#define STPACP (*(volatile unsigned char __near *)0xF008)

#define SBYCON (*(volatile unsigned char __near *)0xF009)
#define _B_SBYCON (*(volatile _BYTE_FIELD __near *)0xF009)

#define FCON (*(volatile unsigned char __near *)0xF00A)
#define _B_FCON (*(volatile _BYTE_FIELD __near *)0xF00A)

#define LTBR (*(volatile unsigned char __near *)0xF00C)
#define _B_LTBR (*(volatile _BYTE_FIELD __near *)0xF00C)

#define HTBR (*(volatile unsigned char __near *)0xF00D)
#define _B_HTBR (*(volatile _BYTE_FIELD __near *)0xF00D0)

#define WDTCON (*(volatile unsigned char __near *)0xF00E)
#define _B_WDTCON (*(volatile _BYTE_FIELD __near *)0xF00E)

#define IE01 (*(volatile unsigned int __near *)0xF010)

#define IE0 (*(volatile unsigned char __near *)0xF010)
#define _B_IE0 (*(volatile _BYTE_FIELD __near *)0xF010)

#define IE1 (*(volatile unsigned char __near *)0xF011)
#define _B_IE1 (*(volatile _BYTE_FIELD __near *)0xF011)

#define IRQ0 (*(volatile unsigned char __near *)0xF014)
#define _B_IRQ0 (*(volatile _BYTE_FIELD __near *)0xF014)

#define IRQ1 (*(volatile unsigned char __near *)0xF015)
#define _B_IRQ1 (*(volatile _BYTE_FIELD __near *)0xF015)

#define EXICON0 (*(volatile unsigned char __near *)0xF018)
#define _B_EXICON0 (*(volatile _BYTE_FIELD __near *)0xF018)

#define TM0D (*(volatile unsigned int __near *)0xF020)

#define TM0D0 (*(volatile unsigned char __near *)0xF020)
#define _B_TM0D0 (*(volatile _BYTE_FIELD __near *)0xF020)

#define TM0D1 (*(volatile unsigned char __near *)0xF021)
#define _B_TM0D1 (*(volatile _BYTE_FIELD __near *)0xF021)

#define TM0C (*(volatile unsigned int __near *)0xF022)

#define TM0C0 (*(volatile unsigned char __near *)0xF022)
#define _B_TM0C0 (*(volatile _BYTE_FIELD __near *)0xF022)

#define TM0C1 (*(volatile unsigned char __near *)0xF023)
#define _B_TM0C1 (*(volatile _BYTE_FIELD __near *)0xF023)

#define TM0CON (*(volatile unsigned int __near *)0xF024)

#define TM0CON0 (*(volatile unsigned char __near *)0xF024)
#define _B_TM0CON0 (*(volatile _BYTE_FIELD __near *)0xF024)

#define TM1CON1 (*(volatile unsigned char __near *)0xF025)
#define _B_TM1CON1 (*(volatile _BYTE_FIELD __near *)0xF025)

#define BLKCON0 (*(volatile unsigned char __near *)0xF028)
#define _B_BLKCON0 (*(volatile _BYTE_FIELD __near *)0xF028)

#define KID (*(volatile unsigned char __near *)0xF040)
#define _B_KID (*(volatile _BYTE_FIELD __near *)0xF040)

#define KICON (*(volatile unsigned char __near *)0xF041)
#define _B_KICON (*(volatile _BYTE_FIELD __near *)0xF041)

#define KIMASK (*(volatile unsigned char __near *)0xF042)
#define _B_KIMASK (*(volatile _BYTE_FIELD __near *)0xF042)

#define KOMASK (*(volatile unsigned int __near *)0xF044)

#define KOMASK0 (*(volatile unsigned char __near *)0xF044)
#define _B_KOMASK0 (*(volatile _BYTE_FIELD __near *)0xF044)

#define KOMASK1 (*(volatile unsigned char __near *)0xF045)
#define _B_KOMASK1 (*(volatile _BYTE_FIELD __near *)0xF045)

#define KOD (*(volatile unsigned int __near *)0xF046)

#define KOD0 (*(volatile unsigned char __near *)0xF046)
#define _B_KOD0 (*(volatile _BYTE_FIELD __near *)0xF046)

#define KOD1 (*(volatile unsigned char __near *)0xF047)
#define _B_KOD1 (*(volatile _BYTE_FIELD __near *)0xF047)

#define P0D (*(volatile unsigned char __near *)0xF048)
#define _B_P0D (*(volatile _BYTE_FIELD __near *)0xF048)

#define P0MOD (*(volatile unsigned char __near *)0xF049)
#define _B_P0MOD (*(volatile _BYTE_FIELD __near *)0xF049)

#define P0CON (*(volatile unsigned int __near *)0xF04A)

#define P0CON0 (*(volatile unsigned char __near *)0xF04A)
#define _B_P0CON0 (*(volatile _BYTE_FIELD __near *)0xF04A)

#define P0CON1 (*(volatile unsigned char __near *)0xF04B)
#define _B_P0CON1 (*(volatile _BYTE_FIELD __near *)0xF04B)

#define P0DIR (*(volatile unsigned char __near *)0xF04C)
#define _B_P0DIR (*(volatile _BYTE_FIELD __near *)0xF04C)

#define P1D (*(volatile unsigned char __near *)0xF220)
#define _B_P1D (*(volatile _BYTE_FIELD __near *)0xF220)

#define P1DIR (*(volatile unsigned char __near *)0xF221)
#define _B_P1DIR (*(volatile _BYTE_FIELD __near *)0xF221)

#define P1CON (*(volatile unsigned int __near *)0xF222)

#define P1CON0 (*(volatile unsigned char __near *)0xF222)
#define _B_P1CON0 (*(volatile _BYTE_FIELD __near *)0xF222)

#define P1CON1 (*(volatile unsigned char __near *)0xF223)
#define _B_P1CON1 (*(volatile _BYTE_FIELD __near *)0xF223)

#define P1MOD (*(volatile unsigned char __near *)0xF224)
#define _B_P1MOD (*(volatile _BYTE_FIELD __near *)0xF224)

#define SIO0BUF (*(volatile unsigned int __near *)0xF280)

#define SIO0BUFL (*(volatile unsigned char __near *)0xF280)
#define _B_SIO0BUFL (*(volatile _BYTE_FIELD __near *)0xF280)

#define SIO0BUFH (*(volatile unsigned char __near *)0xF281)
#define _B_SIO0BUFH (*(volatile _BYTE_FIELD __near *)0xF281)

#define SIO0CON (*(volatile unsigned char __near *)0xF282)
#define _B_SIO0CON (*(volatile _BYTE_FIELD __near *)0xF282)

#define SIO0MOD (*(volatile unsigned int __near *)0xF284)

#define SIO0MOD0 (*(volatile unsigned char __near *)0xF284)
#define _B_SIO0MOD0 (*(volatile _BYTE_FIELD __near *)0xF284)

#define SIO0MOD1 (*(volatile unsigned char __near *)0xF285)
#define _B_SIO0MOD1 (*(volatile _BYTE_FIELD __near *)0xF285)

#define I2C0RD (*(volatile unsigned char __near *)0xF2A0)
#define _B_I2C0RD (*(volatile _BYTE_FIELD __near *)0xF2A0)

#define I2C0SA (*(volatile unsigned char __near *)0xF2A1)
#define _B_I2C0SA (*(volatile _BYTE_FIELD __near *)0xF2A1)

#define I2C0TD (*(volatile unsigned char __near *)0xF2A2)
#define _B_I2C0TD (*(volatile _BYTE_FIELD __near *)0xF2A2)

#define I2C0CON (*(volatile unsigned char __near *)0xF2A3)
#define _B_I2C0CON (*(volatile _BYTE_FIELD __near *)0xF2A3)

#define I2C0MOD (*(volatile unsigned char __near *)0xF2A4)
#define _B_I2C0MOD (*(volatile _BYTE_FIELD __near *)0xF2A4)

#define I2C0STAT (*(volatile unsigned char __near *)0xF2A5)
#define _B_I2C0STAT (*(volatile _BYTE_FIELD __near *)0xF2A5)

#define I2CSL (*(volatile unsigned char __near *)0xF2A7)
#define _B_I2CSL (*(volatile _BYTE_FIELD __near *)0xF2A7)

#define I2C1RD (*(volatile unsigned char __near *)0xF2A8)
#define _B_I2C1RD (*(volatile _BYTE_FIELD __near *)0xF2A8)

#define I2C1SA (*(volatile unsigned char __near *)0xF2A9)
#define _B_I2C1SA (*(volatile _BYTE_FIELD __near *)0xF2A9)

#define I2C1TD (*(volatile unsigned char __near *)0xF2AA)
#define _B_I2C1TD (*(volatile _BYTE_FIELD __near *)0xF2AA)

#define I2C1CON (*(volatile unsigned char __near *)0xF2AB)
#define _B_I2C1CON (*(volatile _BYTE_FIELD __near *)0xF2AB)

#define I2C1MOD (*(volatile unsigned char __near *)0xF2AC)
#define _B_I2C1MOD (*(volatile _BYTE_FIELD __near *)0xF2AC)

#define I2C0STAT (*(volatile unsigned char __near *)0xF2AD)
#define _B_I2C1STAT (*(volatile _BYTE_FIELD __near *)0xF2AD)

/*---------------------------------------------------
	END OF DATA ADDRESS SYMBOLS
---------------------------------------------------*/

/*---------------------------------------------------
	BIT ADDRESS SYMBOLS
---------------------------------------------------*/

#define DSR0			(_B_DSR.b0)

#define LADJ0			(_B_LTBADJL.b0)
#define LADJ1			(_B_LTBADJL.b1)
#define LADJ2			(_B_LTBADJL.b2)
#define LADJ3			(_B_LTBADJL.b3)
#define LADJ4			(_B_LTBADJL.b4)
#define LADJ5			(_B_LTBADJL.b5)
#define LADJ6			(_B_LTBADJL.b6)
#define LADJ7			(_B_LTBADJL.b7)

#define LADJ8			(_B_LTBADJH.b0)
#define LADJ9			(_B_LTBADJH.b1)
#define LADJS			(_B_LTBADJH.b2)

#define HLT				(_B_SBYCON.b0)
#define STP				(_B_SBYCON.b1)

#define XTM0			(_B_FCON0.b0)
#define XTM0			(_B_FCON0.b1)
#define SYSC0			(_B_FCON0.b2)
#define SYSC1			(_B_FCON0.b4)
#define SYSC2			(_B_FCON0.b5)

#define T64HZ			(_B_LTBR.b1)
#define T32HZ			(_B_LTBR.b2)
#define T16HZ			(_B_LTBR.b3)
#define T8HZ			(_B_LTBR.b4)
#define T4HZ			(_B_LTBR.b5)
#define T2HZ			(_B_LTBR.b6)
#define T1HZ			(_B_LTBR.b7)
#define THFHZ			(_B_LTBR.b7)

#define HT8KHZ		(_B_HTBR.b0)
#define HT4KHZ		(_B_HTBR.b1)
#define HT2KHZ		(_B_HTBR.b2)
#define HT1KHZ		(_B_HTBR.b3)

#define WDP				(_B_WDTCON.b0)

#define EWDT			(_B_IE0.b0)
#define EXI0			(_B_IE0.b1)
#define EXI1			(_B_IE0.b2)
#define EXI2			(_B_IE0.b3)
#define EXI3			(_B_IE0.b4)
#define ETM0			(_B_IE0.b5)
#define EL256S		(_B_IE0.b6)
#define EL1024S		(_B_IE0.b7)

#define EL4096S		(_B_IE1.b0)
#define EL16384S	(_B_IE1.b1)
#define ESIO0			(_B_IE1.b2)
#define EI2C0			(_B_IE1.b3)
#define EI2C1			(_B_IE1.b4)

#define QWDT			(_B_IRQ0.b0)
#define QXI0			(_B_IRQ0.b1)
#define QXI1			(_B_IRQ0.b2)
#define QXI2			(_B_IRQ0.b3)
#define QXI3			(_B_IRQ0.b4)
#define QTM0			(_B_IRQ0.b5)
#define QL256S		(_B_IRQ0.b6)
#define QL1024S		(_B_IRQ0.b7)

#define QL4096S		(_B_IRQ1.b0)
#define QL16384S	(_B_IRQ1.b1)
#define QSIO0			(_B_IRQ1.b2)
#define QI2C0			(_B_IRQ1.b3)
#define QI2C1			(_B_IRQ1.b4)

#define P00E0			(_B_EXICON0.b0)
#define P00E1			(_B_EXICON0.b1)
#define P01E0			(_B_EXICON0.b2)
#define P01E1			(_B_EXICON0.b3)
#define P02E0			(_B_EXICON0.b4)
#define P02E1			(_B_EXICON0.b5)
#define P03E0			(_B_EXICON0.b6)
#define P03E1			(_B_EXICON0.b7)

#define T0D0			(_B_TM0D0.b0)
#define T0D1			(_B_TM0D0.b1)
#define T0D2			(_B_TM0D0.b2)
#define T0D3			(_B_TM0D0.b3)
#define T0D4			(_B_TM0D0.b4)
#define T0D5			(_B_TM0D0.b5)
#define T0D6			(_B_TM0D0.b6)
#define T0D7			(_B_TM0D0.b7)
#define T0D8			(_B_TM0D1.b0)
#define T0D9			(_B_TM0D1.b1)
#define T0D10			(_B_TM0D1.b2)
#define T0D11			(_B_TM0D1.b3)
#define T0D12			(_B_TM0D1.b4)
#define T0D13			(_B_TM0D1.b5)
#define T0D14			(_B_TM0D1.b6)
#define T0D15			(_B_TM0D1.b7)

#define T0C0			(_B_TM0C0.b0)
#define T0C1			(_B_TM0C0.b1)
#define T0C2			(_B_TM0C0.b2)
#define T0C3			(_B_TM0C0.b3)
#define T0C4			(_B_TM0C0.b4)
#define T0C5			(_B_TM0C0.b5)
#define T0C6			(_B_TM0C0.b6)
#define T0C7			(_B_TM0C0.b7)
#define T0C8			(_B_TM0C1.b0)
#define T0C9			(_B_TM0C1.b1)
#define T0C10			(_B_TM0C1.b2)
#define T0C11			(_B_TM0C1.b3)
#define T0C12			(_B_TM0C1.b4)
#define T0C13			(_B_TM0C1.b5)
#define T0C14			(_B_TM0C1.b6)
#define T0C15			(_B_TM0C1.b7)

#define T0DIV0		(_B_TM0CON0.b0)
#define T0DIV1		(_B_TM0CON0.b1)
#define T0DIV2		(_B_TM0CON0.b2)
#define T0CS			(_B_TM0CON0.b3)

#define T0RUN			(_B_TM0CON1.b0)

#define DSIO0			(_B_BLKCON2.b0)
#define DI2C0			(_B_BLKCON2.b1)
#define DI2C1			(_B_BLKCON2.b2)
#define DTM0			(_B_BLKCON2.b3)

#define KI0				(_B_KID.b0)
#define KI1				(_B_KID.b1)
#define KI2				(_B_KID.b2)
#define KI3				(_B_KID.b3)
#define KI4				(_B_KID.b4)
#define KI5				(_B_KID.b5)
#define KI6				(_B_KID.b6)
#define KI7				(_B_KID.b7)

#define KIC0			(_B_KICON.b0)
#define KIC1			(_B_KICON.b1)
#define KIC2			(_B_KICON.b2)
#define KIC3			(_B_KICON.b3)
#define KIC4			(_B_KICON.b4)
#define KIC5			(_B_KICON.b5)
#define KIC6			(_B_KICON.b6)
#define KIC7			(_B_KICON.b7)

#define KIM0			(_B_KIMASK.b0)
#define KIM1			(_B_KIMASK.b1)
#define KIM2			(_B_KIMASK.b2)
#define KIM3			(_B_KIMASK.b3)
#define KIM4			(_B_KIMASK.b4)
#define KIM5			(_B_KIMASK.b5)
#define KIM6			(_B_KIMASK.b6)
#define KIM7			(_B_KIMASK.b7)

#define KOM0			(_B_KOMASK0.b0)
#define KOM1			(_B_KOMASK0.b1)
#define KOM2			(_B_KOMASK0.b2)
#define KOM3			(_B_KOMASK0.b3)
#define KOM4			(_B_KOMASK0.b4)
#define KOM5			(_B_KOMASK0.b5)
#define KOM6			(_B_KOMASK0.b6)
#define KOM7			(_B_KOMASK0.b7)

#define KOM8			(_B_KOMASK1.b0)
#define KOM9			(_B_KOMASK1.b1)

#define KOD0			(_B_KOD0.b0)
#define KOD1			(_B_KOD0.b1)
#define KOD2			(_B_KOD0.b2)
#define KOD3			(_B_KOD0.b3)
#define KOD4			(_B_KOD0.b4)
#define KOD5			(_B_KOD0.b5)
#define KOD6			(_B_KOD0.b6)
#define KOD7			(_B_KOD0.b7)

#define KOD8			(_B_KOD1.b0)
#define KOD9			(_B_KOD1.b1)
#define KOD15			(_B_KOD1.b7)

#define P01D			(_B_P0D.b0)
#define P02D			(_B_P0D.b1)
#define P03D			(_B_P0D.b2)

#define P01MD0		(_B_P0MOD.b0)

#define P01C0			(_B_P0CON0.b0)
#define P02C0			(_B_P0CON0.b1)
#define P03C0			(_B_P0CON0.b2)

#define P01C1			(_B_P0CON1.b0)
#define P02C1			(_B_P0CON1.b1)
#define P03C1			(_B_P0CON1.b2)

#define P01DIR		(_B_P0DIR.b0)
#define P02DIR		(_B_P0DIR.b1)
#define P03DIR		(_B_P0DIR.b2)

#define P10D			(_B_P1D.b0)
#define P11D			(_B_P1D.b1)
#define P12D			(_B_P1D.b2)
#define P13D			(_B_P1D.b3)
#define P14D			(_B_P1D.b4)
#define P15D			(_B_P1D.b5)
#define P16D			(_B_P1D.b6)

#define P10DIR		(_B_P0DIR.b0)
#define P11DIR		(_B_P0DIR.b1)
#define P12DIR		(_B_P0DIR.b2)
#define P13DIR		(_B_P0DIR.b3)
#define P14DIR		(_B_P0DIR.b4)
#define P15DIR		(_B_P0DIR.b5)
#define P16DIR		(_B_P0DIR.b6)

#define P10C0			(_B_P1CON0.b0)
#define P11C0			(_B_P1CON0.b1)
#define P12C0			(_B_P1CON0.b2)
#define P13C0			(_B_P1CON0.b3)
#define P14C0			(_B_P1CON0.b4)
#define P15C0			(_B_P1CON0.b5)
#define P16C0			(_B_P1CON0.b6)

#define P10C1			(_B_P1CON1.b0)
#define P11C1			(_B_P1CON1.b1)
#define P12C1			(_B_P1CON1.b2)
#define P13C1			(_B_P1CON1.b3)
#define P14C1			(_B_P1CON1.b4)
#define P15C1			(_B_P1CON1.b5)
#define P16C1			(_B_P1CON1.b6)

#define P10MD0		(_B_P1MOD0.b0)
#define P11MD0		(_B_P1MOD0.b1)
#define P12MD0		(_B_P1MOD0.b2)
#define P13MD0		(_B_P1MOD0.b3)
#define P14MD0		(_B_P1MOD0.b4)
#define P15MD0		(_B_P1MOD0.b5)
#define P16MD0		(_B_P1MOD0.b6)

#define S0B0			(_B_SIO0BUFL.b0)
#define S0B1			(_B_SIO0BUFL.b1)
#define S0B2			(_B_SIO0BUFL.b2)
#define S0B3			(_B_SIO0BUFL.b3)
#define S0B4			(_B_SIO0BUFL.b4)
#define S0B5			(_B_SIO0BUFL.b5)
#define S0B6			(_B_SIO0BUFL.b6)
#define S0B7			(_B_SIO0BUFL.b7)

#define S0B8			(_B_SIO0BUFH.b0)
#define S0B9			(_B_SIO0BUFH.b1)
#define S0B10			(_B_SIO0BUFH.b2)
#define S0B11			(_B_SIO0BUFH.b3)
#define S0B12			(_B_SIO0BUFH.b4)
#define S0B13			(_B_SIO0BUFH.b5)
#define S0B14			(_B_SIO0BUFH.b6)
#define S0B15			(_B_SIO0BUFH.b7)

#define S0EN			(_B_SIO0CON.b0)

#define S0DIR			(_B_SIO0MOD0.b0)
#define S0MD0			(_B_SIO0MOD0.b1)
#define S0MD1			(_B_SIO0MOD0.b2)
#define S0LG			(_B_SIO0MOD0.b3)

#define S0CK0			(_B_SIO0MOD1.b0)
#define S0CK1			(_B_SIO0MOD1.b1)
#define S0CK2			(_B_SIO0MOD1.b2)
#define S0CK3			(_B_SIO0MOD1.b3)
#define S0CKT			(_B_SIO0MOD1.b4)

#define I20R0			(_B_I2C0RD.b0)
#define I20R1			(_B_I2C0RD.b1)
#define I20R2			(_B_I2C0RD.b2)
#define I20R3			(_B_I2C0RD.b3)
#define I20R4			(_B_I2C0RD.b4)
#define I20R5			(_B_I2C0RD.b5)
#define I20R6			(_B_I2C0RD.b6)
#define I20R7			(_B_I2C0RD.b7)

#define I20RW			(_B_I2C0SA.b0)
#define I20A0			(_B_I2C0SA.b1)
#define I20A1			(_B_I2C0SA.b2)
#define I20A2			(_B_I2C0SA.b3)
#define I20A3			(_B_I2C0SA.b4)
#define I20A4			(_B_I2C0SA.b5)
#define I20A5			(_B_I2C0SA.b6)
#define I20A6			(_B_I2C0SA.b7)

#define I20T0			(_B_I2C0TD.b0)
#define I20T1			(_B_I2C0TD.b1)
#define I20T2			(_B_I2C0TD.b2)
#define I20T3			(_B_I2C0TD.b3)
#define I20T4			(_B_I2C0TD.b4)
#define I20T5			(_B_I2C0TD.b5)
#define I20T6			(_B_I2C0TD.b6)
#define I20T7			(_B_I2C0TD.b7)

#define I20ST			(_B_I2C0CON.b0)
#define I20SP			(_B_I2C0CON.b1)
#define I20RS			(_B_I2C0CON.b2)
#define I20ACT		(_B_I2C0CON.b7)

#define I20EN			(_B_I2C0MOD.b0)
#define I20MD			(_B_I2C0MOD.b1)
#define I20DW0		(_B_I2C0MOD.b2)
#define I20DW1		(_B_I2C0MOD.b3)
#define I20SYN		(_B_I2C0MOD.b4)

#define I20BB			(_B_I2C0STAT.b0)
#define I20ACR		(_B_I2C0STAT.b1)
#define I20ER			(_B_I2C0STAT.b2)

#define I2SL			(_B_I2CSL.b0)

#define I21R0			(_B_I2C1RD.b0)
#define I21R1			(_B_I2C1RD.b1)
#define I21R2			(_B_I2C1RD.b2)
#define I21R3			(_B_I2C1RD.b3)
#define I21R4			(_B_I2C1RD.b4)
#define I21R5			(_B_I2C1RD.b5)
#define I21R6			(_B_I2C1RD.b6)
#define I21R7			(_B_I2C1RD.b7)

#define I21RW			(_B_I2C1SA.b0)
#define I21A0			(_B_I2C1SA.b1)
#define I21A1			(_B_I2C1SA.b2)
#define I21A2			(_B_I2C1SA.b3)
#define I21A3			(_B_I2C1SA.b4)
#define I21A4			(_B_I2C1SA.b5)
#define I21A5			(_B_I2C1SA.b6)
#define I21A6			(_B_I2C1SA.b7)

#define I21T0			(_B_I2C1TD.b0)
#define I21T1			(_B_I2C1TD.b1)
#define I21T2			(_B_I2C1TD.b2)
#define I21T3			(_B_I2C1TD.b3)
#define I21T4			(_B_I2C1TD.b4)
#define I21T5			(_B_I2C1TD.b5)
#define I21T6			(_B_I2C1TD.b6)
#define I21T7			(_B_I2C1TD.b7)

#define I21WT			(_B_I2C1CON0.b5)
#define I21ACT		(_B_I2C1CON0.b7)

#define I21EN			(_B_I2C1MODL.b0)
#define I21PIE		(_B_I2C1MODL.b5)
#define I21SIE		(_B_I2C1MODL.b6)

#define I21CD0		(_B_I2C1MODH.b0)
#define I21CD1		(_B_I2C1MODH.b1)

#define I21BB			(_B_I2C1STAT.b0)
#define I21ACR		(_B_I2C1STAT.b1)
#define I21ER			(_B_I2C1STAT.b2)
#define I21SAA		(_B_I2C1STAT.b3)
#define I21TR			(_B_I2C1STAT.b4)
/*---------------------------------------------------
	END OF BIT ADDRESS SYMBOLS
---------------------------------------------------*/

#endif
