# Testes com doctest e verificações de entrada

Os testes exercitam as implementações das classes descritas nos cartões CRC,
Posicao e programação defensiva. Nesta etapa, a suíte deve passar; falhas não
são consideradas esperadas. O CTest também verifica ajuda e argumentos inválidos
do executável, sem abrir janela. Isso não equivale a uma partida integrada.

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

## Diagnóstico e etapa C9

Erros de compilação ou asserções precisam ser corrigidos antes da entrega.
Os arquivos antigos de `tests/tdd` são auxiliares históricos; todas as classes
listadas no CMake já possuem implementação própria. Para impedir o uso acidental
de auxiliares, configure com `-DBOMBERMAN_TDD_STUBS=OFF`.

O CTest apresenta a suíte doctest como um teste agregado e os quatro cenários
de terminal separadamente. Para ver os casos individuais, execute
`build/BombermanTests.exe` no Windows ou `build/BombermanTests` no Linux.

Os casos de programação defensiva verificam tanto a exceção quanto o estado
preservado depois da rejeição.

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
| Partida | Contrato inicial de início/reinício; ainda não comprova criação de inimigos nem o fluxo completo |
| Posicao | Valores de linha e coluna e cópia que pode ser alterada sem modificar o original |
| Programação defensiva | Entradas inválidas, limites de recursos, estado preservado e valores extremos |
| Terminal | Ajuda, opção desconhecida e excesso de argumentos; valida código de saída e mensagem |
