# Testes iniciais com doctest

Nesta etapa, foram criados testes iniciais com doctest para as principais classes
descritas nos cartões CRC e para Posicao. Os testes descrevem o comportamento
esperado de cada classe. Como as regras do jogo ainda não foram implementadas,
é esperado que os testes dessas regras falhem.

## Como executar os testes

No PowerShell, na pasta do projeto:

```powershell
mingw32-make test
```

Também é possível abrir `testar.bat` no Windows.

Para compilar os testes sem executá-los:

```powershell
mingw32-make test-build
```

Para ver os nomes dos testes disponíveis:

```powershell
mingw32-make list-tests
```

No Linux, use `make` no lugar de `mingw32-make`.

Para compilar, são necessários um compilador compatível com C++17, CMake 3.22
ou superior, GNU Make, Ninja e as bibliotecas necessárias para a SFML 3.0.2.
A SFML é baixada na primeira configuração. O doctest 2.4.12 e sua licença já
estão incluídos na pasta `third_party/doctest`.

O projeto usa Ninja como padrão para organizar a compilação. No Linux, é
possível usar outra opção com `GENERATOR="Unix Makefiles"`. Nesse caso, informe
uma nova pasta de compilação com `BUILD_DIR`, para não misturar as configurações.

## Por que os testes falham

As classes ainda não possuem seus arquivos de implementação na pasta `src`.
Por isso, foram criados arquivos temporários em `tests/tdd` para permitir a
compilação e a execução dos testes. Ao usar um desses arquivos, o teste recebe
um erro do tipo `std::runtime_error` com a mensagem `Nao implementado`.

Esses arquivos são usados apenas nos testes e não contêm regras do jogo.
Como os construtores também informam esse erro, muitos testes falham logo ao
criar os objetos que seriam usados nas verificações.

Quando arquivos como `src/Mapa.cpp` e `src/Bloco.cpp` forem criados, a configuração
da compilação passará a usá-los no lugar dos arquivos temporários. O comando
`mingw32-make test` refaz essa configuração antes de compilar. Se algum método
usado pelos testes ainda estiver sem definição, a compilação indicará o problema.

As falhas aparecem normalmente no resultado. Mesmo sendo esperadas nesta etapa,
elas não são contadas como testes aprovados.

O teste de `Posicao` já pode passar, pois essa estrutura já possui os campos
de linha e coluna. Não foi necessário implementar nada para aprovar esse teste.

## O que os testes verificam

| Classe | Verificações iniciais |
| --- | --- |
| Mapa | Configuração inicial, limites, obstáculos, bombas, melhorias, reinício e dados inválidos |
| Bloco | Destruição dos blocos destrutíveis, resistência dos indestrutíveis e revelação de melhoria uma única vez |
| Jogador | Estado inicial, movimento nas quatro direções, bloqueio por obstáculos e derrota por explosão |
| Inimigo | Estado inicial, eliminação, atualização sem passagem de tempo e contato com o jogador |
| Bomba | Posição, alcance, atualização sem passagem de tempo e antecipação da detonação |
| Explosao | Propagação em cruz, alcance, limites do mapa, interrupção por blocos e efeito nos personagens |
| Melhoria | Aumento de alcance, bomba extra, aplicação do benefício uma única vez e proteção do escudo |
| Menu | Opções disponíveis, confirmação da escolha e seleção dentro dos limites da lista |
| Partida | Início e reinício de uma fase com inimigos |
| Posicao | Valores de linha e coluna e cópia que pode ser alterada sem modificar o original |
