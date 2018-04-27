#include <msp430g2553.h>
#include <legacymsp430.h> // Para rodar interrupcoes

#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)
#define BTN BIT3
#define LED_DLY 10000

void delay(volatile unsigned long int t)
{
	while(t--);
}

void blink(int num_blink)
{
	while(num_blink>0)
	{
		num_blink--;
		P1OUT ^= LED2;
		delay(LED_DLY);
		P1OUT ^= LED2;
		delay(LED_DLY);
	}
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LEDS;
	P1OUT &= ~LEDS;
	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES |= BTN;
	P1IE |= BTN;

	TACCR0 = 5;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	_BIS_SR(LPM0_bits+GIE);
	return 0;
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	blink(TAR+1);
	while((P1IN&BTN)==0);
	P1IFG &= ~BTN;
}






