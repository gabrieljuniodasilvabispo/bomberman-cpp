#include <doctest/doctest.h>
#include "Menu.hpp"
struct TestesMenu { bomberman::Menu menu; };
TEST_CASE_FIXTURE(TestesMenu, "Menu inicia ativo e com selecao valida") {
    CHECK(menu.ativo());
    REQUIRE(menu.getOpcoes().size() == 3);
    CHECK(menu.getIndiceSelecionado() >= 0);
    CHECK(menu.getIndiceSelecionado() < 3);
}
TEST_CASE_FIXTURE(TestesMenu, "Menu permite selecionar novo jogo instrucoes e sair") {
    // Contrato inicial proposto: ordem NovoJogo, Instrucoes, Sair; inicio em zero.
    CHECK(menu.confirmarSelecao() == bomberman::OpcaoMenu::NovoJogo);
    bomberman::Menu instrucoes;
    instrucoes.navegarProxima();
    CHECK(instrucoes.confirmarSelecao() == bomberman::OpcaoMenu::Instrucoes);
    bomberman::Menu saida;
    saida.navegarProxima();
    saida.navegarProxima();
    CHECK(saida.confirmarSelecao() == bomberman::OpcaoMenu::Sair);
}
TEST_CASE_FIXTURE(TestesMenu, "Menu mantem indice valido nas bordas") {
    for (int i = 0; i < 10; ++i) {
        menu.navegarAnterior();
        CHECK(menu.getIndiceSelecionado() >= 0);
        CHECK(menu.getIndiceSelecionado() < 3);
    }
    for (int i = 0; i < 10; ++i) {
        menu.navegarProxima();
        CHECK(menu.getIndiceSelecionado() >= 0);
        CHECK(menu.getIndiceSelecionado() < 3);
    }
}
