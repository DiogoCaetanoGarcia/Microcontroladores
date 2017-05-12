#include <msp430g2553.h>
#include <legacymsp430.h>

#define LED BIT0

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
	P1OUT &= ~LED;
	P1DIR |= LED;
	TA0CCR0 = 62500-1; //10000-1;
	TA0CTL = TASSEL_2 + ID_3 + MC_1 + TAIE;
	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

interrupt(TIMER0_A1_VECTOR) TA0_ISR(void)
{
	P1OUT ^= LED;
	TA0CTL &= ~TAIFG;
}





















