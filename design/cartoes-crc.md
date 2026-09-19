# Cartoes CRC - Bomberman

Este documento apresenta os cartoes Classe-Responsabilidade-Colaboracao das principais classes do jogo Bomberman. Os cartoes foram elaborados com base nas User Stories definidas para uma partida individual.

## CRC01 - Partida

### Responsabilidades

- Controlar o estado atual da partida.
- Iniciar uma nova fase.
- Criar o jogador e os inimigos nas posicoes iniciais.
- Atualizar os elementos ativos a cada ciclo do jogo.
- Encerrar a fase quando ocorrer uma condicao de vitoria ou derrota.
- Verificar se todos os inimigos foram eliminados.
- Permitir que o jogador reinicie a fase.
- Restaurar os elementos para seus estados iniciais durante o reinicio.

### Colaboracoes

- `Mapa`: fornece a arena e as posicoes validas da fase.
- `Jogador`: informa se o jogador continua ativo e se alcancou a saida.
- `Inimigo`: informa quantos inimigos ainda estao ativos.
- `Bomba`: recebe atualizacoes durante o ciclo da partida.
- `Menu`: solicita o inicio de uma partida individual.

## CRC02 - Mapa

### Responsabilidades

- Armazenar a organizacao das celulas da arena.
- Armazenar os blocos que representam paredes indestrutiveis e obstaculos destrutiveis.
- Informar se uma posicao pode ser ocupada.
- Impedir movimentos para fora dos limites da arena.
- Liberar a celula de um bloco depois que ele informar sua destruicao.
- Definir as posicoes iniciais seguras da fase.
- Armazenar a posicao da saida da fase.
- Fornecer os blocos presentes nas celulas alcancadas por uma explosao.

### Colaboracoes

- `Partida`: fornece a configuracao necessaria para iniciar ou reiniciar a fase.
- `Jogador`: valida as tentativas de movimento do personagem.
- `Inimigo`: valida as tentativas de movimento dos inimigos.
- `Explosao`: consulta as celulas e os blocos encontrados durante a propagacao.
- `Bloco`: informa se ocupa uma celula e quando foi destruido.
- `Melhoria`: recebe melhorias reveladas pela destruicao de blocos.

## CRC03 - Jogador

### Responsabilidades

- Armazenar sua posicao atual na arena.
- Armazenar se esta ativo ou foi derrotado.
- Interpretar os comandos de movimentacao.
- Solicitar ao mapa a validacao de um movimento.
- Solicitar o posicionamento de uma bomba.
- Controlar a quantidade de bombas que pode manter simultaneamente.
- Armazenar o alcance atual de suas bombas.
- Receber e aplicar o efeito de uma melhoria coletada.
- Usar o escudo ao ser atingido por uma explosao, quando disponivel.

### Colaboracoes

- `Mapa`: valida movimentos e informa os elementos presentes na posicao.
- `Bomba`: recebe a posicao, o alcance e a identificacao de seu responsavel.
- `Melhoria`: altera atributos do jogador quando coletada.
- `Explosao`: informa quando o jogador foi atingido.
- `Inimigo`: informa quando ocorreu contato entre os personagens.
- `Menu`: consulta os comandos do jogador para apresenta-los antes da partida.

## CRC04 - Bomba

### Responsabilidades

- Armazenar sua posicao na arena.
- Armazenar o alcance definido pelo jogador responsavel.
- Controlar o tempo restante ate a detonacao.
- Informar quando esta pronta para explodir.
- Iniciar uma explosao quando seu tempo terminar.
- Antecipar a detonacao ao ser atingida por outra explosao.
- Informar ao jogador quando deixar de ocupar uma vaga de bomba ativa.

### Colaboracoes

- `Jogador`: fornece o alcance da bomba e recupera uma vaga apos a detonacao.
- `Mapa`: valida a posicao em que a bomba sera colocada.
- `Explosao`: e criada quando ocorre a detonacao.
- `Partida`: atualiza o temporizador da bomba durante a fase.

## CRC05 - Explosao

### Responsabilidades

