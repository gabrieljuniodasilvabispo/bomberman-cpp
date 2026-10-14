#include <doctest/doctest.h>
#include "Partida.hpp"
struct TestesPartida { bomberman::Partida partida; };
TEST_CASE_FIXTURE(TestesPartida, "Partida inicia fase com inimigos a enfrentar") {
    partida.iniciarFase();
    CHECK_FALSE(partida.todosInimigosEliminados());
}
TEST_CASE_FIXTURE(TestesPartida, "Partida reinicia fase com inimigos novamente") {
    partida.iniciarFase();
    partida.encerrarFase();
    partida.reiniciarFase();
    CHECK_FALSE(partida.todosInimigosEliminados());
}
