# Microprocessadores e Microcontroladores - Plano de Ensino

- **ATENÇÃO! Revisão de notas amanhã, dia 11 de julho, das 15 às 18hs na minha sala, número 25.**
- **NOTAS FINAIS: https://docs.google.com/spreadsheets/d/1N3INpMJp0XAF44n4lJ6rdwtJmxTxw025nS4UiCCUbQs/edit?usp=sharing**
- **Notas e comentários do 1o Ponto de Controle: https://docs.google.com/spreadsheets/d/1ko0BqwfT8uPgXxI3doVsMtFuDL96hevU31ZVwyz5Uz8/edit?usp=sharing**
- Disciplina: https://matriculaweb.unb.br/graduacao/disciplina.aspx?cod=201383
- Oferta: https://matriculaweb.unb.br/graduacao/oferta_dados.aspx?cod=201383
- Código: 201383
- Créditos: 4
- Turma: A
- Locais e horários: Lab SS às 4as e 6as feiras, sempre das 14:00 às 15:50
- Professor: Diogo Caetano Garcia
- Email: diogogarcia(at)unb.br

# Datas importantes

- 04/04/2018: entrega do primeiro ponto de controle
- 18/04/2018: primeiro teste
- 02/05/2018: entrega do segundo ponto de controle
- 16/05/2018: segundo teste
- 30/05/2018: entrega do terceiro ponto de controle (adiada para 06/06/2018)
- 13/06/2018: entrega do quarto ponto de controle (adiada para 20/06/2018)
- 27/06/2018: entrega do projeto final (adiada para 04/07/2018)

# Objetivos

Capacitar o aluno no desenvolvimento de soluções baseadas em microcontroladores. Apresentar o desenvolvimento de sistemas de processamento digitais baseados em microprocessadores e microcontroladores, os dispositivos periféricos empregados e suas aplicações. Esta disciplina estabelece base para posteriores disciplinas dos cursos em Eletrônica, tais como sistemas embarcados.

# Ementa

- Visão geral de microcontroladores MSP430.
- Microprocessadores e linguagem de máquina.
- Entrada e saída digital.
- Funções e interrupções.
- Temporizadores.
- Entrada e saída analógica.
- Comunicação

# Método de Ensino

Aulas divididas de acordo com a técnica Pomodoro, com 3 períodos de 25 a 30 minutos, intercalados com descansos de 5 minutos:

- 1o período: aula expositiva do professor;
- 2o período: alunos responderão a perguntas teóricas propostas pelo professor;
- 3o período: alunos responderão a perguntas práticas propostas pelo professor.

Dentro do 2o e do 3o períodos, cada aluno deverá desenvolver ao máximo suas respostas, aproveitando o tempo disponível à exaustão, como esperado na técnica Pomodoro. As datas de entrega das respostas serão os fins de semana correspondentes às aulas.

Todas as respostas deverão ser armazenadas em um repositório GitHub público. Utilize arquivos diferentes para as respostas, separando-as por data. Organize bem o seu repositório, com pastas individuais nomeadas **1_Respostas**, **2_PCs** e **3_Trabalho** para guardar as respostas das aulas, os pontos de controle e o trabalho final, respectivamente.

Será utilizada a placa Launchpad do MSP430 (http://www.ti.com/tool/msp-exp430g2) em toda a disciplina.

# Avaliação

- Dois testes (40% da nota final);
- Projeto de fim de curso (30% da nota final);
- Pontos de controle (20% da nota final).
- Respostas dos ciclos Pomodoro em sala de aula (10% da nota final).

Os alunos formarão duplas para montar um projeto final de maior dificuldade, cobrindo os tópicos vistos em sala de aula. Os projetos serão propostos pelos alunos, e será apresentado um protótipo em funcionamento e o relatório do mesmo, na forma de relatório científico com formatação IEEE, contendo os seguintes tópicos:

  1. Resumo (1 ponto): Apresentar o projeto de forma geral e em poucas palavras.
  2. Introdução (1 ponto): Descrição do problema a ser resolvido e revisão bibliográfica do assunto.
  3. Desenvolvimento (5 pontos): Solução, com justificativa baseada em fundamentos teóricos. O desenvolvimento deverá conter a descrição do hardware e do software:
    1. Descrição do hardware (2 pontos) , incluindo a BOM (bill of materials) e a montagem (diagrama de blocos, esquemático, descrição textual etc.).
    2. Descrição do software (3 pontos) , apresentando e justificando o algoritmo desenvolvido. Esta seção do relatório NÃO CONSISTE em simplesmente replicar o código, e sim em explicar como ele funciona, com base em fluxogramas, pseudo-códigos etc. O código deverá ser apresentado como um apêndice do relatório.
  4. Resultados (2 pontos): Explicar os experimentos definidos para validar o projeto proposto, seguido de análise crítica dos resultados esperados e obtidos. Em caso de divergências, apontar as possíveis causas.
  5. Conclusão (1 ponto): Retomar sucintamente os principais pontos do relatório: descrição do problema, solução utilizada e resultados obtidos.

Os pontos de controle serão prévias do projeto final. Cada dia de atraso na apresentação dos pontos de controle acarreta na perda de 0,5 pontos da nota do mesmo. Serão esperados os seguintes resultados para cada ponto de controle:

- PC1: proposta do projeto (justificativa, objetivos, requisitos, benefícios, revisão bibliográfica).
- PC2: protótipo funcional do projeto, utilizando as ferramentas mais básicas (Arduino para MSP430 - http://energia.nu/), bibliotecas prontas etc.
- PC3: refinamento do protótipo em linguagem C, acrescentando recursos básicos de sistema. É obrigatório acrescentar interrupções e uma sub-rotina significativa do código em Assembly. O uso de modos de baixo consumo é encorajado.
- PC4: refinamento do protótipo em linguagem C, acrescentando temporização bem definida do sistema.

Todos os relatórios dos pontos de controle serão entregues através do mesmo repositório GitHub das respostas às perguntas em sala de aula. Crie uma pasta separada no repositório para a entrega dos pontos de controle. DEVERÁ HAVER SOMENTE UM ARQUIVO POR RELATÓRIO, EM FORMATO PDF. Em caso de cópias (integrais ou parciais), ambos os relatórios ficarão com nota ZERO. Os diferentes templates com formatação IEEE estão disponíveis em:

- Word: http://goo.gl/ghgLsR
- Unix LaTeX: http://goo.gl/p3ExfQ
- Windows LaTeX: http://goo.gl/gAxi96

# Bibliografia

Básica

- Davies, J., MSP430 Microcontroller Basics, Elsevier, 2008.

Complementar

- Patterson, D.; Hennessy, J.L, Organização & Projeto de Computadores: a Interface Hardware/Software, 2a ed., LTC, 2000.
- Pereira, F., Microcontroladores MSP430: Teoria e Prática, 1a ed., Érica, 2005.
