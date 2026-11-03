# Folha de Avaliação do Projeto Codexion

Página do projeto: <https://projects.intra.42.fr/projects/codexion>

---

## Introdução

- Mantém-te educado, cortês, respeitoso e construtivo durante todo o processo de avaliação. O bem-estar da comunidade depende disso.
- Identifica com a pessoa (ou o grupo) avaliada as eventuais disfunções do trabalho. Dedica tempo a discutir e debater os problemas identificados.
- Deves considerar que pode haver diferenças na forma como os teus colegas entenderam as instruções do projeto e o âmbito das suas funcionalidades. Mantém sempre a mente aberta e avalia da forma mais honesta possível. A pedagogia só é válida se a avaliação entre pares for conduzida a sério.

## Diretrizes

- Avalia apenas o trabalho que está no repositório Git do estudante ou do grupo.
- Confirma que o repositório Git pertence ao estudante ou ao grupo. Garante que o trabalho corresponde ao projeto em causa e verifica também que o `git clone` é feito numa pasta vazia.
- Verifica cuidadosamente que não foram usados *aliases* maliciosos para te enganar e levar-te a avaliar algo diferente do conteúdo do repositório oficial.
- Para evitar surpresas, verifica cuidadosamente que tanto o estudante avaliador como o avaliado reviram os possíveis scripts usados para facilitar a avaliação.
- Se o estudante avaliador ainda não concluiu este projeto, é obrigatório que leia todo o enunciado antes de iniciar a defesa.
- Usa as *flags* disponíveis nesta folha para sinalizar um repositório vazio, um programa que não funciona, um erro de norma, batota, etc. Nestes casos, a avaliação termina e a nota final é 0 (ou -42 em caso de batota). No entanto, com exceção da batota, és incentivado a continuar a discutir o trabalho (mesmo que não esteja terminado) para identificar os problemas que possam ter causado a falha e evitar repetir o mesmo erro no futuro.
- Durante toda a defesa, não pode ocorrer nenhum *segfault* nem qualquer outra terminação inesperada, prematura ou descontrolada do programa; caso contrário, a nota final é 0. Usa a *flag* apropriada.
- Nunca deverás ter de editar nenhum ficheiro, exceto o ficheiro de configuração, se existir. Se quiseres editar um ficheiro, explica os motivos ao estudante avaliado e certifica-te de que ambos estão de acordo.
- Deves também verificar a ausência de fugas de memória (*memory leaks*). Toda a memória alocada no *heap* tem de ser corretamente libertada antes do fim da execução. Podes usar qualquer uma das ferramentas disponíveis no computador, como `leaks`, `valgrind` ou `e_fence`. Em caso de fugas de memória, assinala a *flag* apropriada.
- Sendo este um projeto de concorrência, deves também verificar a existência de *data races* e *deadlocks*, que nem sempre são visíveis apenas lendo os logs. Usa as ferramentas específicas para *threads* disponíveis no computador, por exemplo:
  - `valgrind --tool=helgrind ./codexion ...`
  - `valgrind --tool=drd ./codexion ...`
  - recompilar o projeto com `-fsanitize=thread` e executá-lo.

  Se qualquer uma destas ferramentas reportar uma *data race* ou um *deadlock* por ordem de bloqueio (*lock-ordering*), assinala a *flag* apropriada. Não confies simplesmente na explicação do estudante avaliado de que o código é *thread-safe*.

## Anexos

