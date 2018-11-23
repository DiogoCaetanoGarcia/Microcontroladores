#include "AD.h"

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

void Config_AD(void)
{
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	ADC10AE0 = IN_AD;
	ADC10CTL1 = IN_AD_CH + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0 + SHS_0; 
}
