# Testes com doctest e verificações de entrada

Os testes exercitam as implementações das classes descritas nos cartões CRC,
Posicao e programação defensiva. Nesta etapa, a suíte deve passar; falhas não
são consideradas esperadas. O CTest também verifica ajuda e argumentos inválidos
do executável, sem abrir janela. Os testes de Partida integram as regras da fase.
Jogo e testes utilizam a mesma biblioteca de regras, `BombermanLogica`.
A interação com teclado e o desenho da janela exigem também verificação visual.

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
Os arquivos antigos de `tests/tdd` são auxiliares históricos e não participam
da compilação. Todas as classes da biblioteca possuem implementação própria;
uma fonte ausente causa erro na configuração, sem substituição por auxiliares.

O CTest apresenta a suíte doctest como um teste agregado e os quatro cenários
de terminal separadamente. Para ver os casos individuais, execute
`build/BombermanTests.exe` no Windows ou `build/BombermanTests` no Linux.

Os casos de programação defensiva verificam tanto a exceção quanto o estado
preservado depois da rejeição.

## Verificação da interface

Execute `mingw32-make run` (Windows), `make run` (Linux) ou `iniciar.bat`.

- Consulte as instruções e inicie uma fase pelo menu.
- Use setas/WASD para andar, verificando limites, blocos e bombas.
- Coloque uma bomba com Espaço, saia de seu alcance e confira a detonação.
- Confira a revelação/coleta dos itens A/B/E e a atualização dos atributos.
- Verifique a derrota por fogo ou contato e a vitória após eliminar os inimigos
  e alcançar a saída verde.
- Reinicie com R ou Enter no resultado e confira terreno, personagens e atributos.
- Use Esc para voltar ao menu e inicie outra fase.
- Alterne para outra janela e confira que a fase pausa e retoma sem avançar
  os temporizadores durante a ausência.
- Confira os sons de passos, bomba colocada, explosao, coleta e derrota.
  Movimento bloqueado e bomba recusada nao devem tocar efeitos. Em uma cadeia,
  cada bomba gera uma detonacao; permanecer na tela de derrota nao repete o som.
- Ao perder o foco, confira o aviso de pausa; R e Esc devem parar os sons antigos.

Os testes de `Partida` tambem verificam a fila de eventos consumida pelo audio,
incluindo consumo unico, acoes recusadas, cadeia, coleta, derrota e limpeza no
reinicio. Eles nao reproduzem som nem dependem de uma saida de audio.

Para instrumentar o código próprio com GCC/gcov, configure
`-DBOMBERMAN_COVERAGE=ON` ou execute `mingw32-make test COVERAGE=ON`
(Linux: `make test COVERAGE=ON`). A instrumentação inclui a biblioteca e a
entrada gráfica; dependências externas e testes não são instrumentados.
Gerar o relatório com gcovr e verificar a meta de 60% continuam sendo etapas
separadas; executar testes com instrumentação não comprova essa porcentagem.

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
| Partida | Criação real da fase, movimento, bombas, cadeia, dano, coleta, vitória/derrota e restauração completa no reinício |
| Posicao | Valores de linha e coluna e cópia que pode ser alterada sem modificar o original |
| Programação defensiva | Entradas inválidas, limites de recursos, estado preservado e valores extremos |
| Terminal | Ajuda, opção desconhecida e excesso de argumentos; valida código de saída e mensagem |
