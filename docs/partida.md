# Controlador da partida

`Partida` mantém os objetos da fase e coordena suas regras. Não cria janela,
não lê teclado e não desenha. A interface em `main.cpp` encaminha os comandos
do teclado para essa classe, avança o tempo da fase e desenha suas consultas.
Jogo e testes compartilham a biblioteca `BombermanLogica`.

`consumirEventos()` entrega e limpa os acontecimentos desde a ultima consulta:
passos horizontais/verticais aceitos, bombas colocadas/detonadas, coleta e derrota.
A interface encaminha esses eventos a `AudioJogo`, que usa SFML Audio e os WAV
de `assets/sounds`. As regras nao carregam arquivos nem abrem dispositivos de som.
Uma detonacao permanece na fila mesmo se sua explosao expirar no mesmo quadro;
cada bomba de uma cadeia registra seu proprio evento. Inicio, reinicio,
avanço de fase e encerramento descartam acontecimentos antigos.

## Uso pela interface

**Novo Jogo** chama `iniciarFase()`. Setas/WASD chamam `moverJogador`, Espaço
chama `colocarBomba`, e o ciclo da janela chama `atualizar` durante a partida.
O movimento repetido tem intervalo controlado pela interface. Ao perder o foco,
a janela pausa a fase e descarta o tempo decorrido antes de retomar.

A interface detecta `Vitoria` e chama `avancarFase()` no mesmo quadro. Nas três
primeiras fases, o método inicia o próximo mapa. Na quarta, retorna `false` e
a interface mostra a vitória final. R repete o mapa atual; Enter também repete
após a derrota. Após a vitória final, Enter ou R começa um novo jogo na fase 1.
Esc chama `encerrarFase()` e retorna ao menu. A renderização consulta o estado
atual a cada quadro, sem conservar referências de entidades entre reinícios ou fases.

```cpp
#include "Partida.hpp"

bomberman::Partida partida;
partida.iniciarFase();

// Exemplos de comandos, encaminhados pela interface conforme a entrada:
partida.moverJogador({0, 1}); // uma coluna à direita
partida.colocarBomba();
partida.atualizar(sf::milliseconds(16)); // tempo decorrido no ciclo
```

`obterEstado()` distingue `NaoIniciada`, `EmAndamento`, `Vitoria`, `Derrota` e
`Encerrada` (saída manual para o menu). Movimento e colocação retornam `false`
quando a ação não pode ser realizada. Depois do resultado, comandos e tempo
não alteram a fase; `reiniciarFase()` cria uma nova tentativa do mesmo mapa.
`avancarFase()` só aceita `Vitoria` quando existe um próximo mapa e incrementa
`obterNumeroFase()`, que vale zero antes do início. `obterQuantidadeFases()`
informa o total de mapas. `iniciarFase()` inicia um novo jogo na fase 1.

Para desenhar, use `obterMapa()`, `obterJogador()`, `obterInimigos()`,
`obterQuantidadeBombas()`/`obterBomba(indice)` e
`obterQuantidadeExplosoes()`/`obterExplosao(indice)`. Inclua os cabeçalhos das
entidades consultadas. Mapa e jogador só podem ser consultados após iniciar.
Índices inexistentes lançam `std::out_of_range`.

Essas consultas emprestam objetos: não guarde suas referências entre reinícios ou fases,
nem referências a bombas/explosões entre comandos ou atualizações. Posições
lógicas usam `{linha, coluna}`; os vetores SFML de bombas, explosões e melhorias
usam `{x = coluna, y = linha}`.

## Mapas fixos, avanço e reinício

O jogo tem quatro mapas fixos de 9 linhas por 11 colunas, jogados sempre na
ordem 1 → 2 → 3 → 4. O primeiro mantém o mapa original. Os outros três definem
suas próprias posições de blocos, melhorias, inimigos e porta. As fases incluem
alcance, bomba extra e escudo escondidos em blocos destrutíveis.

`reiniciarFase()` restaura o mesmo mapa e mantém o número da fase. A interface
limpa comandos, movimentos mantidos, sons e tempo acumulado ao avançar, antes
de receber a entrada da nova fase.

O construtor que recebe `ConfiguracaoFase` permite outras arenas, inclusive
cenários pequenos para testes. Cada `BlocoInicial` pode descrever uma melhoria
escondida, sem receber ponteiros externos. Essa configuração define um cenário
único, sem avanço para os mapas da campanha. A validação ocorre em `iniciarFase()`;
melhoria de velocidade ainda não é suportada e é rejeitada nesse momento.
O construtor com uma lista de `ConfiguracaoFase` permite sequências próprias,
usadas também nos testes de passagem entre mapas.

Cada início, reinício ou avanço recria mapa, jogador, inimigos e itens. Os
atributos voltam ao padrão (alcance 2, uma bomba e sem escudo). Bombas e explosões
antigas são destruídas antes dos objetos que referenciam. A nova fase é preparada
antes da substituição da antiga; nenhum registro de bomba, fogo ou item revelado
é transportado para ela.

## Ordem das regras

O tempo avança em passos de até 10 ms para verificar efeitos entre movimentos
dos inimigos. Tempos não positivos ou maiores que 5 segundos são descartados;
uma pausa longa não deve avançar a fase de uma só vez.

Em cada passo, a partida expira explosões antigas, avança bombas e resolve
detonações e cadeias. Depois movimenta os inimigos sobreviventes, reaplica fogo,
verifica contato, coleta e resultado. Uma cadeia é resolvida inteira no mesmo
passo. Inimigos que entram no fogo são eliminados antes de causar contato.

Mover o jogador resolve imediatamente fogo, contato, coleta e resultado.
Colocar uma bomba sobre fogo ativo a detona imediatamente. O construtor de
`Bomba` já reserva a vaga; sua detonação já libera vaga e ocupação e devolve a
explosão propagada, que passa a ser mantida pela partida.

Derrota tem precedência sobre vitória. Para vencer, o jogador precisa estar
vivo, eliminar todos os inimigos e ocupar a saída. O reinício restaura também
melhorias coletadas, atributos, terreno e temporizadores.
