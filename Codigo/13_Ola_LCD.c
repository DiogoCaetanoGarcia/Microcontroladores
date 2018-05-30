#include <msp430g2553.h>

#define BTN BIT3

//#define SET_BITS(REGS, BIT_NUMS) REGS |= BIT_NUMS
//#define RESET_BITS(REGS, BIT_NUMS) REGS &= ~(BIT_NUMS)
//#define SET_RESET_BITS(REGS, BIT_NUMS, VAL) if(VAL) SET_BITS(REGS, BIT_NUMS); else RESET_BITS(REGS, BIT_NUMS)
//#define WHILE_SET(REGS, BIT_NUMS) while(REGS & (BIT_NUMS))
//#define WHILE_RESET(REGS, BIT_NUMS) while((REGS & (BIT_NUMS))==0)

#define LCD_OUT P2OUT
#define LCD_DIR P2DIR
#define D4 BIT0
#define D5 BIT1
#define D6 BIT2
#define D7 BIT3
#define RS BIT4
#define E  BIT5
#define DADOS 1
#define COMANDO 0
#define CMND_DLY 1000
#define DATA_DLY 1000
#define BIG_DLY  20000
#define CLR_DISPLAY  Send_Byte(1, COMANDO, BIG_DLY)
#define POS0_DISPLAY Send_Byte(2, COMANDO, BIG_DLY)

void Atraso_us(volatile unsigned int us);
void Send_Nibble(volatile unsigned char nibble, volatile unsigned char dados, volatile unsigned int microsegs);
void Send_Byte(volatile unsigned char byte, volatile unsigned char dados, volatile unsigned int microsegs);
void Send_Data(volatile unsigned char byte);
void Send_String(char str[]);
void Send_Int(int n);
void InitLCD(void);

int main(void)
{
	volatile int i = 0;
	WDTCTL = WDTHOLD + WDTPW;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR &= ~BTN;
	P1OUT = P1REN = BTN;
	InitLCD();
	Send_String("Pressione BTN");
	while(1)
	{
		// Espera o botao ser pressionado
		while(P1IN&BTN);
		// Espera o botao ser solto
		while((P1IN&BTN)==0);
		// Transmite a string
		CLR_DISPLAY;
		POS0_DISPLAY;
		Send_Int(i++);
		Send_String(": ola mundo!");
	}
	return 0;
}

void Atraso_us(volatile unsigned int us)
{
	TA1CCR0 = us-1;
	TA1CTL = TASSEL_2 + ID_0 + MC_1 + TAIE;
	while((TA1CTL & TAIFG)==0);
	TA1CTL = TACLR;
	TA1CTL = 0;
}

void Send_Nibble(volatile unsigned char nibble, volatile unsigned char dados, volatile unsigned int microsegs)
{
	LCD_OUT |= E;
	LCD_OUT &= ~(RS + D4 + D5 + D6 + D7);
	LCD_OUT |= RS*(dados==DADOS) +
		D4*((nibble & BIT0)>0) +
		D5*((nibble & BIT1)>0) +
		D6*((nibble & BIT2)>0) +
		D7*((nibble & BIT3)>0);
	LCD_OUT &= ~E;
	Atraso_us(microsegs);
}

void Send_Byte(volatile unsigned char byte, volatile unsigned char dados, volatile unsigned int microsegs)
{
	Send_Nibble(byte >> 4, dados, microsegs/2);
	Send_Nibble(byte & 0xF, dados, microsegs/2);
}

void Send_Data(volatile unsigned char byte)
{
	Send_Byte(byte, DADOS, DATA_DLY);
}

void Send_String(char str[])
{
	while((*str)!='\0')
	{
		Send_Data(*(str++));
	}
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
	for(casa = 10000; casa>n; casa /= 10);
	while(casa>0)
	{
		dig = (n/casa);
		Send_Data(dig+'0');
		n -= dig*casa;
		casa /= 10;
	}
}

void InitLCD(void)
{
	unsigned char CMNDS[] = {0x20, 0x14, 0xC, 0x6};
	unsigned int i;
	// Atraso de 10ms para o LCD fazer o boot
	Atraso_us(10000);
	LCD_DIR |= D4+D5+D6+D7+RS+E;
	Send_Nibble(0x2, COMANDO, CMND_DLY);
	for(i=0; i<4; i++)
		Send_Byte(CMNDS[i], COMANDO, CMND_DLY);
	CLR_DISPLAY;
	POS0_DISPLAY;
}
