#include <msp430g2553.h>
#include <legacymsp430.h>

#define IN_AD BIT1
#define IN_AD_CH INCH_1
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;

	// Configura o canal 1 do Timer A em modo de comparacao
	// com periodo de 0,5 segundos
	TACCR0 = 62500-1;
	TACCR1 = TACCR0/2;
	TACCTL1 = OUTMOD_7;
	TACTL = TASSEL_2 | ID_3 | MC_1;

	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON + ADC10IE;
	ADC10AE0 = IN_AD;
	// Com SHS_1, uma conversao sera requisitada
	// sempre que o canal 1 do Timer_A terminar sua contagem
	ADC10CTL1 = IN_AD_CH + SHS_1 + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_2;
	ADC10CTL0 |= ENC;

	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

interrupt(ADC10_VECTOR) ADC10_ISR(void)
{
	// Acende ou apaga o LED1
	// de acordo com a leitura AD
	if(ADC10MEM < 512)
		P1OUT &= ~LED1;
	else
		P1OUT |= LED1;

	// Inverte o LED2 para vermos
	// a temporizacao da chamada
	// a esta interrupcao
	P1OUT ^= LED2;
}
