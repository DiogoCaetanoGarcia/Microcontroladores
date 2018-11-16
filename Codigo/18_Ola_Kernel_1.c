#include <stdio.h>

// Tarefas
void tst1(void)
{
	printf("1 ");
}
void tst2(void)
{
	printf("22 ");
}
void tst3(void)
{
	printf("333 ");
}

//declaracao do tipo ponteiro para funcao
typedef void(*ptrFunc)(void);

//variáveis do kernel
#define BUFFSIZE 6
ptrFunc pool[BUFFSIZE];
int ini;
int fim;
#define next(elem, tam) ((elem+1)%tam)

//funções do kernel
void InicializaKernel(void)
{
	ini = 0;
	fim = 0;
}
void AddKernel(ptrFunc newFunc)
{
	if(next(fim, BUFFSIZE) != ini)
	{
		pool[fim] = newFunc;
		fim = next(fim, BUFFSIZE);
	}
}
void ExecutaKernel(void)
{
	int i;
	//no microcontrolador real o loop abaixo
	//deve ser trocado por um loop infinito
	//na implementação em desktop utilizamos um loop
	//com 300 iterações para simular o funcionamento do kernel
	for(i=0;i<300;i++)
	{
		(*pool[ini])();
		ini = next(ini, fim);
	}
}

int main(void)
{
	printf("Inicio\n\n");
	InicializaKernel();
	AddKernel(tst1);
	AddKernel(tst2);
	AddKernel(tst3);
	ExecutaKernel();
	printf("\n\nFim\n");
	return 0;
}