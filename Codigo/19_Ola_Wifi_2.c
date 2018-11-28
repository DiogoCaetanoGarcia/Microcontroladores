// Hello World com o modulo ESP-01 e o site ThingSpeak.
// Inicialmente, o MSP430 pisca os LEDs 3 vezes,
// indicando que o ESP-01 foi inicialmente configurado.
// Depois ele pisca os LEDs 3 vezes, indicando que
// o ESP-01 conseguiu entrar na rede Wifi, abrir uma
// conexão com o site "api.thingspeak.com" e fechar esta
// conexão.
//
// A partir daí, o MSP430 acende o LED vermelho e
// aguarda o usuário apertar o botão da placa Launchpad.
// Sempre que o usuário apertar o botão, o MSP430 apaga
// o LED vermelho e pisca os LEDs 4 vezes, indicando
// a abertura da conexão com o site "api.thingspeak.com",
// o envio do valor da variável 'c' na função main() e
// o encerramento da conexão. Daí ele volta a acender
// o LED vermelho e aguardar o usuário apertar o botão
// da placa Launchpad.
//
// Crie uma conta e um canal no site thingspeak.com
// para poder visualizar os dados enviados pelo MSP430.


#include <msp430g2553.h>
#include <legacymsp430.h>

#define BTN  BIT3
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)

#define RX   BIT1
#define TX   BIT2

#define BAUD_9600   0
#define BAUD_19200  1
#define BAUD_38400  2
#define BAUD_56000  3
#define BAUD_115200 4
#define BAUD_128000 5
#define BAUD_256000 6
#define NUM_BAUDS   7

// Insira aqui o nome da rede wifi e a senha
#define WIFI_NAME "NOME_DA_REDE"
#define WIFI_PSWD "SENHA_DA_REDE"
// Insira aqui a chave da API do Thingspeak
#define WRITEAPIKEY "CHAVE_THINGSPEAK"

#define APISERVER "api.thingspeak.com"
#define APIPORT   "80"
#define CONN_ID "4"

void Config_Dig_IO(void);
void Wait_Btn(void);
void Pisca(int num_pisca, char led_bits, unsigned int T_2);

void Int_to_String(int n, char *str);
int cmp_str(char *str1, char *str2);
int find_substr(char *str1, char *str2);
int len_str(char *str);
void cat_str(char *str_dest, char *str_src);
void set_str(char *str_dest, char *str_src);
void cat_int(char *str_dest, int n);

void Init_UART(unsigned int baud_rate_choice);
void Send_Data(unsigned char c);
void Send_String(char str[]);

void Atraso(unsigned int ms);

int AT_command_response(char command[], char response[]);
void connect(char *wifi_name, char *wifi_pswd);
void send_data(char *url, char *port, char *msg);
void AT_CIPSEND(char *cip_data);

int main(void)
{
	int c = 0;
	char s[10];
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	Config_Dig_IO();
	Init_UART(BAUD_9600);
	Atraso(500);
	connect(WIFI_NAME, WIFI_PSWD);
	AT_command_response("AT+CIPSTART=" CONN_ID ",\"TCP\",\""
		APISERVER "\"," APIPORT "\r\n", "OK");
	AT_command_response("AT+CIPCLOSE=" CONN_ID "\r\n","OK");
	while(1)
	{
		P1OUT |= LED1;
		Wait_Btn();
		P1OUT &= ~LED1;
		Int_to_String(c, s);
		c = (c+10)%100;
		send_data(APISERVER, APIPORT, s);
	}
	return 0;
}

void Config_Dig_IO(void)
{
	P1OUT = BTN;
	P1REN |= BTN;
	P1DIR &= ~BTN;
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;
}

void Wait_Btn(void)
{
	volatile unsigned int i = 300;
	// Espera o botao ser pressionado
	while(P1IN & BTN);
	// Espera o botao ser solto
	while((P1IN & BTN)==0);
	// Debounce
	while(i)
	{
		i--;
		if((P1IN & BTN)==0)
			i = 300;
	}
}

void Pisca(int num_pisca, char led_bits, unsigned int T_2)
{
	while(num_pisca--)
	{
		P1OUT |= led_bits;
		Atraso(T_2);
		P1OUT &= ~led_bits;
		Atraso(T_2);
	}
}

int len_str(char *str)
{
	int i = 0;
	while(str[i]!='\0')
		i++;
	return i;
}

void set_str(char *str_dest, char *str_src)
{
	str_dest[0] = '\0';
	cat_str(str_dest, str_src);
}

void cat_int(char *str_dest, int n)
{
	Int_to_String(n, str_dest + len_str(str_dest));
}

