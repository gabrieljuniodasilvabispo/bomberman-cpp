#include "apoio.hpp"
#include "Explosao.hpp"
#include <cstdint>
#include <limits>
#include <stdexcept>

TEST_CASE("Defensiva: mapa rejeita dimensoes extremas antes de alocar") {
    const int extremo = std::numeric_limits<int>::max();
    CHECK_THROWS_AS((Mapa(extremo, extremo, {}, {0, 0}, {}, {0, 0})), std::length_error);
    CHECK_THROWS_AS((Mapa(1, Mapa::MAXIMO_CELULAS + 1, {}, {0, 0}, {}, {0, 0})),
                    std::length_error);
    CHECK_THROWS_AS((Mapa(1, 1, {}, {0, 0}, {{0, 0}}, {0, 0})), std::invalid_argument);
}

TEST_CASE_FIXTURE(TestesMapa, "Defensiva: registro invalido de melhoria preserva o mapa") {
    bomberman::Melhoria item({1, 0}, bomberman::TipoMelhoria::Escudo);
    CHECK_THROWS_AS(mapa.registrarMelhoria({-1, 0}, item), std::invalid_argument);
    CHECK_THROWS_AS(mapa.registrarMelhoria({0, 2}, item), std::invalid_argument);
    CHECK(mapa.obterMelhoria({0, 2}) == nullptr);
    bomberman::Melhoria bloqueada({1, 1}, bomberman::TipoMelhoria::Escudo);
    CHECK_THROWS_AS(mapa.registrarMelhoria({1, 1}, bloqueada), std::invalid_argument);
    CHECK_FALSE(mapa.podeEntrar({1, 1}));

    mapa.registrarMelhoria({0, 1}, item);
    CHECK_NOTHROW(mapa.registrarMelhoria({0, 1}, item));
    bomberman::Melhoria outra({1, 0}, bomberman::TipoMelhoria::BombaExtra);
    CHECK_THROWS_AS(mapa.registrarMelhoria({0, 1}, outra), std::logic_error);
    CHECK(mapa.obterMelhoria({0, 1}) == &item);
    CHECK_FALSE(outra.coletada());

    mapa.removerMelhoria({0, 1});
    bomberman::Jogador jogador({0, 1});
    item.aplicarEm(jogador);
    CHECK_THROWS_AS(mapa.registrarMelhoria({0, 1}, item), std::invalid_argument);
    CHECK(mapa.obterMelhoria({0, 1}) == nullptr);
}

TEST_CASE("Defensiva: bloco rejeita tipo posicao e melhoria inconsistentes") {
    CHECK_THROWS_AS((Bloco({-1, 0}, TipoBloco::Destrutivel)), std::invalid_argument);
    CHECK_THROWS_AS((Bloco({0, 0}, static_cast<TipoBloco>(99))), std::invalid_argument);
    bomberman::Melhoria item({1, 0}, bomberman::TipoMelhoria::Escudo);
    CHECK_THROWS_AS((Bloco({0, 1}, TipoBloco::Indestrutivel, &item)), std::invalid_argument);
    CHECK_THROWS_AS((Bloco({1, 0}, TipoBloco::Destrutivel, &item)), std::invalid_argument);
}

TEST_CASE_FIXTURE(TestesMapa, "Defensiva: impacto em mapa errado nao destroi bloco") {
    Bloco* original = mapa.obterBloco({1, 1});
    REQUIRE(original != nullptr);
    Mapa outro(3, 3, {Bloco({1, 1}, TipoBloco::Destrutivel)}, {0, 0}, {}, {2, 2});
    CHECK_THROWS_AS(original->receberExplosao(outro), std::invalid_argument);
    CHECK_FALSE(original->estaDestruido());
    CHECK_FALSE(outro.obterBloco({1, 1})->estaDestruido());
    CHECK_FALSE(mapa.podeEntrar({1, 1}));
}

TEST_CASE("Defensiva: melhoria escondida inconsistente nao deixa destruicao parcial") {
    bomberman::Melhoria item({1, 1}, bomberman::TipoMelhoria::Escudo);
    Mapa mapa(3, 3, {Bloco({1, 1}, TipoBloco::Destrutivel, &item)}, {0, 0}, {}, {2, 2});
    Bloco* bloco = mapa.obterBloco({1, 1});
    REQUIRE(bloco != nullptr);
    SUBCASE("Item consumido indevidamente antes do impacto") {
        bomberman::Jogador jogador({0, 0});
        item.aplicarEm(jogador);
    }
    SUBCASE("Objeto referenciado substituido por item em outra posicao") {
        item = bomberman::Melhoria({0, 1}, bomberman::TipoMelhoria::Escudo);
    }
    CHECK_THROWS_AS(bloco->receberExplosao(mapa), std::logic_error);
    CHECK_FALSE(bloco->estaDestruido());
    CHECK_FALSE(mapa.podeEntrar({1, 1}));
    CHECK(mapa.obterMelhoria({1, 1}) == nullptr);
}

