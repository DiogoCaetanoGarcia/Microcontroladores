#include <msp430g2553.h>

#define CPX0 BIT0
#define CPX1 BIT1
#define CPX2 BIT2
#define Tloop 10
#define Nloop 25

void atraso(volatile unsigned int T_ms)
{
	TACCR0 = 999;
	TACTL = TACLR;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	while(T_ms>0)
	{
		while((TACTL&TAIFG)==0);
		T_ms--;
		TACTL &= ~TAIFG;
	}
	TACTL = MC_0;
}

void Charlieplex_2leds(char pin_in, char pin_out1, char pin_out2, unsigned int N_loops, unsigned int T_loops)
{
	P1DIR = (P1DIR&(~pin_in))|(pin_out1+pin_out2);
	while(N_loops>0)
	{
		N_loops--;
		P1OUT = (P1OUT|pin_out1)&(~pin_out2);
		atraso(T_loops);
		P1OUT = (P1OUT|pin_out2)&(~pin_out1);
		atraso(T_loops);
	}
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
	while(1)
	{
		Charlieplex_2leds(CPX2, CPX0, CPX1, Nloop, Tloop);
		Charlieplex_2leds(CPX0, CPX1, CPX2, Nloop, Tloop);
		Charlieplex_2leds(CPX1, CPX0, CPX2, Nloop, Tloop);
	}
	return 0;
}




