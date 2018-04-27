#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (BIT0|BIT6)

int main(void)
{
	volatile int i = 0;
	WDTCTL = WDTPW + WDTCNTCL + WDTTMSEL +
		WDTIS0 + WDTIS1;
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz

	P1OUT &= ~LEDS;
	P1DIR |= LEDS;
	while(1)
	{
		while((IFG1 & WDTIFG)==0);
		IFG1 &= ~WDTIFG;
		i++;
		if(i==15625)
		{
			P1OUT ^= LEDS;
			i=0;
		}
	}
	return 0;
}























