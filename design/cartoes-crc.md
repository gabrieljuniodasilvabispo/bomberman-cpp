# Cartoes CRC - Bomberman

Este documento apresenta os cartoes Classe-Responsabilidade-Colaboracao das principais classes do jogo Bomberman, conforme a implementacao atual de uma partida individual.

A interface grafica é implementada por funcoes em `src/main.cpp`: le o teclado, controla as telas, desenha o estado da fase e encaminha eventos ao audio. Ela nao e uma classe do projeto. As colaboracoes mediadas por essa interface ou por outra classe estao identificadas abaixo; nao significam uma chamada direta entre as duas classes.

## CRC01 - Partida

### Responsabilidades

- Controlar o estado atual da partida.
- Guardar a configuracao da fase e fornecer uma arena padrao com uma rota inicial de fuga.
- Iniciar a fase criando mapa, jogador, inimigos e melhorias nas posicoes configuradas.
- Atualizar os elementos ativos e seus temporizadores a cada ciclo do jogo.
- Coordenar a colocacao de bombas, verificando posicao, ocupacao e vagas disponiveis.
- Processar explosoes e resolver todas as detonacoes de uma reacao em cadeia no mesmo ciclo.
- Resolver contato com inimigos e coleta de melhorias na posicao do jogador.
- Verificar inimigos restantes, chegada a saida e derrota para determinar o resultado da fase.
- Encerrar a tentativa ou reiniciar recriando os elementos em seus estados iniciais.
- Registrar eventos de passos, bombas, coleta e derrota para consumo unico pela interface.

### Colaboracoes

- `Mapa`: valida a configuracao e fornece terreno, ocupacoes, posicoes iniciais, melhorias reveladas e saida.
- `Jogador`: recebe deslocamentos e fornece posicao, estado, alcance e capacidade de bombas.
- `Inimigo`: recebe atualizacoes, verifica contato e informa se continua ativo.
- `Bomba`: e criada com os dados do jogador, recebe atualizacoes e produz uma explosao ao detonar.
- `Explosao`: recebe atualizacoes e aplica efeitos em personagens e bombas.
- `Melhoria`: e criada para a fase e aplica seu efeito quando coletada; sua vida util pertence a Partida.
- `Menu` (via interface): fornece a escolha de novo jogo, que a interface transforma em inicio de fase.
- `AudioJogo` (via interface): recebe os eventos consumidos da partida para reproduzir os efeitos correspondentes.

## CRC02 - Mapa

### Responsabilidades

- Armazenar a organizacao das celulas da arena e os blocos destrutiveis e indestrutiveis.
- Informar se uma posicao permite entrada, considerando terreno e ocupacao por bomba.
- Validar os limites da arena para consultas e tentativas de movimento.
- Liberar a celula de um bloco depois que ele informar sua destruicao.
- Armazenar e validar as posicoes iniciais recebidas, rejeitando bloqueios, limites invalidos e sobreposicoes entre personagens.
- Armazenar e validar a posicao da saida da fase.
- Fornecer os blocos presentes nas celulas consultadas por uma explosao.
- Registrar e remover a ocupacao por bombas sem controlar seus temporizadores.
- Registrar, consultar e remover melhorias reveladas sem assumir a propriedade desses objetos.
- Restaurar o terreno inicial e limpar os registros de bombas e melhorias no reinicio do mapa.

### Colaboracoes

- `Partida`: fornece a configuracao da fase, define a disposicao inicial segura e coordena o reinicio de todos os objetos.
- `Jogador`: consulta limites e ocupacao para validar seu movimento.
- `Inimigo`: consulta limites e ocupacao antes de mudar de celula.
- `Explosao`: consulta os blocos encontrados e remove registros de melhorias expostas atingidas.
- `Bloco`: notifica sua destruicao e registra a melhoria que escondia.
- `Bomba`: remove sua ocupacao do mapa ao detonar; o registro inicial e feito por Partida.
- `Melhoria`: fornece posicao e estado de coleta para validar seu registro na arena.

O mapa valida as posicoes recebidas. A escolha de uma disposicao com rota de fuga pertence a configuracao da fase em Partida; Mapa nao calcula uma rota segura para qualquer configuracao.

## CRC03 - Jogador

### Responsabilidades

