#include "apoio.hpp"
#include "Explosao.hpp"
#include "Bomba.hpp"
#include <algorithm>
struct TestesExplosao {
    Mapa mapa{5, 5, {}, {0, 0}, {}, {4, 4}};
    bool contem(const bomberman::Explosao& explosao, int coluna, int linha) {
        const auto& celulas = explosao.getCelulasAtingidas();
        return std::find(celulas.begin(), celulas.end(), sf::Vector2i(coluna, linha)) != celulas.end();
    }
};
TEST_CASE_FIXTURE(TestesExplosao, "Explosao propaga em cruz ate o alcance") {
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    CHECK(explosao.getCelulasAtingidas().size() == 5);
    for (auto p : {sf::Vector2i{2, 2}, {1, 2}, {3, 2}, {2, 1}, {2, 3}})
        CHECK(contem(explosao, p.x, p.y));
    CHECK_FALSE(contem(explosao, 1, 1));
}
TEST_CASE_FIXTURE(TestesExplosao, "Explosao respeita bordas da arena") {
    bomberman::Explosao explosao({0, 0}, 2, mapa);
    explosao.propagar();
    CHECK(explosao.getCelulasAtingidas().size() == 5);
    for (auto p : explosao.getCelulasAtingidas())
        CHECK(mapa.dentroDosLimites({p.y, p.x}));
}
TEST_CASE_FIXTURE(TestesExplosao, "Explosao para no primeiro bloco de cada direcao") {
    Mapa arena(5, 5, {Bloco({2, 3}, TipoBloco::Destrutivel),
                      Bloco({1, 2}, TipoBloco::Indestrutivel)}, {0, 0}, {}, {4, 4});
    bomberman::Explosao explosao({2, 2}, 2, arena);
    explosao.propagar();
    CHECK(arena.obterBloco({2, 3}) == nullptr);
    CHECK(arena.obterBloco({1, 2}) != nullptr);
    CHECK_FALSE(contem(explosao, 4, 2));
    CHECK_FALSE(contem(explosao, 2, 0));
}
TEST_CASE_FIXTURE(TestesExplosao, "Explosao elimina personagens atingidos e preserva os distantes") {
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    bomberman::Jogador perto({2, 3}), longe({0, 0});
    bomberman::Inimigo inimigo({1, 2});
    explosao.aplicarEm(perto);
    explosao.aplicarEm(longe);
    explosao.aplicarEm(inimigo);
    CHECK_FALSE(perto.ativo());
    CHECK(longe.ativo());
    CHECK_FALSE(inimigo.ativo());
}

TEST_CASE_FIXTURE(TestesExplosao, "Explosao converte coordenadas em mapa retangular") {
    Mapa arena(3, 7, {}, {0, 0}, {}, {2, 6});
    bomberman::Explosao explosao({4, 1}, 1, arena);
    explosao.propagar();
    REQUIRE(explosao.getCelulasAtingidas().size() == 5);
    for (auto p : {sf::Vector2i{4, 1}, {3, 1}, {5, 1}, {4, 0}, {4, 2}})
        CHECK(contem(explosao, p.x, p.y));
    bomberman::Jogador jogador({1, 5});
    explosao.aplicarEm(jogador);
    CHECK_FALSE(jogador.ativo());
}

TEST_CASE_FIXTURE(TestesExplosao, "Mesma explosao nao atravessa bloco destruido ao propagar novamente") {
    bomberman::Melhoria melhoria({3, 2}, bomberman::TipoMelhoria::Escudo);
    Mapa arena(5, 5, {Bloco({2, 3}, TipoBloco::Destrutivel, &melhoria)},
               {0, 0}, {}, {4, 4});
    bomberman::Explosao explosao({2, 2}, 2, arena);
    explosao.propagar();
    REQUIRE(arena.obterBloco({2, 3}) == nullptr);
    REQUIRE(arena.obterMelhoria({2, 3}) == &melhoria);
    const auto celulas = explosao.getCelulasAtingidas();
    explosao.propagar();
    CHECK(explosao.getCelulasAtingidas() == celulas);
    CHECK_FALSE(contem(explosao, 4, 2));
    CHECK(arena.obterMelhoria({2, 3}) == &melhoria);
}

TEST_CASE_FIXTURE(TestesExplosao, "Escudo protege durante todos os quadros da mesma explosao") {
    bomberman::Jogador jogador({2, 2});
    jogador.ativarEscudo();
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    explosao.aplicarEm(jogador);
    REQUIRE(jogador.ativo());
    CHECK_FALSE(jogador.temEscudo());
    explosao.atualizar(sf::milliseconds(100));
    explosao.aplicarEm(jogador);
    CHECK(jogador.ativo());
    bomberman::Explosao outra({2, 2}, 1, mapa);
    outra.propagar();
    outra.aplicarEm(jogador);
    CHECK_FALSE(jogador.ativo());
}

TEST_CASE_FIXTURE(TestesExplosao, "Explosao expirada nao causa dano nem reacao em cadeia") {
    bomberman::Jogador jogador({2, 2});
    bomberman::Inimigo inimigo({2, 3});
    bomberman::Bomba bomba({2, 2}, 1, jogador);
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    explosao.atualizar(sf::milliseconds(500));
    explosao.atualizar(sf::seconds(-1));
    CHECK_FALSE(explosao.ativa());
    explosao.aplicarEm(jogador);
    explosao.aplicarEm(inimigo);
    explosao.aplicarEm(bomba);
    CHECK(jogador.ativo());
    CHECK(inimigo.ativo());
    CHECK_FALSE(bomba.prontaParaExplodir());
}

TEST_CASE_FIXTURE(TestesExplosao, "Explosao antecipa somente bombas atingidas") {
    bomberman::Jogador jogador({0, 0});
    jogador.aumentarLimiteBombas();
    bomberman::Bomba perto({3, 2}, 1, jogador), longe({0, 0}, 1, jogador);
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    explosao.aplicarEm(perto);
    explosao.aplicarEm(longe);
    CHECK(perto.prontaParaExplodir());
    CHECK_FALSE(longe.prontaParaExplodir());
}

TEST_CASE_FIXTURE(TestesExplosao, "Explosao remove melhorias ja reveladas inclusive na origem") {
    bomberman::Melhoria centro({2, 2}, bomberman::TipoMelhoria::Escudo);
    bomberman::Melhoria perto({3, 2}, bomberman::TipoMelhoria::BombaExtra);
    bomberman::Melhoria longe({0, 0}, bomberman::TipoMelhoria::AumentoAlcance);
    mapa.registrarMelhoria({2, 2}, centro);
    mapa.registrarMelhoria({2, 3}, perto);
    mapa.registrarMelhoria({0, 0}, longe);
    bomberman::Explosao explosao({2, 2}, 1, mapa);
    explosao.propagar();
    CHECK(mapa.obterMelhoria({2, 2}) == nullptr);
    CHECK(mapa.obterMelhoria({2, 3}) == nullptr);
    CHECK(mapa.obterMelhoria({0, 0}) == &longe);
}
