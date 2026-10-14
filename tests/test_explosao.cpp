#include "apoio.hpp"
#include "Explosao.hpp"
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