- Armazenar sua posicao atual na arena.
- Armazenar se esta ativo ou foi derrotado.
- Receber deslocamentos logicos e aceitar apenas passos unitarios nas quatro direcoes.
- Consultar o mapa antes de efetivar um movimento.
- Controlar o limite de bombas simultaneas e reservar ou devolver vagas de bombas ativas.
- Armazenar e fornecer o alcance atual de suas bombas.
- Aplicar aumentos de alcance e de limite de bombas solicitados por uma melhoria.
- Armazenar o escudo e consumi-lo ao receber uma explosao; sem escudo, registrar a derrota.
- Registrar a derrota por contato com inimigo, inclusive quando possui escudo.

### Colaboracoes

- `Mapa`: informa limites e ocupacao do destino de um movimento.
- `Partida`: encaminha deslocamentos, consulta os atributos do jogador e coordena a colocacao das bombas e a coleta.
- `Bomba`: reserva uma vaga no jogador ao ser criada e devolve essa vaga ao detonar ou ser destruida antes da detonacao.
- `Melhoria`: chama os metodos que aumentam atributos ou concedem escudo.
- `Explosao`: aplica dano e controla que a mesma explosao nao atinja o jogador repetidamente.
- `Inimigo`: informa quando ocorreu contato entre os personagens.

A traducao de teclas em deslocamentos ocorre na interface. O comando de colocar bomba e encaminhado a Partida; Jogador fornece os atributos e controla as vagas.

## CRC04 - Bomba

### Responsabilidades

- Armazenar sua posicao na arena.
- Armazenar o alcance recebido e a referencia ao jogador responsavel.
- Reservar uma vaga de bomba ativa ao ser criada.
- Controlar o tempo restante ate a detonacao e informar quando esta pronta.
- Produzir uma explosao quando Partida solicita a detonacao de uma bomba pronta.
- Antecipar a detonacao ao ser atingida por outra explosao.
- Remover sua ocupacao no mapa e devolver uma unica vaga ao detonar.
- Devolver sua reserva ao ser destruida antes da detonacao.

### Colaboracoes

- `Jogador`: fornece alcance e responsabilidade por meio de Partida e controla a reserva de vagas.
- `Mapa`: tem sua ocupacao removida na detonacao; a validacao e o registro da colocacao sao realizados por Partida.
- `Explosao`: e criada e propagada na detonacao e pode antecipar o temporizador de outra bomba.
- `Partida`: valida a colocacao, cria a bomba, atualiza o temporizador e solicita a detonacao.

## CRC05 - Explosao

### Responsabilidades

- Armazenar a posicao de origem e o alcance maximo da explosao.
- Calcular uma unica vez as celulas atingidas nas quatro direcoes.
- Notificar o bloco encontrado de que foi atingido, delegando a ele o tratamento do impacto.
- Interromper a propagacao conforme a resposta do bloco e os limites do mapa.
- Aplicar dano ao jogador ou consumir seu escudo, sem repetir o efeito da mesma explosao nesse jogador.
- Eliminar inimigos atingidos.
- Antecipar a detonacao das bombas atingidas para que Partida resolva a cadeia no mesmo ciclo.
- Remover do mapa melhorias ja expostas atingidas, preservando a revelada pelo bloco destruido nesse impacto.
- Permanecer ativa somente durante o tempo definido e deixar de aplicar efeitos depois de expirar.

### Colaboracoes

- `Bomba`: fornece origem e alcance e recebe a antecipacao da detonacao quando atingida.
- `Mapa`: fornece as celulas e os blocos encontrados e remove os registros de melhorias atingidas.
- `Bloco`: trata o impacto, decide se deve ser destruido e informa se interrompe a propagacao.
- `Jogador`: recebe o efeito da explosao ou utiliza seu escudo.
- `Inimigo`: e eliminado quando atingido.
- `Melhoria` (via mapa): deixa de estar disponivel na arena quando seu registro e removido pela explosao.
- `Partida`: atualiza a duracao, aplica os efeitos, resolve reacoes em cadeia e remove explosoes expiradas.

## CRC06 - Inimigo

### Responsabilidades

- Armazenar sua posicao atual na arena.
- Armazenar se esta ativo ou foi eliminado.
- Escolher automaticamente uma direcao de movimento.
- Solicitar ao mapa a validacao de seu movimento.
- Acumular tempo e atualizar sua posicao em intervalos definidos.
- Causar a derrota quando entrar em contato com o jogador.
- Ser eliminado quando for atingido por uma explosao.

