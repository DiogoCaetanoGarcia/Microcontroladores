#include <msp430g2553.h>
#include <legacymsp430.h>

#define BTN BIT3

unsigned long round_ulong(unsigned long divisor, unsigned long dividendo);
void Send_Data(unsigned char c);
void Send_Int(int n);
void Send_String(char str[]);

volatile unsigned int ta_overflow, capt_ant, n_ciclos;
volatile unsigned long ciclos, freq;

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;

	P1DIR &= ~BTN;
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES &= ~BTN;

	P2DIR &= ~BIT0;
	P2SEL |= BIT0;
	P2SEL2 &= ~BIT0;
	TA1CCTL0 = CM_2 + CCIS_0 + CAP;
	TA1CTL = TASSEL_2 + ID_0 + MC_2;

	P1SEL2 = P1SEL = BIT1+BIT2;
	UCA0CTL0 = 0;
	UCA0CTL1 = UCSSEL_2;
	UCA0BR1 = 0x6;
	UCA0BR0 = 0x82;
	UCA0MCTL = UCBRF_0+UCBRS_6;

	Send_String("Medidor de frequencia baseado no MSP430.\n");
	Send_String("Pressione o botao da placa Launchpad.\n\n");
	while(1)
	{
		ciclos = n_ciclos = ta_overflow = capt_ant = 0;
		TA1CCTL0 &= ~CCIE;
		TA1CTL &= ~TAIE;
		P1IFG &= ~BTN;
		P1IE |= BTN;
		_BIS_SR(LPM4_bits+GIE);
		Send_String("Realizando leitura ...\n");
		TA1CCTL0 |= CCIE;
		TA1CTL |= TAIE;
		_BIS_SR(LPM0_bits+GIE);
		Send_String("Frequencia = ");
		if(ciclos<=16)
			freq = 1600;
		else if(ciclos<=16000)
			freq = 1600000;
		else
			freq = 1600000000;
		freq = round_ulong(freq, ciclos);
		Send_Int((int)(freq/100));
		Send_String(".");
		freq = freq%100;
		if(freq<10)
			Send_String("0");
		Send_Int((int)(freq));
		if(ciclos<=16)
			Send_String("M");
		else if(ciclos<=16000)
			Send_String("k");
		Send_String("Hz\n");
	}
	return 0;
}

interrupt(TIMER1_A1_VECTOR) TA1_Overflow_ISR(void)
{
	ta_overflow++;
	TA1CTL &= ~TAIFG;
}

interrupt(TIMER1_A0_VECTOR) TA1_Capt0_ISR(void)
{
	if((TA1CCTL0&COV)==0)
	{
		while(ta_overflow>0)
		{
			ciclos += 0x10000;
			ta_overflow--;
		}
		ciclos += (unsigned long)TA1CCR0;
		ciclos -= (unsigned long)capt_ant;
		n_ciclos++;
	}
	else
		TA1CCTL0 &= ~COV;
	capt_ant = TA1CCR0;
	ta_overflow = 0;
	if(ciclos>=32000000)
	{
		ciclos += (unsigned long)TA1CCR0;
		ciclos -= (unsigned long)capt_ant;
		ciclos = round_ulong(ciclos, n_ciclos);
		TA1CCTL0 &= ~CCIE;
		TA1CTL &= ~TAIE;
		LPM0_EXIT;
	}
	TA1CCTL0 &= ~CCIFG;
}

interrupt(PORT1_VECTOR) Interrupcao_P1(void)
{
	P1IE &= ~BTN;
	LPM4_EXIT;
}

unsigned long round_ulong(unsigned long divisor, unsigned long dividendo)
{
	return (divisor+(dividendo/2))/dividendo;
}

void Send_Data(unsigned char c)
{
	while((IFG2&UCA0TXIFG)==0);
	UCA0TXBUF = c;
}

void Send_Int(int n)
{
	int casa, dig;
	if(n==0)
	{
		Send_Data('0');
		return;
	}
	if(n<0)
	{
		Send_Data('-');
		n = -n;
	}
	for(casa = 1; casa<=n; casa *= 10);
	casa /= 10;
	while(casa>0)
	{
		dig = (n/casa);
		Send_Data(dig+'0');
		n -= dig*casa;
		casa /= 10;
	}
}

void Send_String(char str[])
{
	int i;
	for(i=0; str[i]!= '\0'; i++)
		Send_Data(str[i]);
}
