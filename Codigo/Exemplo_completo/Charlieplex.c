#include "Charlieplex.h"

void Charlieplex_3(void)
{
	static int led_num = 0;
	char pin_pos[N_CPX] = {CPX0, CPX1, CPX1, CPX2, CPX0, CPX2};
	char pin_neg[N_CPX] = {CPX1, CPX0, CPX2, CPX1, CPX2, CPX0};
	P1DIR &= ~CPXs;
	P1OUT &= ~CPXs;
	if(escala>led_num)
	{
		P1OUT |= pin_pos[led_num];
		P1DIR |= pin_pos[led_num] + pin_neg[led_num];
	}
	led_num = next(led_num, N_CPX);
}