#include <msp430g2553.h>
#include <legacymsp430.h>

#define LED BIT6
#define PERIODO 20000
#define MIN_T  650 //1000
#define MAX_T 2500 //2000
#define STEP_T  50

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LED;
	P1SEL |= LED;
	P1SEL2 &= ~LED;
	TACCR0 = PERIODO-1;
	TACCR1 = MIN_T;
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
		TACCR1 += STEP_T;
		if(TACCR1>=MAX_T)
		{
			TACCR1 = MAX_T;
			direcao = 1;
		}
	}
	// Diminuir o ciclo de trabalho ate o minimo 
	else if(direcao==1)
	{
		TACCR1 -= STEP_T;
		if(TACCR1<=MIN_T)
		{
			TACCR1 = MIN_T;
			direcao = 0;
		}
	}
	TACTL &= ~TAIFG;
}






