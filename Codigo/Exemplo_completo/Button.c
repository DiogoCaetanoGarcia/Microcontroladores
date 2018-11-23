#include "Button.h"

void Config_BTN(void)
{
	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES &= ~BTN;
	P1IE |= BTN;
}

void Debounce_BTN()
{
	volatile unsigned int i = 300;
	while(i)
	{
		i--;
		if((P1IN&BTN)==0)
			i = 300;
	}
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	if(liga_servo==0)
	{
		Servo_On();
		liga_servo = 1;
		pool[2] = Atualiza_Servo;
	}
	else
	{
		Servo_Off();
		liga_servo = 0;
		pool[2] = Null_func;
	}
	Debounce_BTN();
	P1IFG &= ~BTN;
}