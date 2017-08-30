Para todas as questões, considere que as variáveis `f`, `g`, `h`, `i` e `j` são do tipo inteiro (16 bits na arquitetura do MSP430), e que o vetor `A[]` é do tipo inteiro. Estas variáveis estão armazenadas nos seguintes registradores:

- f: R4
- g: R5
- h: R6
- i: R7
- j: R8
- A: R9

Utilize os registradores R11, R12, R13, R14 e R15 para armazenar valores temporários.

1. Traduza as seguintes linhas em C para a linguagem assembly do MSP430. Utilize somente as seguintes instruções: mov.w, add.w e sub.w.

(a) `f = 0;`

(b) `g++;`

(c) `h--;`

(d) `i += 2;`

(e) `j -= 2;`

2. Traduza as seguintes linhas em C para a linguagem assembly do MSP430. Utilize somente as seguintes instruções: mov.w, add.w, sub.w, clr.w, dec.w, decd.w, inc.w e incd.w.

(a) `f = 0;`

(b) `g++;`

(c) `h--;`

(d) `i += 2;`

(e) `j -= 2;`

3. Traduza as seguintes linhas em C para a linguagem assembly do MSP430. Utilize somente as seguintes instruções: mov.w, add.w, sub.w, clr.w, dec.w, decd.w, inc.w e incd.w.

(a) `f *= 2;`

(b) `g *= 3;`

(c) `h *= 4;`

(d) `A[2] = A[1] + A[0];`

(e) `A[3] = 2*f - 4*h;`

(f) `A[3] = 2*(f - 2*h);`
