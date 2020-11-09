//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8UB1_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
#include "efm8_usb.h"
#include "descriptors.h"
#include "usb_0.h"
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void) {
	// $[Config Calls]
	// Save the SFRPAGE
	uint8_t SFRPAGE_save = SFRPAGE;
	WDT_0_enter_DefaultMode_from_RESET();
	VREG_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CIP51_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	SPI_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	USBLIB_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$

}

extern void WDT_0_enter_DefaultMode_from_RESET(void) {
	// $[Watchdog Timer Init Variable Declarations]
	uint32_t i;
	bool ea;
	// [Watchdog Timer Init Variable Declarations]$

	// $[WDTCN - Watchdog Timer Control]
	// Deprecated
	// [WDTCN - Watchdog Timer Control]$

	// $[WDTCN_2 - Watchdog Timer Control]
	SFRPAGE = 0x00;

	// Feed WDT timer before disabling (Erratum WDT_E102)
	WDTCN = 0xA5;

	// Add 2 LFO cycle delay before disabling WDT (Erratum WDT_E102)
	for (i = 0; i < (2 * 3062500UL) / (80000 * 3); i++) {
		NOP();
	}

	// Disable WDT
	ea = IE_EA;
	IE_EA = 0;
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	IE_EA = ea;

	// [WDTCN_2 - Watchdog Timer Control]$

}

extern void VREG_0_enter_DefaultMode_from_RESET(void) {
	// $[REG0CN - Voltage Regulator 0 Control]
	// [REG0CN - Voltage Regulator 0 Control]$

	// $[REG1CN - Voltage Regulator 1 Control]
	/***********************************************************************
	 - Regulator is enabled
	 - Regulator bias is disabled
	 - The 5V regulator is in normal power mode
	 ***********************************************************************/
	SFRPAGE = 0x20;
	REG1CN = REG1CN_REG1ENB__ENABLED | REG1CN_BIASENB__DISABLED
			| REG1CN_SUSEN__NORMAL;
	// [REG1CN - Voltage Regulator 1 Control]$

}

extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	/***********************************************************************
	 - P0.0 is high. Set P0.0 to drive or float high
	 - P0.1 is high. Set P0.1 to drive or float high
	 - P0.2 is high. Set P0.2 to drive or float high
	 - P0.3 is low. Set P0.3 to drive low
	 - P0.4 is high. Set P0.4 to drive or float high
	 - P0.5 is low. Set P0.5 to drive low
	 - P0.6 is low. Set P0.6 to drive low
	 - P0.7 is high. Set P0.7 to drive or float high
	 ***********************************************************************/
	SFRPAGE = 0x00;
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__LOW | P0_B4__HIGH
			| P0_B5__LOW | P0_B6__LOW | P0_B7__HIGH;
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
	/***********************************************************************
	 - P0.0 output is open-drain
	 - P0.1 output is open-drain
	 - P0.2 output is open-drain
	 - P0.3 output is push-pull
	 - P0.4 output is open-drain
	 - P0.5 output is push-pull
	 - P0.6 output is push-pull
	 - P0.7 output is push-pull
	 ***********************************************************************/
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
			| P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__PUSH_PULL
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__PUSH_PULL
			| P0MDOUT_B6__PUSH_PULL | P0MDOUT_B7__PUSH_PULL;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/***********************************************************************
	 - P0.0 pin is skipped by the crossbar
	 - P0.1 pin is skipped by the crossbar
	 - P0.2 pin is skipped by the crossbar
	 - P0.3 pin is not skipped by the crossbar
	 - P0.4 pin is not skipped by the crossbar
	 - P0.5 pin is not skipped by the crossbar
	 - P0.6 pin is not skipped by the crossbar
	 - P0.7 pin is not skipped by the crossbar
	 ***********************************************************************/
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
			| P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED
			| P0SKIP_B5__NOT_SKIPPED | P0SKIP_B6__NOT_SKIPPED
			| P0SKIP_B7__NOT_SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
	// $[XBR2 - Port I/O Crossbar 2]
	/***********************************************************************
	 - Weak Pullups enabled 
	 - Crossbar enabled
	 - UART1 I/O unavailable at Port pin
	 - UART1 RTS1 unavailable at Port pin
	 - UART1 CTS1 unavailable at Port pin
	 ***********************************************************************/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED
			| XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
			| XBR2_URT1CTSE__DISABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[PRTDRV - Port Drive Strength]
	// [PRTDRV - Port Drive Strength]$

	// $[XBR0 - Port I/O Crossbar 0]
	/***********************************************************************
	 - UART0 I/O unavailable at Port pin
	 - SPI I/O routed to Port pins
	 - SMBus 0 I/O unavailable at Port pins
	 - CP0 unavailable at Port pin
	 - Asynchronous CP0 unavailable at Port pin
	 - CP1 unavailable at Port pin
	 - Asynchronous CP1 unavailable at Port pin
	 - SYSCLK unavailable at Port pin
	 ***********************************************************************/
	XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__ENABLED | XBR0_SMB0E__DISABLED
			| XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
			| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$

}

