// Acende ou apaga o LED em P1.6
// de acordo com a tensão em P1.0:
//    Acende LED se V(P1.0) > Vcc/2
//    Apaga  LED se V(P1.0) < Vcc/2
//
// Este código não utiliza interrupção.

#include <msp430g2553.h>
#include <legacymsp430.h>

#define LED BIT6

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1DIR |= LED;
	P1OUT &= ~LED; 
	
	CAPD = BIT0;
	CACTL2 = P2CA0;
	CACTL1 = CARSEL + CAREF_1 + CAON;
	
	while(1)
	{
		while((CACTL2&CAOUT)==0);
		P1OUT ^= LED;
		while((CACTL2&CAOUT)>0);
		P1OUT ^= LED;
	}
	return 0;
}
