#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define BTN  BIT3
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
		P1OUT ^= LED1;
		delay(LED_DLY);
		P1OUT ^= LED1;
		delay(LED_DLY);
	}
}

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer
	P1REN  = BTN;           // Habilitar resistor de pull-up/down no pino do botão
	P1OUT  = LED2 + BTN;            // LED2 on, Resistor do botão conectado a VCC
	P1DIR  = LED1 + LED2;           // Set pins with LEDs to output
	
	while(1)
	{
		if((P1IN & BTN) == 0) blink(1);
		if((P1IN & BTN) == 0) blink(2);
		if((P1IN & BTN) == 0) blink(3);
		if((P1IN & BTN) == 0) blink(4);
		if((P1IN & BTN) == 0) blink(5);
		if((P1IN & BTN) == 0) blink(6);
	}
	return 0;
}
