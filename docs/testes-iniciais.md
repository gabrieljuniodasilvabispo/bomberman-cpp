# Guia de testes do Bomberman

Este guia descreve os testes automáticos das regras do jogo e do terminal,
além das verificações manuais da interface e do áudio. Inclui os testes da
sequência de quatro fases, dos reinícios e dos eventos usados pelos efeitos sonoros.

Os testes usam doctest e exercitam as classes isoladamente e em conjunto por
meio de `Partida`. Jogo e testes utilizam a mesma biblioteca de regras,
`BombermanLogica`. O CTest executa essa suíte e verifica ajuda e argumentos
inválidos do executável, sem abrir janela. Todos os testes devem passar.

## Como executar os testes

No PowerShell, na pasta do projeto:

```powershell
mingw32-make test
```

Esse comando configura o projeto, compila o jogo e os testes e executa o CTest,
mostrando os detalhes em caso de falha.

Para compilar os testes sem executá-los:

```powershell
mingw32-make test-build
```

Para ver os nomes dos testes disponíveis:

```powershell
mingw32-make list-tests
```

No Linux, use `make` no lugar de `mingw32-make`.
No Windows, a pasta `bin` do MinGW (por exemplo, `C:\msys64\ucrt64\bin`)
deve estar no `PATH` para localizar o compilador e suas bibliotecas.

Para compilar, são necessários um compilador compatível com C++17, CMake 3.22
ou superior, GNU Make, Ninja e as bibliotecas necessárias para a SFML 3.0.2.
A primeira configuração precisa de internet para baixar a SFML e as dependências
de áudio, incluindo Ogg, Vorbis e FLAC. O doctest 2.4.12 e sua licença já estão
incluídos na pasta `third_party/doctest`.

O projeto usa Ninja como padrão para organizar a compilação. No Linux, é
possível usar outra opção com `GENERATOR="Unix Makefiles"`. Nesse caso, informe
uma nova pasta de compilação com `BUILD_DIR`, para não misturar as configurações.

## Como interpretar o resultado

O CTest apresenta cinco entradas: a suíte doctest como um teste agregado,
chamado `testes_iniciais`, e quatro cenários de terminal. Portanto, cinco entradas
aprovadas no CTest não significam que existem apenas cinco casos de teste.

| Entrada do CTest | O que verifica |
| --- | --- |
| `testes_iniciais` | Todos os casos doctest das classes, integração da partida, fases e programação defensiva |
| `cli_ajuda` | `--ajuda` apresenta instruções e termina com código 0 |
| `cli_help` | `--help` apresenta instruções e termina com código 0 |
| `cli_invalido` | Uma opção desconhecida apresenta erro e termina com código 2 |
| `cli_excesso` | Argumentos em excesso apresentam erro e terminam com código 2 |

Os testes de terminal também verificam se a mensagem é enviada para a saída
correta: saída padrão para ajuda e saída de erro para argumentos inválidos.

Para ver os nomes dos casos doctest, use `mingw32-make list-tests`. Para executar
a suíte diretamente e consultar o resumo de casos e asserções, use
`.\build\BombermanTests.exe` no PowerShell ou `./build/BombermanTests` no Linux,
após compilar. Se definir outro `BUILD_DIR`, ajuste o caminho do executável.

Erros de compilação ou asserções precisam ser corrigidos. Os arquivos de
`tests/tdd` são auxiliares históricos e não participam da compilação definida
no `CMakeLists.txt`; os testes atuais usam as implementações de `src`.

## O que os testes automáticos verificam

