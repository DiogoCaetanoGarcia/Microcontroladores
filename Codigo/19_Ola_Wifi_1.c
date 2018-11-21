#include <msp430g2553.h>
#include <legacymsp430.h>

// O codigo para quando _CONEXAO_OK_
// esta definida funciona corretamente.
// Por algum motivo, o codigo para
// quando _CONEXAO_OK_ nao esta definida
// nao funciona

// #define _CONEXAO_OK_

#define BTN BIT3
#define RX BIT1
#define TX BIT2
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)

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
void Atraso(volatile unsigned int ms);
int cmp_str(char str1[], char str2[]);
int Read_String(char str[], int max_char);
void Wait_Btn(void);

int main(void)
{
	char wifi_str[100];
	int strlen;
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1OUT = BTN;
	P1REN |= BTN;
	P1DIR &= ~BTN;
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;

	Init_UART(BAUD_115200);
	Send_String("AT+UART_DEF=9600,8,1,0,0\r\n");
	Init_UART(BAUD_9600);
#ifdef _CONEXAO_OK_
	_BIS_SR(GIE);
#endif
	while(1)
	{
		wifi_str[0] = '\0';
		Wait_Btn();
		P1OUT |= LED1;
		Send_String("AT\r\n");
#ifdef _CONEXAO_OK_
		_BIS_SR(LPM0_bits+GIE);
#else
		strlen = Read_String(wifi_str, 100);
		if(cmp_str(wifi_str, "OK\r\n"))
			P1OUT |= LED2;
#endif
		Atraso(250);
		P1OUT &= ~LEDS;
		Atraso(250);
	}
	return 0;
}

void Wait_Btn(void)
{
	// Espera o botao ser pressionado
	while(P1IN & BTN);
	// Espera o botao ser solto
	while((P1IN & BTN)==0);
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
#ifdef _CONEXAO_OK_
		// // Habilita a interrupcao por chegada de dados via UART
		IE2 |= UCA0RXIE;
#endif
	}
}

#ifdef _CONEXAO_OK_
interrupt(USCIAB0RX_VECTOR) Receive_Data(void)
{
	static int i = 0;
	static char str[50];
	str[i] = UCA0RXBUF;
	if(str[i]=='\n')
	{
		str[i+1] = '\0';
		i = 0;
		if(cmp_str(str, "OK\r\n"))
			P1OUT |= LED2;
		LPM0_EXIT;
	}
	else
		i++;
}
#endif

int Read_String(char str[], int max_char)
{
	int i;
	for(i=0; i<max_char; i++)
	{
		while((IFG2&UCA0RXIFG)==0);
		str[i] = UCA0RXBUF;
		if(str[i]=='\n')
		{
			str[++i] = '\0';
			// Retorna o tamanho da string
			return i;
		}
	}
	return i;
}

// Atraso de ms
void Atraso(volatile unsigned int ms)
{

	TACCR0 = 1000-1;
	TACTL |= TACLR;
	TACTL = TASSEL_2 + ID_0 + MC_1;
	while(ms--)
	{
		while((TACTL&TAIFG)==0);
		TACTL &= ~TAIFG;
	}
	TACTL = MC_0;
}

int cmp_str(char str1[], char str2[])
{
	int i;
	for(i=0; (str1[i]!='\0')&&(str2[i]!='\0'); i++)
	{
		if(str1[i]!=str2[i])
			return 0;
	}
	return (str1[i]==str2[i]);
}