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
	WDTCTL = WDTPW + WDTHOLD;
	P1REN  = BTN;
	P1OUT  = LED2 + BTN;
	P1DIR  = LED1 + LED2;
	__asm__(".BTN1: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN2 \n"
		"mov.b   #1, r15 \n"
		"call   blink \n"
		".BTN2: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN3 \n"
		"mov.b   #2, r15 \n"
		"call   #blink \n"
		".BTN3: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN4 \n"
		"mov.b   #3, r15 \n"
		"call   #blink \n"
		".BTN4: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN5 \n"
		"mov.b   #4, r15 \n"
		"call   #blink \n"
		".BTN5: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN6 \n"
		"mov.b   #5, r15 \n"
		"call   #blink \n"
		".BTN6: \n"
		"mov.b  &__P1IN, r15 \n"
		"and.b  #8, r15 \n"
		"jnz    .BTN1 \n"
		"mov.b   #6, r15 \n"
		"call   #blink \n"
		"jmp    .BTN1");
	return 0;
}
