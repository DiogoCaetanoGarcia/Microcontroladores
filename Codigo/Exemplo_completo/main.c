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

// PARA COMPILAR NO msp430-gcc E RODAR O CODIGO:
//    msp430-gcc -Os -Wall -g -mmcu=msp430g2553 -o out.elf main.c Kernel.c Button.c Servo.c Charlieplex.c AD.c
//    sudo mspdebug rf2500 "prog out.elf"
//    rm out.elf

#include <msp430g2553.h>
#include "Kernel.h"
#include "Servo.h"
#include "Button.h"
#include "Charlieplex.h"
#include "AD.h"

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