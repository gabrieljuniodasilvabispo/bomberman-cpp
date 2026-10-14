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

## Testes iniciais (TDD)

Os testes usam doctest e estao na fase **RED**: os comportamentos do jogo ainda
nao foram implementados e as falhas sao esperadas.

No Windows, execute `mingw32-make test` na pasta do projeto ou abra `testar.bat`.
Use `mingw32-make test-build` para apenas compilar. No Linux, use `make test`.

Veja [o guia de testes](docs/testes-iniciais.md) para saber como executar os
testes, por que eles falham e o que verificam. A meta final de 60% de
cobertura com gcov ainda nao se aplica aos auxiliares desta fase inicial.
