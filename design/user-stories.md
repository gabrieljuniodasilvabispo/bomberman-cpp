# User Stories - Bomberman

Este documento apresenta os requisitos funcionais iniciais do jogo Bomberman. As historias foram escritas do ponto de vista dos usuarios e devem orientar a modelagem e a implementacao do sistema.

## US01 - Iniciar uma partida individual

**Como jogador, quero iniciar uma partida individual para enfrentar os desafios de uma fase do Bomberman.**

### Criterios de aceitacao

- A partida deve ser iniciada com um jogador.
- O jogador deve aparecer em uma posicao inicial segura do mapa.
- Os inimigos devem aparecer em posicoes diferentes da posicao inicial do jogador.
- O mapa deve conter limites, paredes indestrutiveis e blocos destrutiveis.
- Os comandos do jogador devem ser apresentados antes do inicio da partida.

## US02 - Movimentar o personagem

**Como jogador, quero movimentar meu personagem pela arena para explorar o mapa e escapar das explosoes.**

### Criterios de aceitacao

- O personagem deve poder se movimentar para cima, para baixo, para a esquerda e para a direita.
- O personagem deve responder aos comandos definidos para o jogador.
- O personagem nao deve atravessar paredes nem blocos.
- O personagem deve permanecer dentro dos limites do mapa.

## US03 - Posicionar bombas

**Como jogador, quero posicionar bombas na arena para destruir blocos e eliminar inimigos.**

### Criterios de aceitacao

- A bomba deve ser posicionada na celula ocupada pelo jogador ao receber o comando correspondente.
- A bomba deve explodir automaticamente depois de um intervalo definido.
- O jogador nao deve posicionar mais bombas simultaneas do que o limite permitido.
- Depois da explosao, a bomba deve ser removida da arena e voltar a ficar disponivel para o jogador.

## US04 - Interagir com explosoes

**Como jogador, quero que as explosoes interajam com os elementos da arena para que minhas decisoes tenham consequencias durante a partida.**

### Criterios de aceitacao

- A explosao deve se propagar nas quatro direcoes ate atingir seu alcance maximo ou um obstaculo.
- Paredes indestrutiveis devem interromper a explosao sem serem removidas.
- O primeiro bloco destrutivel atingido em cada direcao deve ser removido e interromper a propagacao naquela direcao.
- Uma explosao deve eliminar um inimigo atingido e causar a derrota quando atingir o jogador.
- Uma bomba atingida por uma explosao deve explodir imediatamente, provocando uma reacao em cadeia.

## US05 - Coletar melhorias

**Como jogador, quero coletar melhorias escondidas nos blocos para aumentar minhas possibilidades durante a partida.**

### Criterios de aceitacao

- Um bloco destrutivel pode revelar uma melhoria ao ser destruido.
- A melhoria deve permanecer na arena ate ser coletada ou destruida por uma explosao.
- A melhoria deve ser coletada quando um jogador ocupar sua posicao.
- O jogo deve oferecer pelo menos melhorias de alcance da explosao, quantidade simultanea de bombas e escudo resistente a explosao.

## US06 - Concluir ou reiniciar a fase

**Como jogador, quero visualizar o resultado da fase e poder reinicia-la para continuar jogando depois de uma vitoria ou derrota.**

### Criterios de aceitacao

- O jogador deve vencer depois de eliminar todos os inimigos e alcancar a saida da fase.
- O jogador deve perder ao ser atingido por uma explosao ou por um inimigo.
- O jogo deve informar se o jogador venceu ou perdeu.
- O jogo deve oferecer a opcao de reiniciar a fase.
- Ao reiniciar, o mapa, os jogadores, as bombas e as melhorias devem voltar ao estado inicial.

## Referencia

- [User story - Wikipedia](https://en.wikipedia.org/wiki/User_story)
