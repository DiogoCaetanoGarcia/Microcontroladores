#include <msp430g2553.h>

#define IN_AD BIT4
#define IN_AD_CH INCH_4
#define N_CPX 6
#define CPX0 BIT0
#define CPX1 BIT1
#define CPX2 BIT2

unsigned int escala;
// Tarefas
void Mede_Potenciometro(void)
{
	// Comeca uma conversao
	ADC10CTL0 |= ENC + ADC10SC;
	// Espera a conversao ficar pronta
	while((ADC10CTL0 & ADC10IFG)==0);
	escala = (ADC10MEM*7+1023/2)/1023;
}
void Charlieplex(void)
{
	static int led = 0;

	P1DIR &= ~(CPX0+CPX1+CPX2);
}

//declaracao do tipo ponteiro para funcao
typedef void(*ptrFunc)(void);

//variáveis do kernel
#define BUFFSIZE 16
ptrFunc pool[BUFFSIZE];
int ini;
int fim;
#define next(elem, tam) ((elem+1)%tam)

//funções do kernel
void InicializaKernel(void)
{
	ini = 0;
	fim = 0;
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
	(*pool[ini])();
	ini = next(ini, fim);
}

void Config_AD(void)
{
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10AE0 = IN_AD;
	// Com SHS_0, uma conversao sera requisitada
	// sempre que o bit ADC10SC for setado em ADC10CTL0
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
	AddKernel(Charlieplex);
	AddKernel(Charlieplex);
	AddKernel(Charlieplex);
	AddKernel(Charlieplex);
	AddKernel(Charlieplex);
	AddKernel(Charlieplex);
	ExecutaKernel();
	return 0;
}