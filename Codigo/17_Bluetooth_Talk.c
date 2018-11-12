#include <msp430g2553.h>
#include <legacymsp430.h>

#define LEDS (BIT0|BIT6)
#define RX BIT1
#define TX BIT2

#define BAUD_9600   0
#define BAUD_19200  1
#define BAUD_38400  2
#define BAUD_56000  3
#define BAUD_115200 4
#define BAUD_128000 5
#define BAUD_256000 6
#define NUM_BAUDS   7

void Send_Data(unsigned char c);
void Send_Int(int n);
void Send_String(char str[]);
void Init_UART(unsigned int baud_rate_choice);
void Atraso_ms(volatile unsigned int ms);

int main(void)
{
	char c;

	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1DIR |= LEDS;
	P1OUT |= LEDS;

	Init_UART(BAUD_115200);
	TA1CCR0 = 12500-1;
	TA1CTL = TASSEL_2 + ID_3 + MC_1;
	_BIS_SR(GIE);
	while(1)
	{
		TA1CTL |= TAIE;
		while((IFG2 & UCA0RXIFG)==0);
		c = UCA0RXBUF;
		if((c=='0')||(c=='1'))
		{
			TA1CTL &= ~TAIE;
			if(c=='1')
			{
				P1OUT |= LEDS;
				Send_String("LEDs acesos!\r\n");
			}
			else if(c=='0')
			{
				P1OUT &= ~LEDS;
				Send_String("LEDs apagados!\r\n");
			}
			Atraso_ms(2000);
		}
		else
			Send_String("Opcao invalida!\r\n");
	}
	return 0;
}

interrupt(TIMER1_A1_VECTOR) TA1_ISR(void)
{
	P1OUT ^= LEDS;
	TA1CTL &= ~TAIFG;
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

void Init_UART(unsigned int baud_rate_choice)
{
	unsigned char BRs[NUM_BAUDS] = {104, 52, 26, 17, 8, 7, 3};
	unsigned char MCTLs[NUM_BAUDS] = {UCBRF_0+UCBRS_1,
										UCBRF_0+UCBRS_0,
										UCBRF_0+UCBRS_0,
										UCBRF_0+UCBRS_7,
										UCBRF_0+UCBRS_6,
										UCBRF_0+UCBRS_7,
										UCBRF_0+UCBRS_7};
	if(baud_rate_choice<NUM_BAUDS)
	{
		// Habilita os pinos para transmissao serial UART
		P1SEL2 = P1SEL = RX+TX;
		// Configura a transmissao serial UART com 8 bits de dados,
		// sem paridade, comecando pelo bit menos significativo,
		// e com um bit de STOP
		UCA0CTL0 = 0;
		// Escolhe o SMCLK como clock para a UART
		UCA0CTL1 = UCSSEL_2;
		// Define a baud rate
		UCA0BR0 = BRs[baud_rate_choice];
		UCA0BR1 = 0;
		UCA0MCTL = MCTLs[baud_rate_choice];
	}
}

void Atraso_ms(volatile unsigned int ms)
{

	TACCR0 = 1000-1;
	TACTL = TACLR;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	while(ms--)
	{
		while((TACTL&TAIFG)==0);
		TACTL &= ~TAIFG;
	}
	TACTL = MC_0;
}