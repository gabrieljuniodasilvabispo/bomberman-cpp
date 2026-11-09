# 💣 Grupo Bomberman — PDS II

## 👥 Integrantes

* Gabriel Júnio da Silva Bispo
* Eduardo Amaral
* João Paulo
* Gabriel Augusto
* Raphael Bastos

---

## 🎯 Objetivos, Contexto e Motivação

O objetivo deste projeto é **replicar, em alguma medida, o conhecido jogo da Konami, originalmente criado pela Hudson Soft: *Bomberman***.

### 🕹️ Contexto

*Bomberman* foi um jogo lançado em **julho de 1983**. Nele, o protagonista, um robô, precisava posicionar bombas para abrir caminho por um labirinto, destruir inimigos e encontrar a saída.

Ao final do jogo, o protagonista se transformava em um humano, conectando-se à história do clássico ***Lode Runner***.

Com o decorrer da sequência de jogos lançados sob a mesma franquia, a série passou gradualmente a adotar uma abordagem mais voltada para **partidas PvP (Player versus Player)**.

### 💡 Motivação

A escolha deste projeto foi motivada, ao mesmo tempo, pelo **apreço pelo jogo original de *Bomberman*** e pelos **desafios que a implementação de um jogo desse nível pode proporcionar**.

### ⚙️ Implementação

Com relação à implementação, o jogo original foi desenvolvido em **BASIC**, enquanto versões mais modernas da franquia utilizam tecnologias como **C#**.

A adaptação de um projeto desse porte para **C++** apresenta um grande desafio técnico, envolvendo aspectos como programação orientada a objetos, gerenciamento de recursos, lógica de jogo, gráficos e interação com o usuário.

Dessa forma, o desenvolvimento do projeto busca não apenas reproduzir a experiência de *Bomberman*, mas também proporcionar ao grupo uma oportunidade de aplicar e aprofundar conhecimentos adquiridos ao longo da disciplina **PDS II**.

## Modelagem e escopo

- **Histórias de usuário**: requisitos e critérios de aceitação.
- **Cartões CRC**: responsabilidades e colaborações.
- Mapa.hpp: limites, terreno, posições iniciais, saída, ocupação por bombas, registro de melhorias e reinício.
- Bloco.hpp: propriedades dos obstáculos e tratamento de impactos.
- Posicao.hpp: coordenadas lógicas de linha e coluna.

## Testes e programação defensiva (C9)

Nesta etapa, foram acrescentadas validações de posições, dimensões, tipos de
objetos e limites numéricos. As classes sinalizam violações de seus contratos
por meio de exceções, enquanto ações normais que não podem ser realizadas,
como tentar atravessar uma parede, retornam uma indicação de falha.

O programa também trata argumentos inválidos e exceções na entrada da aplicação,
apresentando mensagens no terminal. Os contratos e as exceções previstas estão
documentados nos cabeçalhos das classes.

Os testes com **doctest** verificam as regras implementadas, a rejeição de dados
inválidos e a preservação do estado após operações recusadas. Há também testes
para os comandos de ajuda e o tratamento de argumentos inválidos do executável.

No Windows, execute `mingw32-make test` na pasta do projeto ou abra `testar.bat`.
Use `mingw32-make test-build` para apenas compilar. No Linux, use `make test`.

Os comandos e os cenários verificados estão descritos no
[guia de testes](docs/testes-iniciais.md). A cobertura será medida com **gcovr**,
com meta mínima de **60%** para a entrega final.

A classe `Partida` já coordena uma arena fixa, movimento, bombas, explosões em
cadeia, coleta, vitória, derrota e reinício completo. Seu uso e a ordem das
atualizações estão descritos no [guia da partida](docs/partida.md).

## Como jogar

No Windows, abra `iniciar.bat` ou execute `mingw32-make run`. No Linux, use
`make run`. O comando `make` (ou `mingw32-make`) compila o jogo; os testes
continuam disponíveis pelo alvo `test`.

Escolha **Novo Jogo** no menu para abrir a arena. A janela desenha o estado de
`Partida`: terreno, personagens, bombas, explosões, melhorias e saída. O painel
mostra inimigos restantes, bombas ativas e limite, alcance e escudo. Para vencer,
elimine os inimigos e alcance a saída, que fica verde quando liberada.

| Comando | Ação |
| --- | --- |
| Setas ou W/A/S/D | Mover uma célula; manter pressionado repete o movimento |
| Espaço | Colocar uma bomba |
| R | Reiniciar a fase |
| Enter na tela de resultado | Reiniciar após vitória ou derrota |
| Esc | Encerrar a tentativa e voltar ao menu |

Ao alternar para outra janela, a partida fica pausada. As melhorias reveladas
são identificadas por **A** (alcance), **B** (bomba extra) e **E** (escudo).
As instruções completas também estão no menu.

A interface utiliza formas geométricas e a fonte incluída em
`assets/fonts/tuffy.ttf`. O CMake informa o caminho da fonte ao executável;
se ela estiver ausente, o programa apresenta um erro no terminal. Não são
necessários arquivos externos de mapa ou imagens. Após compilar, use
`build/Bomberman.exe --ajuda` no Windows ou `build/Bomberman --ajuda` no Linux
para consultar os comandos no terminal.

## Sons

Os efeitos de `assets/sounds` sao reproduzidos pelo modulo **SFML Audio**:

| Evento | Arquivo |
| --- | --- |
| Passo horizontal / vertical aceito | `step_horizontal.wav` / `step_vertical.wav` |
| Bomba colocada | `bomb_lay.wav` |
| Detonacao, inclusive em cadeia | `bomb_explosion.wav` |
| Melhoria coletada | `powerup.wav` |
| Derrota | `bomberman_killed.wav` e `bomberman-death-sound.wav`, alternados entre tentativas |
| Janela do jogo perde o foco | `pause.wav` |

Movimentos bloqueados e tentativas de colocar bombas sem vaga nao produzem som.
Os efeitos pausam ao perder o foco e sao interrompidos ao reiniciar ou voltar
ao menu. Passos possuem volume menor que os outros efeitos. Se um WAV estiver
ausente, o jogo avisa no terminal e continua com os demais sons.

Na primeira compilacao com audio, o CMake baixa as dependencias internas do
modulo da SFML (Ogg, Vorbis e FLAC). Os testes de regras continuam independentes
do dispositivo de audio.
