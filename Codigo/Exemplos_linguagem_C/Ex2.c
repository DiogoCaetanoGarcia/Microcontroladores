/*
	Ex2.c - Código que calcula os
	valores de mínimo, de míximo,
	de média e de variância de um vetor.

	Toda a análise do vetor foi
	separada em funções específicas,
	melhorando a organização, o
	entendimento e o reuso do código.
	Mesmo assim, as variáveis principais
	de análise ainda estão "espalhadas"
	ao longo da função main().
*/
#include <stdio.h>

// Calculo do valor maximo de um vetor
int calc_max(int v[100]);
// Calculo do valor minimo de um vetor
int calc_min(int v[100]);
// Calculo do valor medio de um vetor
float calc_media(int v[100]);
// Calculo da variancia de um vetor
float calc_var(int v[100]);

void main()
{
	// Vetor para guardar dados de
	// calculos estatisticos
	int v[100];
	// Variaveis de analise do vetor
	int max, min;
	float media, var;
	// Variavel intermediaria
	int i;

	// Vamos preencher o vetor com
	// os valores {0,1,4,9,16,25,...,9801}
	// para podermos rodar testes
	for(i=0; i<100; i++)
		v[i] = i*i;
	
	// Vamos analisar o vetor
	max = calc_max(v);
	min = calc_min(v);
	media = calc_media(v);
	var = calc_var(v);

	// Vamos ler os resultados
	printf("Min = %d\n", min);
	printf("Max = %d\n", max);
	printf("Media = %f\n", media);
	printf("Variancia = %f\n", var);
}

// Calculo do valor maximo de um vetor
int calc_max(int v[100])
{
	int max, i;

	max = v[0];
	for(i=1; i<100; i++)
		if(v[i]>max)
			max = v[i];
	return max;
}

// Calculo do valor minimo de um vetor
int calc_min(int v[100])
{
	int min, i;

	min = v[0];
	for(i=1; i<100; i++)
		if(v[i]<min)
			min = v[i];
	return min;
}

// Calculo do valor medio de um vetor
float calc_media(int v[100])
{
	float media;
	int i;

	media = v[0];
	for(i=1; i<100; i++)
		media += (float) v[i];
	media /= 100.0;
	return media;
}

// Calculo da variancia de um vetor
float calc_var(int v[100])
{
	float var, media, d;
	int i;

	media = calc_media(v);
	var = 0.0;
	for(i=0; i<100; i++)
	{
		d = ((float)v[i]) - media;
		var += d*d;
	}
	var /= 99.0;
	return var;
}