void cat_str(char *str_dest, char *str_src)
{
	int i = 0, j = len_str(str_dest);
	while(1)
	{
		if((str_dest[j] = str_src[i])=='\0')
			return;
		i++;
		j++;
	}
}

void Int_to_String(int n, char *str)
{
	int casa, dig, i=0;
	if(n==0)
	{
		str[i++] = '0';
		str[i++] = '\0';
		return;
	}
	if(n<0)
	{
		str[i++] = '-';
		n = -n;
	}
	for(casa = 1; casa<=n; casa *= 10);
	casa /= 10;
	while(casa>0)
	{
		dig = (n/casa);
		str[i++] = dig+'0';
		n -= dig*casa;
		casa /= 10;
	}
	str[i] = '\0';
}

int cmp_str(char *str1, char *str2)
{
	for(; ((*str1)!='\0')&&((*str2)!='\0'); str1++, str2++)
	{
		if((*str1)!=(*str2))
			return 0;
	}
	return ((*str1)==(*str2));
}

int find_substr(char *str1, char *str2)
{
	int i, j;
	for(; (*str1)!='\0'; str1++)
	{
		i=j=0;
		while((str1[i]==str2[j])
			||(str1[i]=='\0')
			||(str2[j]=='\0'))
		{
			if(str2[j]=='\0')
				return 1;
			if(str1[i]=='\0')
				break;
			i++;
			j++;
		}
	}
	return 0;
}

void Send_Data(unsigned char c)
{
	while((IFG2&UCA0TXIFG)==0);
	UCA0TXBUF = c;
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
		UCA0CTL1 |= UCSWRST;
		// Configura a transmissao serial UART com 8 bits de dados,
		// sem paridade, comecando pelo bit menos significativo,
		// e com um bit de STOP
		UCA0CTL0 = 0;
		// Escolhe o SMCLK como clock para a UART
		UCA0CTL1 |= UCSSEL_2;
		// Define a baud rate
		UCA0BR0 = BRs[baud_rate_choice];
		UCA0BR1 = 0;
		UCA0MCTL = MCTLs[baud_rate_choice];
		// Habilita os pinos para transmissao serial UART
		P1SEL2 = P1SEL = RX+TX;
		UCA0CTL1 &= ~UCSWRST;
	}
}

// Atraso de ms
void Atraso(unsigned int ms)
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

void connect(char *wifi_name, char *wifi_pswd)
{
	char req[100] = "AT+CWJAP_CUR=\"";
	cat_str(req, wifi_name);
	cat_str(req, "\",\"");
	cat_str(req, wifi_pswd);
	cat_str(req, "\"\r\n");
	AT_command_response("AT+RST\r\n","OK");
	AT_command_response("AT+CWMODE_CUR=3\r\n", "OK");
	AT_command_response("AT+CIPMUX=1\r\n","OK");
	AT_command_response(req,"OK");
}

int AT_command_response(char command[], char response[])
{
	char str[150];
	int i=0, tries=0;
	Send_String(command);
	while(1)
	{
		while((IFG2&UCA0RXIFG)==0);
		str[i] = UCA0RXBUF;
		if(str[i]=='\n')
		{
			tries++;
			str[i+1] = '\0';
			if(find_substr(str, response))
			{
				Pisca(1, LEDS, 250);
				return tries;
			}
			else if(find_substr(str, "ERROR") || 
				find_substr(str, "link is not valid"))
			{
				// Pisca(1, LED1, 100);
				Send_String(command);
			}
			else if(find_substr(str, "busy"))
			{
				// Pisca(1, LED2, 100);
				Atraso(50);
			}
			str[i=0] = '\0';
		}
		else
			i++;
	}
	Pisca(1, LEDS, 250);
	return tries;
}

void send_data(char *url, char *port, char *msg)
{
	char cur_req[100] = "AT+CIPSTART=" CONN_ID ",\"TCP\",\"";
	cat_str(cur_req, url);
	cat_str(cur_req, "\",");
	cat_str(cur_req, port);
	cat_str(cur_req, "\r\n");
	AT_command_response(cur_req, "OK");
	
	set_str(cur_req, "GET /update?api_key=" WRITEAPIKEY "&field1=");
	cat_str(cur_req, msg);
	cat_str(cur_req, "\r\n\r\n");
	AT_CIPSEND(cur_req);
	AT_command_response("AT+CIPCLOSE=" CONN_ID "\r\n","UNLINK");
}

void AT_CIPSEND(char *cip_data)
{
	char header[150] = "AT+CIPSENDBUF=" CONN_ID ",";
	cat_int(header, len_str(cip_data));
	cat_str(header, "\r\n");
	AT_command_response(header,   "OK");
	AT_command_response(cip_data, "OK");
}