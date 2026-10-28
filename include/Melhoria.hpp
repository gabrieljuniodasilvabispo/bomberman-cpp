#include "Melhoria.hpp"
#include "Jogador.hpp"

namespace bomberman {

Melhoria::Melhoria(sf::Vector2i posicao, TipoMelhoria tipo)
    : posicao_(posicao)
    , tipo_(tipo)
    , coletada_(false)
{
}

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
        jogador.aumentarVelocidade();
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
    // Atualização de tempo de permanência ou animação do item no mapa
    (void)deltaTempo;
}

sf::Vector2i Melhoria::getPosicao() const {
    return posicao_;
}

TipoMelhoria Melhoria::getTipo() const {
    return tipo_;
}

} // namespace bomberman

🎮 *Quer ajuda para integrar a geração desses power-ups no método de destruição de blocos da classe `Mapa`?*
