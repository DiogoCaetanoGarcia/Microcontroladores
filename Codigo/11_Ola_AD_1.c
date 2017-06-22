#include <msp430g2553.h>

#define IN_AD BIT1
#define IN_AD_CH INCH_1
#define LED1 BIT0
#define LED2 BIT6
#define LEDS (LED1|LED2)

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1OUT &= ~LEDS;
	P1DIR |= LEDS;
	
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10AE0 = IN_AD;
	// Com SHS_0, uma conversao sera requisitada
	// sempre que o bit ADC10SC for setado em ADC10CTL0
	ADC10CTL1 = IN_AD_CH + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0 + SHS_0; 
	
	while(1)
	{
		// Manda comecar uma conversao
		ADC10CTL0 |= ENC + ADC10SC;
		// Espera a conversao ficar pronta
		while((ADC10CTL0 & ADC10IFG)==0);
		
		// Acende ou apaga o LED1
		// de acordo com a conversao AD
		if(ADC10MEM < 512)
			P1OUT &= ~LED1;
		else
			P1OUT |= LED1;

		// Inverte o LED2 para vermos
		// a temporizacao deste loop
		P1OUT ^= LED2;
	}
	return 0;
}
