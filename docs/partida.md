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
cada bomba de uma cadeia registra seu proprio evento. Inicio, reinicio e
encerramento descartam acontecimentos antigos.

## Uso pela interface

**Novo Jogo** chama `iniciarFase()`. Setas/WASD chamam `moverJogador`, Espaço
chama `colocarBomba`, e o ciclo da janela chama `atualizar` durante a partida.
O movimento repetido tem intervalo controlado pela interface. Ao perder o foco,
a janela pausa a fase e descarta o tempo decorrido antes de retomar.

A interface mostra vitória ou derrota conforme `obterEstado()`. R reinicia a
fase durante o jogo ou no resultado; Enter também reinicia após o resultado.
Esc chama `encerrarFase()` e retorna ao menu. A renderização consulta o estado
atual a cada quadro, sem conservar referências de entidades entre reinícios.

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
não alteram a fase; `reiniciarFase()` cria uma nova tentativa.

Para desenhar, use `obterMapa()`, `obterJogador()`, `obterInimigos()`,
`obterQuantidadeBombas()`/`obterBomba(indice)` e
`obterQuantidadeExplosoes()`/`obterExplosao(indice)`. Inclua os cabeçalhos das
entidades consultadas. Mapa e jogador só podem ser consultados após iniciar.
Índices inexistentes lançam `std::out_of_range`.

Essas consultas emprestam objetos: não guarde suas referências entre reinícios,
nem referências a bombas/explosões entre comandos ou atualizações. Posições
lógicas usam `{linha, coluna}`; os vetores SFML de bombas, explosões e melhorias
usam `{x = coluna, y = linha}`.

## Fase e reinício

A configuração padrão é uma arena de 9 linhas por 11 colunas, com bordas e
pilares indestrutíveis, blocos destrutíveis, três inimigos e uma melhoria de
cada tipo obrigatório: alcance, bomba extra e escudo. O jogador inicia em
`{1, 1}` e a saída fica em `{7, 9}`. A área inicial oferece uma rota de fuga.

O construtor que recebe `ConfiguracaoFase` permite outras arenas, inclusive
cenários pequenos para testes. Cada `BlocoInicial` pode descrever uma melhoria
escondida, sem receber ponteiros externos. A validação ocorre em `iniciarFase()`;
melhoria de velocidade ainda não é suportada e é rejeitada nesse momento.

Cada início/reinício recria mapa, jogador, inimigos e itens a partir da
configuração original. Bombas e explosões antigas são destruídas antes dos
objetos que referenciam. A nova fase é preparada antes da substituição da antiga.

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
