// Codigo para acender 0 a 6 LEDs charliplexados
// de acordo com a leitura do conversor A/D.
// Utiliza agendamento de tarefas, com intervalo
// de 833us entre cada uma. Assim, os LEDs
// piscam a 100 Hz.
// Alem disso, a posicao do potenciometro
// define a posicao de um servo SG90
// ligado ao pino P1.6. O acionamento do
// servo pelo potenciometro é ligado ou
// desligado através do botão da placa
// ligado ao pino P1.3.

#include <msp430g2553.h>
#include <legacymsp430.h>

#define IN_AD BIT4
#define IN_AD_CH INCH_4
#define N_CPX 6
#define CPX0 BIT0
#define CPX1 BIT1
#define CPX2 BIT2
#define CPXs (CPX0|CPX1|CPX2)
#define BTN BIT3
#define SERVO BIT6
#define next(elem, tam) ((elem+1)%tam)

unsigned int escala;
int liga_servo = 0;
unsigned int pos_servo; 
// Tarefas
void Mede_Potenciometro(void)
{
	// Comeca uma conversao
	ADC10CTL0 |= ENC + ADC10SC;
	// Espera a conversao ficar pronta
	while((ADC10CTL0 & ADC10IFG)==0);
	escala = (ADC10MEM*6+1023/2)/1023;
	if(liga_servo)
	{
		pos_servo = 37*ADC10MEM + 1023/2;
		pos_servo /= 1023;
		pos_servo *= 50;
		pos_servo += 650; //700;
	}
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

void Atualiza_Servo(void)
{
	TACCR1 = pos_servo;
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

void Config_AD(void)
{
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10AE0 = IN_AD;
	ADC10CTL1 = IN_AD_CH + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0 + SHS_0; 
}

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
	volatile unsigned int i = 1000;
	while(i)
	{
		i--;
		if((P1IN&BTN)==0)
			i = 1000;
	}
}

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


int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	Config_AD();
	Servo_Off();
	Config_BTN();
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

interrupt(TIMER1_A1_VECTOR) TA1_ISR(void)
{
	(*pool[ini])();
	ini = next(ini, fim);
	TA1CTL &= ~TAIFG;
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