| Classe ou área | Verificações |
| --- | --- |
| Mapa | Dimensões e configuração inicial, limites, obstáculos, ocupação por bombas, registro de melhorias, cópias dos dados de entrada, consultas inválidas e reinícios repetidos |
| Bloco | Destruição dos blocos destrutíveis, resistência dos indestrutíveis e revelação de melhoria uma única vez |
| Jogador | Estado inicial, movimento nas quatro direções, bloqueio por obstáculos e bombas, rejeição de diagonais e saltos e derrota por explosão |
| Inimigo | Movimento temporizado, obstáculos, contato com o jogador, eliminação e rejeição de tempos e posições inválidos |
| Bomba | Posição, alcance, detonação após três segundos, antecipação, reserva e devolução de vagas e preservação do estado em falhas |
| Explosao | Propagação em cruz, alcance, limites, interrupção por blocos, dano, escudo, expiração, reação em cadeia e destruição de melhorias reveladas |
| Melhoria | Aumento de alcance, bomba extra, aplicação do benefício uma única vez e proteção do escudo |
| Menu | Opções disponíveis, confirmação da escolha e seleção dentro dos limites da lista |
| Partida | Criação da fase, movimento, bombas, cadeia, dano, coleta, vitória/derrota, controle do tempo e restauração completa no reinício |
| Fases (`test_fases.cpp`) | Quatro mapas no jogo padrão, preservação do primeiro mapa, sequência de avanço em arenas de teste, reinício da fase atual, término sem quinta fase e rejeição de configurações inválidas |
| Eventos de áudio | Registro das ações aceitas, consumo único, uma detonação por bomba em cadeia, coleta, derrota e limpeza ao iniciar, reiniciar, avançar ou encerrar |
| Posicao | Valores de linha e coluna e cópia que pode ser alterada sem modificar o original |
| Programação defensiva | Entradas inválidas, limites de recursos, estado preservado após rejeição e valores extremos |
| Terminal | Ajuda, opção desconhecida e excesso de argumentos, validando código de saída e mensagem |

Os testes de eventos verificam a fila produzida por `Partida` e consumida pela
interface para acionar `AudioJogo`. Eles não reproduzem som nem dependem de um
dispositivo de áudio. Teclado, desenho da janela e reprodução dos efeitos
precisam das verificações manuais abaixo. Os contratos e a ordem das regras
estão detalhados no [guia da partida](partida.md).

## Verificação da interface

Execute `mingw32-make run` (Windows) ou `make run` (Linux).

- Consulte as instruções e escolha **Novo Jogo**: a partida deve começar na fase 1.
- Use setas/WASD para andar, verificando limites, blocos e bombas.
- Coloque uma bomba com Espaço, saia de seu alcance e confira a detonação.
- Confira a revelação/coleta dos itens A/B/E e a atualização dos atributos.
- Verifique a derrota por fogo ou contato. Após a derrota, Enter deve repetir
  o mesmo mapa e manter o número da fase.
- Elimine os inimigos e alcance a saída verde. Nas três primeiras fases, confira
  o avanço automático na ordem 1 → 2 → 3 → 4. A saída do quarto mapa deve mostrar
  a vitória final, sem criar uma quinta fase.
- Use R durante a partida para repetir a fase atual. Confira a restauração de
  terreno, personagens e melhorias, sem bombas ou explosões antigas. Ao reiniciar
  ou avançar, os atributos devem voltar ao padrão: alcance 2, uma bomba e sem escudo.
- Após a vitória final, confira que Enter ou R começa um novo jogo na fase 1.
- Use Esc para voltar ao menu. Escolher **Novo Jogo** deve retornar à fase 1.
- Alterne para outra janela e confira que a fase pausa e retoma sem avançar
  os temporizadores durante a ausência.
- Confira os sons de passos, bomba colocada, explosão, coleta e derrota.
  Movimento bloqueado e bomba recusada não devem tocar efeitos. Em uma cadeia,
  cada bomba gera uma detonação; permanecer na tela de derrota não repete o som.
- Ao perder o foco, confira o efeito de pausa e a suspensão dos sons em andamento;
  ao retornar, confira a retomada. Reiniciar, avançar de fase e voltar ao menu
  devem interromper os sons antigos.

## Cobertura de código

Para instrumentar o código próprio com GCC/gcov, configure
`-DBOMBERMAN_COVERAGE=ON` ou execute `mingw32-make test COVERAGE=ON`
(Linux: `make test COVERAGE=ON`). A instrumentação inclui a biblioteca de regras
e o executável do jogo, com interface e áudio; dependências externas e testes
não são instrumentados.
Gerar o relatório com gcovr e verificar a meta de 60% continuam sendo etapas
separadas; executar testes com instrumentação não comprova essa porcentagem.
