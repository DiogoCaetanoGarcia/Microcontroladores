/*
	Ex4.c - Código que calcula os
	valores de mínimo, de míximo,
	de média e de variância de um vetor.

	Todas as informações de análise do vetor
	foram agrupadas em uma estrutura,
	e uma função específica realiza a análise.
	Isto melhora a organização, o
	entendimento e o reuso do código, e
	agrupa as variáveis principais de análise
	em um mesmo local, a estrutura.

	A função que analisa o vetor recebe o
	endereço da estrutura a ser preenchida,
	evitando a cópia de duas estruturas na
	chamada da função (ver Ex3.c).
*/
#include <stdio.h>

struct vetor
{
	// Vetor para guardar dados de
	// calculos estatisticos
	int v[100];
	// Variaveis de analise do vetor
	int max, min;
	float media, var;
};

// Calculo do minimo, do maximo,
// da media e da variancia de um vetor
void calcula_stats(struct vetor *A);

void main()
{
	// Estrutura com o vetor
	// e as variaveis de analise
	struct vetor X;
	// Variavel intermediaria
	int i;
	
	// Vamos preencher o vetor com
	// os valores {0,1,4,9,16,25,...,9801}
	// para podermos rodar testes
	for(i=0; i<100; i++)
		X.v[i] = i*i;
	
	// Vamos analisar o vetor
	calcula_stats(&X);
	
	// Vamos ler os resultados
	printf("Min = %d\n", X.min);
	printf("Max = %d\n", X.max);
	printf("Media = %f\n", X.media);
	printf("Variancia = %f\n", X.var);
}

// Calculo do minimo, do maximo,
// da media e da variancia de um vetor.
// Usamos um ponteiro para uma estrutura
// para evitar ter de copiar toda uma
// estrutura na entrada e na saída da funcao,
// como foi o caso do codigo no arquivo Ex3.c
void calcula_stats(struct vetor *A)
{
	// Variaveis intermediarias
	int i;
	float d;
	// Loop para calculo do minimo,
	// do maximo e da media
	A->max = A->min = A->v[0];
	A->media = (float)A->v[0];
	for(i=1; i<100; i++)
	{
		if((A->v[i]) > (A->max))
			A->max = A->v[i];
		if((A->v[i]) < (A->min))
			A->min = A->v[i];
		A->media += (float)A->v[i];
	}
	A->media /= 100.0;
	// Loop para calculo da variancia
	for(A->var=0.0, i=0; i<100; i++)
	{
		d  = (float)A->v[i];
		d -= A->media;
		A->var += d*d;
	}
	A->var /= 99.0;
}