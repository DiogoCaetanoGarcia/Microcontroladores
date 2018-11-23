#include "Kernel.h"

void Null_func(void)
{
	return;
}

void InicializaKernel(void)
{
	ini = 0;
	fim = 0;
	TA1CCR0 = 832;
	TA1CTL = TASSEL_2 + ID_0 + MC_1;
}

void AddKernel(ptrFunc newFunc)
{
	if(next(fim, BUFFSIZE) != ini)
	{
		pool[fim] = newFunc;
		fim = next(fim, BUFFSIZE);
	}
}

void ExecutaKernel(void)
{
	TA1CTL |= TAIE;
	_BIS_SR(LPM0_bits+GIE);
}

interrupt(TIMER1_A1_VECTOR) TA1_ISR(void)
{
	(*pool[ini])();
	ini = next(ini, fim);
	TA1CTL &= ~TAIFG;
}