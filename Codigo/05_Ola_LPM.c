#include <msp430g2553.h>
#include <legacymsp430.h> // Para rodar interrupcoes
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN  BIT3

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;
	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES |= BTN;
	P1IE |= BTN;
	_BIS_SR(GIE + LPM4_bits);
	return 0;
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	P1OUT ^= LEDS;
	while((P1IN&BTN)==0);
	P1OUT ^= LEDS;
	P1IFG &= ~BTN;
}
