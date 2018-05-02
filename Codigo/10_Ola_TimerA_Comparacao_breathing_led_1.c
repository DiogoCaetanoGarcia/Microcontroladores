#include <msp430g2553.h>
#include <legacymsp430.h>

#define LED BIT6
#define PERIODO 1000

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LED;
	P1SEL |= LED;
	P1SEL2 &= ~LED;
	TACCR0 = PERIODO-1;
	TACCR1 = 0;
	TACCTL1 = OUTMOD_7;
	TACTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
	_BIS_SR(LPM0_bits + GIE);
	return 0;
}

interrupt(TIMER0_A1_VECTOR) TA0_ISR(void)
{
	static int direcao = 0;
	// Aumentar o ciclo de trabalho ate o maximo
	if(direcao==0)
	{
		TACCR1++;
		if(TACCR1>=TACCR0)
			direcao = 1;
	}
	// Diminuir o ciclo de trabalho ate o minimo 
	else if(direcao==1)
	{
		TACCR1--;
		if(TACCR1<=50)
			direcao = 0;
	}
	TACTL &= ~TAIFG;
}






