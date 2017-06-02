#include <msp430g2553.h>
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN  BIT3

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1OUT |= LEDS;
	P1DIR |= LEDS;
	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	while(1)
	{
		if((P1IN&BTN)==0)
			P1OUT = LEDS+BTN;
		else
			P1OUT &= ~LEDS;
	}
	return 0;
}
