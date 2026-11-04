#include "Melhoria.hpp"
#include "Jogador.hpp"
#include <stdexcept>

namespace bomberman {

Melhoria::Melhoria(sf::Vector2i posicao, TipoMelhoria tipo)
    : posicao_(posicao)
    , tipo_(tipo)
    , coletada_(false)
{
    if (posicao.x < 0 || posicao.y < 0) {
        throw std::invalid_argument("A posicao da melhoria nao pode ser negativa.");
    }
    switch (tipo) {
    case TipoMelhoria::AumentoAlcance:
    case TipoMelhoria::BombaExtra:
    case TipoMelhoria::AumentoVelocidade:
    case TipoMelhoria::Escudo:
        break;
    default:
        throw std::invalid_argument("Tipo de melhoria desconhecido.");
    }
}

void Melhoria::aplicarEm(Jogador& jogador) {
    if (coletada_ || !jogador.ativo()) {
        return;
    }

    switch (tipo_) {
    case TipoMelhoria::AumentoAlcance:
        jogador.aumentarAlcance();
        break;
    case TipoMelhoria::BombaExtra:
        jogador.aumentarLimiteBombas();
        break;
    case TipoMelhoria::AumentoVelocidade:
        throw std::logic_error("A melhoria de velocidade ainda nao esta disponivel.");
    case TipoMelhoria::Escudo:
        jogador.ativarEscudo();
        break;
    }
    coletada_ = true;
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
