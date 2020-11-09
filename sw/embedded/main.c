#include <SI_EFM8UB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "efm8_usb.h"

#define SYSCLK       48000000      // SYSCLK frequency in Hz

SI_SBIT(LED,       SFR_P0, 2);
SI_SBIT(LE,        SFR_P0, 6);
SI_SBIT(OE_n,      SFR_P0, 7);

#define LED_ATTACH_ENABLE  0x0004
#define LED_SHOW_ATTACH    0x0002
#define LED_PRE_ATTACH     0x0001
#define LED_REPL_ATTACH    0x0020
#define LED_POST_ATTACH    0x0010
#define LED_DELETE         0x0200
#define LED_INSERT_DELAY   0x0400
#define LED_REC            0x0800
#define LED_SHOW_MACRO     0x2000
#define LED_MACRO_DELEG    0x1000

void Timer2_Init (int counts);
void WriteLEDs (uint16_t leds);

volatile uint8_t flag100 = 0;
volatile uint8_t i;

void SiLabs_Startup (void)
{
}


int main (void)
{
	uint8_t led_timer = 0;

	enter_DefaultMode_from_RESET();

	Timer2_Init (40000); // SYSCLK / 12 / 100 for 100 Hz

	WriteLEDs (0x0000);
	OE_n = 0;

	while (1) {

		// check if 100 Hz timer expired
		if (flag100) {
			// clear flag
			flag100 = false;

			// blink led
			if (led_timer < 25) {
				LED = 0; // LED ON
			} else if (led_timer < 50) {
				LED = 1; // LED OFF
			} else if (led_timer < 75) {
				LED = 0; // LED ON
			} else if (led_timer < 100) {
				LED = 1; // LED OFF
			}

			// update 1 second led timer
			if (++led_timer == 100) {
				led_timer = 0;
			}
		}
	}
}


void Timer2_Init (int counts)
{
   TMR2CN0 = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON0 &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN0_TR2 = 1;                    // Start Timer2
}


SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
    TMR2CN0_TF2H = 0;                   // Clear Timer2 interrupt flag
	flag100 = true;
}


void WriteLEDs (uint16_t leds)
{
	LE = 0;

	SPI0CN0_SPIF = 0;

	SPI0DAT = leds >> 8;
	while(!SPI0CN0_SPIF);
	SPI0CN0_SPIF = 0;

	SPI0DAT = leds;
	while(!SPI0CN0_SPIF);
	SPI0CN0_SPIF = 0;

	LE = 1;
	LE = 0;
}
