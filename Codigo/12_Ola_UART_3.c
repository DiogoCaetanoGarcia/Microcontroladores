/* 

Código para o MSP430 medir a tensão analogica
no pino P1.0 sempre que o usuário pressionar o botão P1.3,
e enviar o resultado via porta serial assincrona UART.

Conexoes:
   P1.0: sinal analogico entre 0 e Vcc
   P1.1: recepcao UART do MSP430
   P1.2: transmissao UART do MSP430
   P1.3: botao da placa Launchpad
   P1.6: LED verde da placa Launchpad

*/

#include <msp430g2553.h>
#include <legacymsp430.h>

#define AD_IN BIT0
#define AD_INCH INCH_0
#define RX BIT1
#define TX BIT2
#define BTN BIT3
#define LED BIT6
#define BAUD_9600   0
#define BAUD_19200  1
#define BAUD_38400  2
#define BAUD_56000  3
#define BAUD_115200 4
#define BAUD_128000 5
#define BAUD_256000 6
#define NUM_BAUDS   7

void Send_Data(volatile unsigned char c);
void Send_Int(int n);
void Send_String(char str[]);
void Init_P1(void);
void Init_AD(void);
void Init_UART(unsigned int baud_rate_choice);

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	Init_P1();
	Init_AD();
	Init_UART(BAUD_9600);
	
	_BIS_SR(LPM4_bits+GIE);
	return 0;
}

interrupt(PORT1_VECTOR) P1_ISR(void)
{
	P1OUT |= LED;
	while((P1IN&BTN)==0);
	// Manda comecar uma conversao
	ADC10CTL0 |= ENC + ADC10SC;
	// Espera a conversao ficar pronta
	while((ADC10CTL0 & ADC10IFG)==0);
	Send_String("Leitura A/D = ");
	Send_Int(ADC10MEM);
	Send_String("\n");
	P1IFG &= ~BTN;
	P1OUT &= ~LED;
}

void Send_Data(volatile unsigned char c)
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

void Init_P1(void)
{
	P1OUT |= BTN;
	P1REN |= BTN;
	P1DIR &= ~BTN;
	P1IE = P1IES = BTN;
	P1OUT &= ~LED;
	P1DIR |= LED;
}

void Init_AD(void)
{
	ADC10AE0  = AD_IN;
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10CTL1 = AD_INCH + SHS_0 + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0;
}
