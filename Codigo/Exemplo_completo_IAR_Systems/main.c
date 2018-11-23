// Codigo para acender 0 a 6 LEDs charliplexados
// de acordo com a leitura do conversor A/D.
// Utiliza agendamento de tarefas, com intervalo
// de 833us entre cada uma. Assim, os LEDs
// piscam a 100 Hz.
// Alem disso, a posicao do potenciometro
// define a posicao de um servo SG90
// ligado ao pino P1.6. O acionamento do
// servo pelo potenciometro é ligado ou
// desligado através do botão da placa
// ligado ao pino P1.3.

#include "io430.h"
#include "Kernel.h"
#include "Servo.h"
#include "Charlieplex.h"
#include "AD.h"
#include "Button.h"
#include "global_vars.h"

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	Config_AD();
	Servo_Off();
	Config_BTN();
	InicializaKernel();
	AddKernel(Mede_Potenciometro);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	AddKernel(Null_func);
	AddKernel(Charlieplex_3);
	ExecutaKernel();
	return 0;
}