extern void CIP51_0_enter_DefaultMode_from_RESET(void) {
	// $[PFE0CN - Prefetch Engine Control]
	/***********************************************************************
	 - Enable the prefetch engine 
	 - SYSCLK < 50 MHz
	 ***********************************************************************/
	SFRPAGE = 0x10;
	PFE0CN = PFE0CN_PFEN__ENABLED | PFE0CN_FLRT__SYSCLK_BELOW_50_MHZ;
	// [PFE0CN - Prefetch Engine Control]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[HFOSC1 Setup]
	// Ensure SYSCLK is > 24 MHz before switching to HFOSC1
	SFRPAGE = 0x00;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
		;
	// [HFOSC1 Setup]$

	// $[CLKSEL - Clock Select]
	/***********************************************************************
	 - Clock derived from the Internal High Frequency Oscillator 1
	 - SYSCLK is equal to selected clock source divided by 1
	 ***********************************************************************/
	CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
		;
	// [CLKSEL - Clock Select]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	// [EIE1 - Extended Interrupt Enable 1]$

	// $[EIE2 - Extended Interrupt Enable 2]
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP1H - Extended Interrupt Priority 1 High]
	// [EIP1H - Extended Interrupt Priority 1 High]$

	// $[EIP1 - Extended Interrupt Priority 1 Low]
	// [EIP1 - Extended Interrupt Priority 1 Low]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$

	// $[EIP2H - Extended Interrupt Priority 2 High]
	// [EIP2H - Extended Interrupt Priority 2 High]$

	// $[IE - Interrupt Enable]
	/***********************************************************************
	 - Enable each interrupt according to its individual mask setting
	 - Disable external interrupt 0
	 - Disable external interrupt 1
	 - Disable all SPI0 interrupts
	 - Disable all Timer 0 interrupt
	 - Disable all Timer 1 interrupt
	 - Disable Timer 2 interrupt
	 - Disable UART0 interrupt
	 ***********************************************************************/
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$

}

extern void USBLIB_0_enter_DefaultMode_from_RESET(void) {
	// $[USBD Init]
	USBD_Init (&initstruct);
	// [USBD Init]$

}

extern void SPI_0_enter_DefaultMode_from_RESET(void) {
	// $[SPI0CKR - SPI0 Clock Rate]
	/***********************************************************************
	 - SPI0 Clock Rate = 0x0F
	 ***********************************************************************/
	SPI0CKR = (0x0F << SPI0CKR_SPI0CKR__SHIFT);
	// [SPI0CKR - SPI0 Clock Rate]$

	// $[SPI0FCN0 - SPI0 FIFO Control 0]
	// [SPI0FCN0 - SPI0 FIFO Control 0]$

	// $[SPI0FCN1 - SPI0 FIFO Control 1]
	/***********************************************************************
	 - Received bytes will be discarded
	 - SPI0 interrupts will not be generated when SPIF is set
	 ***********************************************************************/
	SFRPAGE = 0x20;
	SPI0FCN1 &= ~(SPI0FCN1_RXFIFOE__BMASK | SPI0FCN1_SPIFEN__BMASK);
	// [SPI0FCN1 - SPI0 FIFO Control 1]$

	// $[SPI0CFG - SPI0 Configuration]
	/***********************************************************************
	 - Enable master mode. Operate as a master
	 ***********************************************************************/
	SFRPAGE = 0x00;
	SPI0CFG |= SPI0CFG_MSTEN__MASTER_ENABLED;
	// [SPI0CFG - SPI0 Configuration]$

	// $[SPI0CN0 - SPI0 Control]
	/***********************************************************************
	 - Enable the SPI module
	 - 3-Wire Slave or 3-Wire Master Mode
	 ***********************************************************************/
	SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;
	SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;
	// [SPI0CN0 - SPI0 Control]$

}