- Armazenar a posicao de origem da explosao.
- Armazenar seu alcance maximo.
- Calcular as celulas atingidas nas quatro direcoes.
- Notificar o bloco encontrado de que foi atingido, delegando a ele o tratamento da interacao.
- Interromper a propagacao na direcao atingida conforme a resposta do bloco.
- Eliminar personagens atingidos que nao estejam protegidos.
- Ativar imediatamente outras bombas atingidas.
- Permanecer ativa somente durante o tempo definido para seu efeito.

### Colaboracoes

- `Bomba`: fornece a origem e o alcance e recebe reacoes em cadeia.
- `Mapa`: fornece as celulas e os blocos encontrados no trajeto.
- `Bloco`: trata o impacto, decide se deve ser destruido e informa se interrompe a propagacao.
- `Jogador`: recebe o efeito da explosao ou utiliza seu escudo.
- `Inimigo`: e eliminado quando atingido.
- `Melhoria`: pode ser destruida quando atingida.

## CRC06 - Inimigo

### Responsabilidades

- Armazenar sua posicao atual na arena.
- Armazenar se esta ativo ou foi eliminado.
- Escolher automaticamente uma direcao de movimento.
- Solicitar ao mapa a validacao de seu movimento.
- Atualizar sua posicao em intervalos definidos.
- Causar a derrota quando entrar em contato com o jogador.
- Ser eliminado quando for atingido por uma explosao.

### Colaboracoes

- `Mapa`: valida os movimentos e informa os obstaculos da arena.
- `Jogador`: recebe a consequencia do contato com o inimigo.
- `Explosao`: elimina o inimigo quando o atinge.
- `Partida`: verifica se ainda existem inimigos ativos.

## CRC07 - Melhoria

### Responsabilidades

- Armazenar seu tipo de efeito.
- Armazenar sua posicao na arena.
- Armazenar se esta disponivel para coleta.
- Ser revelada quando o bloco que a esconde for destruido.
- Identificar quando foi coletada pelo jogador.
- Aplicar ao jogador o efeito correspondente ao seu tipo.
- Aumentar o alcance, aumentar o limite de bombas ou conceder um escudo.
- Ser removida quando for coletada ou atingida por uma explosao.

### Colaboracoes

- `Mapa`: armazena sua posicao na arena.
- `Bloco`: revela a melhoria que esconde quando e destruido.
- `Jogador`: recebe o efeito da melhoria coletada.
- `Explosao`: remove a melhoria quando ela e atingida.

## CRC08 - Bloco

### Responsabilidades

- Armazenar sua posicao no mapa.
- Conhecer seu tipo: destrutivel ou indestrutivel.
- Armazenar se esta intacto ou destruido.
- Informar se impede a passagem de personagens.
- Receber e tratar a interacao com uma explosao.
- Permanecer intacto quando for indestrutivel e ser destruido quando for destrutivel ao receber o impacto.
- Informar que o impacto em um bloco intacto interrompe a propagacao naquela direcao, mesmo quando o destroi.
- Notificar o mapa quando for destruido para liberar sua celula.
- Revelar a melhoria escondida, caso exista, ao ser destruido.

### Colaboracoes

- `Explosao`: informa o impacto e recebe a resposta sobre a interrupcao da propagacao.
- `Mapa`: mantem a posicao do bloco e libera sua celula quando recebe a notificacao de destruicao.
- `Melhoria`: e revelada quando o bloco que a esconde e destruido.

## CRC09 - Menu

### Responsabilidades

- Apresentar o menu inicial do jogo.
- Exibir as opcoes de iniciar partida, consultar comandos e sair.
- Armazenar a opcao atualmente selecionada.
- Interpretar os comandos de navegacao e confirmacao do menu.
- Solicitar o inicio de uma partida individual quando essa opcao for confirmada.
- Apresentar os comandos do jogador antes do inicio da partida.
- Permitir o retorno da tela de comandos ao menu inicial.
- Sinalizar o pedido de encerramento do jogo ao selecionar sair.

### Colaboracoes

- `Partida`: recebe a solicitacao para iniciar uma partida individual.
- `Jogador`: fornece a descricao dos comandos para exibicao, sem exigir uma partida em andamento.
