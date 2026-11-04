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

O executável gráfico ainda apresenta o protótipo de movimento; a integração da
partida está em desenvolvimento. Após compilar, use `build/Bomberman.exe --ajuda`
no Windows ou `build/Bomberman --ajuda` no Linux para consultar as instruções.
O protótipo atual não exige arquivos externos de mapa, imagem, fonte ou configuração
para iniciar.
