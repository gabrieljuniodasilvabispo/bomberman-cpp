/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Bomberman", "index.html", [
    [ "💣 Grupo Bomberman — PDS II", "index.html", "index" ],
    [ "Cartoes CRC - Bomberman", "md_design_2cartoes-crc.html", [
      [ "CRC01 - Partida", "md_design_2cartoes-crc.html#autotoc_md11", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md12", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md13", null ]
      ] ],
      [ "CRC02 - Mapa", "md_design_2cartoes-crc.html#autotoc_md14", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md15", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md16", null ]
      ] ],
      [ "CRC03 - Jogador", "md_design_2cartoes-crc.html#autotoc_md17", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md18", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md19", null ]
      ] ],
      [ "CRC04 - Bomba", "md_design_2cartoes-crc.html#autotoc_md20", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md21", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md22", null ]
      ] ],
      [ "CRC05 - Explosao", "md_design_2cartoes-crc.html#autotoc_md23", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md24", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md25", null ]
      ] ],
      [ "CRC06 - Inimigo", "md_design_2cartoes-crc.html#autotoc_md26", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md27", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md28", null ]
      ] ],
      [ "CRC07 - Melhoria", "md_design_2cartoes-crc.html#autotoc_md29", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md30", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md31", null ]
      ] ],
      [ "CRC08 - Bloco", "md_design_2cartoes-crc.html#autotoc_md32", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md33", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md34", null ]
      ] ],
      [ "CRC09 - Menu", "md_design_2cartoes-crc.html#autotoc_md35", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md36", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md37", null ]
      ] ],
      [ "CRC10 - AudioJogo", "md_design_2cartoes-crc.html#autotoc_md38", [
        [ "Responsabilidades", "md_design_2cartoes-crc.html#autotoc_md39", null ],
        [ "Colaboracoes", "md_design_2cartoes-crc.html#autotoc_md40", null ]
      ] ]
    ] ],
    [ "User Stories - Bomberman", "md_design_2user-stories.html", [
      [ "US01 - Iniciar uma partida individual", "md_design_2user-stories.html#autotoc_md42", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md43", null ]
      ] ],
      [ "US02 - Movimentar o personagem", "md_design_2user-stories.html#autotoc_md44", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md45", null ]
      ] ],
      [ "US03 - Posicionar bombas", "md_design_2user-stories.html#autotoc_md46", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md47", null ]
      ] ],
      [ "US04 - Interagir com explosoes", "md_design_2user-stories.html#autotoc_md48", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md49", null ]
      ] ],
      [ "US05 - Coletar melhorias", "md_design_2user-stories.html#autotoc_md50", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md51", null ]
      ] ],
      [ "US06 - Avancar pelas fases, concluir o jogo ou reiniciar", "md_design_2user-stories.html#autotoc_md52", [
        [ "Criterios de aceitacao", "md_design_2user-stories.html#autotoc_md53", null ]
      ] ],
      [ "Referencia", "md_design_2user-stories.html#autotoc_md54", null ]
    ] ],
    [ "Controlador da partida", "md_docs_2partida.html", [
      [ "Uso pela interface", "md_docs_2partida.html#autotoc_md56", null ],
      [ "Mapas fixos, avanço e reinício", "md_docs_2partida.html#autotoc_md57", null ],
      [ "Ordem das regras", "md_docs_2partida.html#autotoc_md58", null ]
    ] ],
    [ "Guia de testes do Bomberman", "md_docs_2testes-iniciais.html", [
      [ "Como executar os testes", "md_docs_2testes-iniciais.html#autotoc_md60", null ],
      [ "Como interpretar o resultado", "md_docs_2testes-iniciais.html#autotoc_md61", null ],
      [ "O que os testes automáticos verificam", "md_docs_2testes-iniciais.html#autotoc_md62", null ],
      [ "Verificação da interface", "md_docs_2testes-iniciais.html#autotoc_md63", null ],
      [ "Cobertura de código", "md_docs_2testes-iniciais.html#autotoc_md64", null ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Lista de Classes", "annotated.html", "annotated_dup" ],
      [ "Índice dos Componentes", "classes.html", null ],
      [ "Membros de classe", "functions.html", [
        [ "Todos", "functions.html", null ],
        [ "Funções", "functions_func.html", null ],
        [ "Variáveis", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Arquivos", "files.html", [
      [ "Lista de Arquivos", "files.html", "files_dup" ],
      [ "Membros dos Arquivos", "globals.html", [
        [ "Todos", "globals.html", null ],
        [ "Enumerações", "globals_enum.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_audio_jogo_8hpp_source.html"
];

const SYNCONMSG = 'clique para desativar a sincronização do painel';
const SYNCOFFMSG = 'clique para ativar a sincronização do painel';
const LISTOFALLMEMBERS = 'Lista de todos os Membros';