TEST_CASE("Defensiva: melhoria rejeita entrada invalida e preserva item em falha") {
    CHECK_THROWS_AS((bomberman::Melhoria({-1, 0}, bomberman::TipoMelhoria::Escudo)),
                    std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Melhoria({0, 0}, static_cast<bomberman::TipoMelhoria>(99))),
                    std::invalid_argument);
    bomberman::Jogador jogador({0, 0});
    bomberman::Melhoria velocidade({0, 0}, bomberman::TipoMelhoria::AumentoVelocidade);
    CHECK_THROWS_AS(velocidade.aplicarEm(jogador), std::logic_error);
    CHECK_FALSE(velocidade.coletada());
    CHECK(jogador.ativo());
    CHECK_FALSE(jogador.temEscudo());
}

TEST_CASE("Defensiva: jogador derrotado nao consome melhoria") {
    bomberman::Jogador jogador({0, 0});
    jogador.receberExplosao();
    bomberman::Melhoria item({0, 0}, bomberman::TipoMelhoria::BombaExtra);
    const int limiteAntes = jogador.obterLimiteBombas();
    item.aplicarEm(jogador);
    CHECK_FALSE(item.coletada());
    CHECK(jogador.obterLimiteBombas() == limiteAntes);
}

TEST_CASE("Defensiva: explosao rejeita origem e alcance invalidos") {
    Mapa mapa(3, 4, {Bloco({1, 2}, TipoBloco::Destrutivel)}, {0, 0}, {}, {2, 3});
    CHECK_THROWS_AS((bomberman::Explosao({-1, 0}, 1, mapa)), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Explosao({0, -1}, 1, mapa)), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Explosao({4, 0}, 1, mapa)), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Explosao({0, 0}, 0, mapa)), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Explosao({0, 0}, -1, mapa)), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Explosao({2, 1}, 1, mapa)), std::invalid_argument);
    REQUIRE(mapa.obterBloco({1, 2}) != nullptr);
    CHECK_FALSE(mapa.obterBloco({1, 2})->estaDestruido());
}

TEST_CASE("Defensiva: alcance extremo termina nas bordas da arena") {
    Mapa mapa(3, 7, {}, {0, 0}, {}, {2, 6});
    bomberman::Explosao explosao({4, 1}, std::numeric_limits<int>::max(), mapa);
    CHECK_NOTHROW(explosao.propagar());
    CHECK(explosao.getCelulasAtingidas().size() == 9);
    for (const auto posicao : explosao.getCelulasAtingidas()) {
        CHECK(mapa.dentroDosLimites({posicao.y, posicao.x}));
    }
}

TEST_CASE("Defensiva: explosao nao reativa com tempos extremos repetidos") {
    Mapa mapa(3, 3, {}, {0, 0}, {}, {2, 2});
    bomberman::Jogador jogador({1, 1});
    bomberman::Explosao explosao({1, 1}, 1, mapa);
    explosao.propagar();
    const auto extremo = sf::microseconds(std::numeric_limits<std::int64_t>::max());
    explosao.atualizar(extremo);
    CHECK_FALSE(explosao.ativa());
    explosao.atualizar(extremo);
    CHECK_FALSE(explosao.ativa());
    explosao.aplicarEm(jogador);
    CHECK(jogador.ativo());
}

TEST_CASE("Defensiva: configuracao grande valida e duplicata no fim da lista") {
    std::vector<Posicao> inimigos;
    for (int coluna = 1; coluna < 10000; ++coluna) {
        inimigos.push_back({0, coluna});
    }
    Mapa mapa(1, 10001, {}, {0, 0}, inimigos, {0, 10000});
    CHECK(mapa.obterIniciosInimigos().size() == inimigos.size());
    inimigos.back() = inimigos.front();
    CHECK_THROWS_AS((Mapa(1, 10001, {}, {0, 0}, inimigos, {0, 10000})), std::invalid_argument);
}
