#include "Inimigo.hpp"

#include <array>
#include <random>

#include "Jogador.hpp"
#include "Mapa.hpp"

namespace bomberman {

Inimigo::Inimigo(Posicao posicao)
    : posicao_(posicao)
    , ativo_(true)
{
}

Posicao Inimigo::obterPosicao() const {
    return posicao_;
}

bool Inimigo::ativo() const {
    return ativo_;
}

void Inimigo::atualizar(sf::Time deltaTempo, ::Mapa& mapa) {
    if (!ativo_ || deltaTempo <= sf::Time::Zero) {
        return;
    }

    static std::mt19937 rng{std::random_device{}()};

    std::array<Posicao, 4> direcoes{
        Posicao{1, 0},
        Posicao{-1, 0},
        Posicao{0, 1},
        Posicao{0, -1}
    };

    std::shuffle(direcoes.begin(), direcoes.end(), rng);

    for (const auto& direcao : direcoes) {
        Posicao destino{
            posicao_.linha + direcao.linha,
            posicao_.coluna + direcao.coluna
        };

        if (mapa.dentroDosLimites(destino) && mapa.podeEntrar(destino)) {
            posicao_ = destino;
            break;
        }
    }
}

void Inimigo::receberExplosao() {
    ativo_ = false;
}

void Inimigo::verificarContato(Jogador& jogador) {
    if (!ativo_) {
        return;
    }

    const Posicao posicaoJogador = jogador.obterPosicao();
    if (posicaoJogador.linha == posicao_.linha && posicaoJogador.coluna == posicao_.coluna) {
        jogador.receberContatoInimigo();
    }
}

} // namespace bomberman
