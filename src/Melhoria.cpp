#include "Melhoria.hpp"
#include "Jogador.hpp"

namespace bomberman {

Melhoria::Melhoria(sf::Vector2i posicao, TipoMelhoria tipo)
    : posicao_(posicao)
    , tipo_(tipo)
    , coletada_(false)
{}

void Melhoria::aplicarEm(Jogador& jogador) {
    if (coletada_) {
        return;
    }

    coletada_ = true;

    switch (tipo_) {
    case TipoMelhoria::AumentoAlcance:
        jogador.aumentarAlcance();
        break;
    case TipoMelhoria::BombaExtra:
        jogador.aumentarLimiteBombas();
        break;
    case TipoMelhoria::AumentoVelocidade:
        // Reservado para quando o Jogador possuir controle de velocidade
        break;
    case TipoMelhoria::Escudo:
        jogador.ativarEscudo();
        break;
    }
}

bool Melhoria::coletada() const {
    return coletada_;
}

void Melhoria::atualizar(sf::Time deltaTempo) {
    (void)deltaTempo;
}

sf::Vector2i Melhoria::getPosicao() const {
    return posicao_;
}

TipoMelhoria Melhoria::getTipo() const {
    return tipo_;
}

} // namespace bomberman
