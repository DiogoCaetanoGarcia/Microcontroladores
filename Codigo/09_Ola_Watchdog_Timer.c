#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (BIT0|BIT6)
#define BTN  BIT3

int main(void)
{
	WDTCTL = WDTPW | WDTCNTCL;
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;
	P1OUT |= BTN;
	P1DIR &= BTN;
	P1REN |= BTN;
	if(IFG1 & WDTIFG)
		P1OUT |= LED2;
	while(1)
	{
		if(P1IN & BTN)
		{
			P1OUT &= ~LED1;
			WDTCTL = WDTPW | WDTCNTCL;
		}
		else P1OUT |= LED1;
	}
	return 0;
}























