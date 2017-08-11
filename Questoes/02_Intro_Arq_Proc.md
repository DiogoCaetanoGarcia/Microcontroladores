1. Quais as diferenças entre os barramentos de dados e de endereços?

2. Quais são as diferenças entre as memórias RAM e ROM?

3. Considere o código abaixo, de 12 linhas:

    1.   #include <stdio.h>
    2.   int main(void)
    3.   {
    4.      int i;
    5.      printf("Insira um número inteiro: ");
    6.      scanf("%d", &i);
    7.      if(i%2)
    8.         printf("%d eh impar.\n");
    9.      else
   10.         printf("%d eh par.\n");
   11.      return 0;
   12.   }

Para este código, responda:
	(a) A variável 'i' é armazenada na memória RAM ou ROM? Por quê?
	(b) O programa compilado a partir deste código é armazenado na memória RAM ou ROM? Por quê?

4. Quais são as diferenças, vantagens e desvantagens das arquiteturas Harvard e Von Neumann?

5. Considere a variável inteira 'i', armazenando o valor 0x8051ABCD. Se 'i' é armazenada na memória a partir do endereço 0x0200, como ficam este byte e os seguintes, considerando que a memória é:
	(a) Little-endian?
	(b) Big-endian?

6. Sabendo que o processador do MSP430 tem registradores de 16 bits, como ele soma duas variáveis de 32 bits?