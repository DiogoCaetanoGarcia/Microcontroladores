// Codigo para acender 0 a 6 LEDs charliplexados
// de acordo com a leitura do conversor A/D.
// Utiliza agendamento de tarefas, com intervalo
// de 832us entre cada uma. Assim, os LEDs
// piscam a 100 Hz.

#include <msp430g2553.h>
#include <legacymsp430.h>

#define IN_AD BIT4
#define IN_AD_CH INCH_4
#define N_CPX 6
#define CPX0 BIT0
#define CPX1 BIT1
#define CPX2 BIT2
#define CPXs (CPX0|CPX1|CPX2)
#define next(elem, tam) ((elem+1)%tam)

unsigned int escala;
// Tarefas
void Mede_Potenciometro(void)
{
	// Comeca uma conversao
	ADC10CTL0 |= ENC + ADC10SC;
	// Espera a conversao ficar pronta
	while((ADC10CTL0 & ADC10IFG)==0);
	escala = (ADC10MEM*6+1023/2)/1023;
}

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

void Null_func(void)
{
	return;
}

//declaracao do tipo ponteiro para funcao
typedef void(*ptrFunc)(void);

//variáveis do kernel
#define BUFFSIZE 16
ptrFunc pool[BUFFSIZE];
int ini;
int fim;

//funções do kernel
void InicializaKernel(void)
{
	ini = 0;
	fim = 0;
	TACCR0 = 832;
	TACTL = TASSEL_2 + ID_0 + MC_1;
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
	TACTL |= TAIE;
	while(1)
	{
		_BIS_SR(LPM0_bits+GIE);
		(*pool[ini])();
		ini = next(ini, fim);
	}
}

void Config_AD(void)
{
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10AE0 = IN_AD;
	ADC10CTL1 = IN_AD_CH + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0 + SHS_0; 
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	Config_AD();
	InicializaKernel();
	AddKernel(Mede_Potenciometro);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	ExecutaKernel();
	return 0;
}

interrupt(TIMER0_A1_VECTOR) TA0_ISR(void)
{
	LPM0_EXIT;
	TA0CTL &= ~TAIFG;
}