### Colaboracoes

- `Mapa`: valida os movimentos e informa os obstaculos da arena.
- `Jogador`: recebe a consequencia do contato com o inimigo.
- `Explosao`: elimina o inimigo quando o atinge.
- `Partida`: atualiza os inimigos, solicita a verificacao de contato e consulta quais continuam ativos.

## CRC07 - Melhoria

### Responsabilidades

- Armazenar seu tipo de efeito.
- Armazenar sua posicao na arena.
- Armazenar e informar se seu beneficio ja foi coletado.
- Ignorar uma nova aplicacao quando ja foi coletada ou quando o jogador esta derrotado.
- Aplicar o efeito correspondente: aumentar alcance, aumentar limite de bombas ou conceder escudo.
- Marcar a coleta somente depois de aplicar o efeito com sucesso.
- Rejeitar entradas invalidas e sinalizar efeitos ainda nao suportados, preservando seu estado de coleta em caso de falha.

### Colaboracoes

- `Mapa`: mantem o registro que torna a melhoria revelada disponivel na arena e remove esse registro na coleta ou destruicao.
- `Bloco`: mantem a referencia da melhoria escondida e a revela ao registrar no mapa apos ser destruido.
- `Jogador`: recebe o efeito por meio dos metodos que alteram seus atributos.
- `Explosao` (via mapa): remove o registro de uma melhoria exposta atingida.
- `Partida`: possui os objetos, identifica a coleta por posicao, solicita o efeito, remove o registro coletado e recria as melhorias no reinicio.

O indicador de coleta nao representa sozinho a disponibilidade: uma melhoria escondida ou removida por explosao tambem nao pode ser coletada. Essa disponibilidade e determinada pelo registro no mapa. Aumento de velocidade e uma extensao ainda nao suportada, fora dos tres efeitos previstos para a fase.

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
- `Mapa`: armazena o bloco, libera sua celula e recebe a melhoria revelada.
- `Melhoria`: fornece posicao e estado de coleta e e revelada pelo registro no mapa.

## CRC09 - Menu

### Responsabilidades

- Fornecer as opcoes de novo jogo, instrucoes e saida para a interface.
- Armazenar a opcao atualmente selecionada.
- Avancar a selecao de forma circular quando recebe o comando de proxima opcao.
- Retroceder a selecao de forma circular quando recebe o comando de opcao anterior.
- Retornar a opcao confirmada para que a interface execute a acao correspondente.
- Fornecer as secoes de instrucoes sobre objetivo, controles, bombas, melhorias e resultado.

### Colaboracoes

- Interface em `main.cpp`: traduz teclas em navegacao, desenha opcoes e instrucoes, trata o retorno com Esc e fecha a janela quando Menu retorna a opcao de sair.
- `Partida` (via interface): recebe o inicio de uma fase quando a opcao de novo jogo e confirmada no Menu.

Menu fornece textos de instrucoes definidos na propria classe. Ele nao consulta Jogador para obter comandos nem controla diretamente a janela ou a troca de telas.

## CRC10 - AudioJogo

### Responsabilidades

- Carregar uma vez os oito efeitos WAV disponiveis em `assets/sounds`.
- Associar eventos da partida aos sons de passos, colocacao e explosao de bombas, coleta e derrota.
- Manter os buffers vivos enquanto existirem canais que os utilizem.
- Limitar a quantidade de efeitos simultaneos e manter passos em um canal separado, com volume menor.
- Alternar as duas variantes de derrota entre tentativas, usando a disponivel quando uma delas estiver ausente.
- Pausar efeitos em andamento e reproduzir o aviso de pausa uma unica vez por pausa.
- Retomar somente os efeitos pausados e interromper todos os sons ao receber o comando de parar.
- Avisar sobre arquivos que nao puderam ser carregados e continuar com os efeitos disponiveis.

### Colaboracoes

- `Partida` (via interface): produz os valores de `EventoPartida` consumidos e encaminhados para reproducao.
- Interface em `main.cpp`: entrega os eventos e solicita pausa, retomada ou parada conforme foco, reinicio e retorno ao menu.
- `sf::SoundBuffer` (SFML Audio): carrega e conserva as amostras dos arquivos de som.
- `sf::Sound` (SFML Audio): reproduz, pausa, retoma e interrompe os efeitos nos canais mantidos por AudioJogo.
