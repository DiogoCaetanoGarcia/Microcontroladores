// Codigo de controle do servo SG90
// com um potenciometro.
//
// O servo SG90 recebe uma onda quadrada
// de 50 Hz (20 ms), e controla sua posição
// de acordo com o tempo em que a onda
// fica em nivel alto:
//    -90 graus: 1 ms
//     90 graus: 2 ms
//
// (O datasheet indica estes valores,
// mas experimentalmente verificou-se
// faxas de 650us a 2,5 ms)
//
// Ligue um potenciometro a Vcc, GND e P1.1
// Ligue pino PWM do SG90 a P1.6

#include <msp430g2553.h>
#include <legacymsp430.h>

#define IN_AD BIT1
#define IN_AD_CH INCH_1
#define LED BIT6

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;

	P1DIR |= LED;
	P1SEL |= LED;
	P1SEL2 &= ~LED;
	// Configura o canal 1 do Timer A em modo de comparacao
	// com periodo de 20ms
	TACCR0 = 20000-1;
	TACCR1 = (2500-650)/2;
	TACCTL1 = OUTMOD_7;
	TACTL = TASSEL_2 | ID_0 | MC_1;

	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON + ADC10IE;
	ADC10AE0 = IN_AD;
	// Com SHS_1, uma conversao sera requisitada
	// sempre que o canal 1 do Timer_A terminar sua contagem
	ADC10CTL1 = IN_AD_CH + SHS_1 + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_2;
	ADC10CTL0 |= ENC;

	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

// Descomente a definicao abaixo para ver
// a diferenca no tamanho do codigo compilado

// #define _CALCULO_COM_FLOAT_
interrupt(ADC10_VECTOR) ADC10_ISR(void)
{
#ifdef _CALCULO_COM_FLOAT_
	float n = (float)ADC10MEM;
	n *= 1850.0/1023.0;
	n += 650.0;
	TACCR1 = (unsigned int)n;
#else
	TACCR1 = 37*ADC10MEM;
	TACCR1 /= 1023;
	TACCR1 *= 50;
	TACCR1 += 700;
#endif
}
