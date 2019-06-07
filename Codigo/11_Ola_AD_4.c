#include <msp430g2553.h>

#define IN_AD BIT0+BIT1
#define IN_AD_CH INCH_1
#define LED BIT6

void atraso(volatile unsigned int d)
{
	while(d--);
}

void pisca(volatile unsigned char pisca)
{
	while(pisca--)
	{
		P1OUT |= LED;
		atraso(25000);
		P1OUT &= ~LED;
		atraso(25000);
	}
}

int main(void)
{
	volatile unsigned int res[2];
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1OUT |= LED;
	P1DIR |= LED;
	
	ADC10CTL1 = IN_AD_CH + CONSEQ_1 + ADC10DIV_0 + ADC10SSEL_3 + SHS_0;
	ADC10CTL0 = MSC + SREF_0 + ADC10SHT_2 + ADC10ON;
	ADC10AE0 = IN_AD;
	ADC10DTC1 = 2;
	pisca(2);
	P1OUT |= LED;

	while(1)
	{
		// Manda comecar uma conversao
		ADC10CTL0 &= ~ENC;
		while(ADC10CTL1 & ADC10BUSY);
		ADC10SA = (unsigned int) res;
		ADC10CTL0 |= ENC + ADC10SC;
		while((ADC10CTL0 & ADC10IFG)==0);
		if((res[1] < 512) && (res[0] < 512))
			P1OUT &= ~LED;
		else
			P1OUT |= LED;
	}
	return 0;
}