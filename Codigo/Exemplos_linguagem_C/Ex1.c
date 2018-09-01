/*
	Ex1.c - Código que calcula os
	valores de mínimo, de míximo,
	de média e de variância de um vetor.

	Toda a lógica do programa está
	contida na função main(), o que
	dificulta a organização, o
	entendimento e o reuso do código.
*/
#include <stdio.h>

void main()
{
	// Vetor para guardar dados de
	// calculos estatisticos
	int v[100];
	// Variaveis de analise do vetor
	int max, min;
	float media, var;
	// Variaveis intermediarias
	float d;
	int i;
	
	// Vamos preencher o vetor com
	// os valores {0,1,4,9,16,25,...,9801}
	// para podermos rodar testes
	for(i=0; i<100; i++)
		v[i] = i*i;

	// Vamos achar o valor maximo do vetor
	max = v[0];
	for(i=1; i<100; i++)
		if(v[i]>max)
			max = v[i];

	// Vamos achar o valor minimo do vetor
	min = v[0];
	for(i=1; i<100; i++)
		if(v[i]<min)
			min = v[i];
	
	// Vamos achar a media do vetor
	media = v[0];
	for(i=1; i<100; i++)
		media += (float) v[i];
	media /= 100.0;

	// Vamos achar a variancia nao-enviesada do vetor
	// https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation
	var = 0.0;
	for(i=0; i<100; i++)
	{
		d = ((float)v[i]) - media;
		var += d*d;
	}
	var /= 99.0;
	
	// Vamos ler os resultados
	printf("Min = %d\n", min);
	printf("Max = %d\n", max);
	printf("Media = %f\n", media);
	printf("Variancia = %f\n", var);
}
