#include <msp430g2553.h>
#include <legacymsp430.h>

#define BTN BIT3
#define LED BIT6

void Atraso_ms(volatile unsigned int n)
{
	TA1CCR0 = 125-1;
	TA1CTL = TASSEL_2 + ID_3 + MC_1;
	while(n>0)
	{
		while((TA1CTL&TAIFG)==0);
		TA1CTL &= ~TAIFG;
		n--;
	}
	TA1CTL = TACLR;
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES &= ~BTN;
	P1IE  |= BTN;
	P1IFG &= ~BTN;

	P1DIR |= LED;
	P1SEL |= LED;
	P1SEL2 &= ~LED;

	TACCR0 = 62500-1;
	TACCR1 = 62500/2-1;
	TACCTL1 = OUTMOD_7;
	TACTL = TASSEL_2 + ID_3 + MC_1;
	_BIS_SR(LPM0_bits + GIE);
	return 0;
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	static int t = 0;
	volatile unsigned int periodos[8] = {62500, 50000, 50000, 50000, 20000, 10000, 1000, 80};
	volatile unsigned int divisores[8] = {ID_3, ID_2, ID_1, ID_0, ID_0, ID_0, ID_0, ID_0};
	TACTL = MC_0;
	t = (t+1)%8;
	TACCR0 = periodos[t]-1;
	TACCR1 = (TACCR0+1)/2-1;
	TACTL = TASSEL_2 + MC_1 + divisores[t];
	Atraso_ms(200);
	P1IFG &= ~BTN;
}