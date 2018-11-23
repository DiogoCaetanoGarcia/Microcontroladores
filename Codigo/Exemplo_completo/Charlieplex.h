#ifndef _CHARLIEPLEX_H_
#define _CHARLIEPLEX_H_

#include <msp430g2553.h>
#include "Kernel.h"

#define N_CPX 6
#define CPX0 BIT0
#define CPX1 BIT1
#define CPX2 BIT2
#define CPXs (CPX0|CPX1|CPX2)

unsigned int escala;

void Charlieplex_3(void);

#endif