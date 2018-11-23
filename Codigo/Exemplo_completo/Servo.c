#include "Servo.h"

void Servo_On(void)
{
	P1DIR |= SERVO;
	P1SEL |= SERVO;
	P1SEL2 &= ~SERVO;
	TACCR0 = 20000-1;
	TACCR1 = (2500-650)/2;
	TACCTL1 = OUTMOD_7;
	TACTL = TASSEL_2 | ID_0 | MC_1;
	liga_servo = 1;
}

void Servo_Off(void)
{
	TACTL = MC_0;
	liga_servo = 0;
	P1DIR &= ~SERVO;
	P1SEL &= ~SERVO;
	P1SEL2 &= ~SERVO;
}

void Atualiza_Servo(void)
{
	TACCR1 = pos_servo;
}