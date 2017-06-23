// Utiliza a Launchpad para enviar uma string
// para o computador via UART sempre que o botao
// em P1.3 for pressionado. Pisca os LEDs N vezes
// (0<N<10), de acordo com o valor recebido pela UART.
//
// Utilize o monitor serial do Energia ou do
// Code Composer para ver a string enviada, e para
// mandar a quantidade de piscadas.
//
// EH NECESSARIO MUDAR DUAS CONEXOES NA LAUNCHPAD
// PARA O CODIGO FUNCIONAR.
//
// Procure 5 jumpers na placa, como indicado abaixo:
// RXD | | | | |
// TXD | | | | |
//     T R R T V
//     X X S E C
//     D D T S C
//           T
//
// Mude a conexoes de 2 desses jumpers, como indicado abaixo:
// RXD --- | | |
// TXD --- | | |
//     T R R T V
//     X X S E C
//     D D T S C
//           T

#include <msp430g2553.h>
#include <legacymsp430.h>

#define BTN BIT3
#define RX BIT1
#define TX BIT2
#define LEDS (BIT0|BIT6)

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

int main(void)
{
	volatile int i = 0;
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1OUT = BTN;
	P1REN |= BTN;
	P1DIR &= ~BTN;
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;

	Init_UART(BAUD_9600);
	_BIS_SR(GIE);
	while(1)
	{
		// Espera o botao ser pressionado
		while(P1IN & BTN);
		// Espera o botao ser solto
		while((P1IN & BTN)==0);
		// Transmite a string
		Send_Int(i++);
		Send_String(": ola mundo!\n");
	}
	return 0;
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
		// Habilita a interrupcao por chegada de dados via UART
		IE2 |= UCA0RXIE;
	}
}

interrupt(USCIAB0RX_VECTOR) Receive_Data(void)
{
	unsigned char blink = UCA0RXBUF - '0';
	volatile unsigned int t;
	if(blink<10)
	{
		while(blink--)
		{
			P1OUT |= LEDS;
			for(t=30000; t>0; t--);
			P1OUT &= ~LEDS;
			for(t=30000; t>0; t--);
		}
	}
}
