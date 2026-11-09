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

A classe `Partida` coordena quatro arenas fixas, movimento, bombas, explosões em
cadeia, coleta, avanço de fase, derrota e reinício completo. Seu uso e a ordem das
atualizações estão descritos no [guia da partida](docs/partida.md).

## Como jogar

O projeto inclui um `Makefile` na raiz. No Linux, execute nessa pasta:

```sh
make
make run
```

`make` configura e compila o jogo completo, incluindo o áudio. `make run`
compila o que estiver pendente e abre o jogo. O Makefile utiliza a configuração
CMake do projeto e o gerador Ninja por padrão.

No PowerShell/Windows com MinGW, o programa Make se chama `mingw32-make`:

```powershell
mingw32-make
mingw32-make run
```

Também é possível abrir `iniciar.bat`. São necessários compilador C++17, CMake,
GNU Make e Ninja disponíveis no terminal, além dos requisitos da SFML indicados
no [guia de testes](docs/testes-iniciais.md). Na primeira configuração, é preciso
acesso à internet para baixar as dependências.

Os alvos `test`, `test-build` e `list-tests` continuam disponíveis. Use
`make run ARGS=--ajuda` para consultar os comandos sem abrir janela, ou
`mingw32-make run ARGS=--ajuda` no Windows. A quantidade de tarefas de compilação
pode ser ajustada, por exemplo com `make JOBS=2`.

Escolha **Novo Jogo** no menu para começar o primeiro dos quatro mapas fixos.
A janela desenha o estado de
`Partida`: terreno, personagens, bombas, explosões, melhorias e saída. O painel
mostra o número da fase, inimigos restantes, bombas ativas e limite, alcance e
escudo. Elimine os inimigos e entre na saída verde para iniciar automaticamente
o próximo mapa, sempre na ordem 1 → 2 → 3 → 4. A primeira fase mantém o mapa
original; as outras três têm disposições próprias de blocos, melhorias, inimigos
e porta. Ao sair do quarto mapa, o jogo mostra a vitória. Cada fase começa com
os atributos básicos do jogador.

| Comando | Ação |
| --- | --- |
| Setas ou W/A/S/D | Mover uma célula; manter pressionado repete o movimento |
| Espaço | Colocar uma bomba |
| R | Repetir o mesmo mapa e número da fase, restaurando todos os elementos |
| Enter após derrota | Tentar novamente o mesmo mapa |
| Enter ou R após vencer os quatro mapas | Começar um novo jogo na fase 1 |
| Esc | Encerrar a tentativa e voltar ao menu |

Ao alternar para outra janela, a partida fica pausada. As melhorias reveladas
são identificadas por **A** (alcance), **B** (bomba extra) e **E** (escudo).
As instruções completas também estão no menu.
Voltar ao menu e escolher **Novo Jogo** também retorna ao primeiro mapa.

A interface utiliza formas geométricas e a fonte incluída em
`assets/fonts/tuffy.ttf`. O CMake informa o caminho da fonte ao executável;
se ela estiver ausente, o programa apresenta um erro no terminal. Não são
necessários arquivos externos de mapa. O fundo do menu usa a imagem incluída em
`assets/img/fundo.png`. Após compilar, use
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