- [subject.pdf](https://cdn.intra.42.fr/pdf/pdf/220993/en.subject.pdf)

---

## Preliminares

### Básico

O projeto tem de compilar com `-Wall -Wextra -Werror -pthread` sem qualquer aviso.

Qualquer erro de compilação, *segmentation fault*, *data race*, *deadlock*, linha de log entrelaçada ou fuga de memória equivale a 0 no projeto.

O programa tem de receber exatamente os seguintes argumentos, por esta ordem:

`number_of_coders`, `time_to_burnout`, `time_to_compile`, `time_to_debug`, `time_to_refactor`, `number_of_compiles_required`, `dongle_cooldown`, `scheduler`.

Cada *coder* tem de ser representado por uma *thread* (usando `pthread_create`).

O *scheduler* tem de ser `"fifo"` ou `"edf"`.

Em hardware lento, a temporização precisa pode variar ligeiramente; discute honestamente os casos-limite antes de os contares como falhas.

Verifica a compilação: `make`

O binário tem de se chamar `codexion`.

- [ ] Sim  [ ] Não

### Global

Verifica se existe alguma variável global mutável usada para gerir os recursos partilhados (*dongles*), a política de escalonamento ou o *logging*.

Se encontrares esse estado global, a avaliação termina aqui. Podes continuar a analisar o código, mas não avalies os exercícios.

- [ ] Sim  [ ] Não

### Ficheiro README.md

O repositório contém um ficheiro `README.md` na raiz e inclui todos os seguintes elementos?

- A primeira linha está em itálico e formatada exatamente como: *This project has been created as part of the 42 curriculum by `<login1>[, <login2>[, <login3>[...]]]`.*
- Uma secção **"Description"** que explica o propósito do projeto e apresenta uma breve visão geral.
- Uma secção **"Instructions"** com detalhes relevantes sobre compilação, instalação e/ou execução.
- Uma secção **"Resources"** que lista referências (documentação, tutoriais, etc.) e explica como a IA foi usada, especificando para que tarefas e que partes do projeto.
- Uma secção **"Blocking cases handled"** que descreve todos os problemas de concorrência abordados na solução.
- Uma secção **"Thread synchronization mechanisms"** que explica as primitivas de *threading* específicas utilizadas.

- [ ] Sim  [ ] Não

---

## Codexion

### Testes do Codexion — Fácil

- Não testes com mais de 200 *coders*.
- Não testes com valores de tempo inferiores a 60 ms.
- `number_of_compiles_required` tem de ser pelo menos 1: com 0, todos os *coders* já "compilaram pelo menos 0 vezes" no arranque e a simulação pode parar imediatamente.
- O primeiro caso abaixo tem de terminar em *burnout* (verifica a deteção de *burnout*); os restantes casos têm de correr sem qualquer *burnout* e completar todas as compilações necessárias.

**Comportamento de referência (usa estes casos como *benchmark*).** Os argumentos são, por ordem:

`number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler`

1. `./codexion 1 800 200 200 200 10 0 fifo`

   Um único *coder* tem um único *dongle*, mas precisa de dois para compilar. O *coder* nunca pode compilar e **TEM** de entrar em *burnout*, por volta do instante 800.

   - [ ] Sim  [ ] Não

2. `./codexion 5 2000 200 200 200 10 0 fifo`

   `time_to_burnout` (2000) é muito superior a um ciclo de compilar + depurar + refatorar (200 + 200 + 200 = 600), logo nenhum *coder* deve entrar em *burnout*; o programa tem de parar sozinho quando todos os *coders* tiverem compilado pelo menos 10 vezes.

   - [ ] Sim  [ ] Não

3. `./codexion 5 2000 200 200 200 7 0 edf`

   O mesmo, com o *scheduler* `edf`: sem *burnout*, para após 7 compilações por *coder*.

   - [ ] Sim  [ ] Não

Como não existe binário de referência, estes casos e o respetivo comportamento esperado **SÃO** o *benchmark*. Devido ao escalonamento do sistema operativo, executa cada caso várias vezes e espera o comportamento correto na grande maioria das execuções (permite uma pequena tolerância temporal nos casos-limite antes de os contares como falhas).

---

### Testes do Codexion — Menos fácil

- Valida os casos-limite de *burnout*.
- Verifica o *logging* correto e as tolerâncias temporais.
- Verifica que não há duplicação de *dongles* e que as transições de estado estão corretas.

**Comportamento de referência (casos de *benchmark*):**

`./codexion 5 500 200 200 200 10 0 fifo`

Inviável por construção: um ciclo de compilar + depurar + refatorar demora pelo menos 200 + 200 + 200 = 600 ms, o que é superior a `time_to_burnout` (500), pelo que um *coder* não consegue iniciar a compilação seguinte a tempo e **TEM** de entrar em *burnout* (por volta do instante 500). A linha "burned out" tem de ser a **ÚLTIMA** linha impressa e tem de aparecer a menos de 10 ms do limite.

Numa execução viável (por exemplo, o caso `5 2000 ...` da secção Fácil), verifica que cada linha "is compiling" é imediatamente precedida por exatamente duas linhas "has taken a dongle" do mesmo *coder*, e que nunca há duas linhas de log entrelaçadas na mesma linha.

Confirma que o mesmo *dongle* nunca é detido por dois *coders* ao mesmo tempo (sem duplicação de *dongles*).

- [ ] Sim  [ ] Não

---

### Testes do Codexion — Médio

- Verifica o comportamento do *cooldown*.
- Testa as diferenças entre *schedulers* (`edf` vs `fifo`).
- Valida a temporização da refatoração e a serialização dos logs.

**Comportamento de referência (casos de *benchmark*):**

1. `./codexion 5 3000 200 200 200 10 400 fifo`

   Verificação do *cooldown*: depois de um *coder* libertar um *dongle*, esse *dongle* não pode ser apanhado novamente até terem passado 400 ms. Lendo os *timestamps*, verifica que nenhuma linha "has taken a dongle" para um dado *dongle* ocorre nos 400 ms seguintes à sua libertação. `time_to_burnout` (3000) é generoso, pelo que nenhum *coder* deve entrar em *burnout* e a execução deve terminar.

2. `./codexion 5 3000 200 200 200 10 800 fifo` e depois `... edf`

   Executa exatamente os mesmos parâmetros uma vez com `fifo` e outra com `edf`. O *cooldown* elevado (800) obriga os *coders* a fazer fila pelos *dongles*. Compara a ordem pela qual os *coders* em espera recebem os *dongles*: com `edf`, um *coder* mais próximo do seu limite de *burnout* pode ser servido antes de um que pediu mais cedo, pelo que a ordem de atribuição pode diferir da ordem estrita de chegada observada com `fifo`.

Verifica que cada *coder* passa um tempo próximo de `time_to_compile` / `time_to_debug` / `time_to_refactor` em cada fase (dentro da tolerância temporal) e que o *output* se mantém corretamente serializado mesmo sob forte contenção.

- [ ] Sim  [ ] Não

---

### Recodificação do Codexion — de FIFO para LIFO

Pede ao estudante avaliado que transforme o *scheduler* `"fifo"` num `"lifo"`: quando vários *coders* estão à espera do mesmo *dongle*, este tem agora de ser atribuído ao *coder* cujo pedido chegou **POR ÚLTIMO**, em vez do primeiro.

Esta alteração é observável em tempo de execução, ao contrário de um empate nos *deadlines* do EDF. Executa o programa uma vez antes da alteração e outra depois, com os **MESMOS** parâmetros de forte contenção (por exemplo: `./codexion 5 3000 200 200 200 10 800 fifo`), e compara a ordem pela qual os *coders* em espera adquirem os *dongles* nos logs.

A recodificação é validada se a ordem de atribuição com `"lifo"` for o inverso da ordem de chegada (o pedido mais recente é servido primeiro), enquanto o resto do programa continua a funcionar: nenhum *coder* duplica um *dongle*, os logs permanecem serializados e, com parâmetros viáveis, nenhum *coder* entra em *burnout*.

Lê também o código para confirmar que a alteração foi feita na própria lógica do *scheduler*/fila e não "falsificada" ao nível dos logs.

- [ ] Sim  [ ] Não
- [ ] Sim  [ ] Não
