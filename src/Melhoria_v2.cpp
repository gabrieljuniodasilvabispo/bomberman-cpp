#include "Melhoria.hpp"
#include "Jogador.hpp"

namespace bomberman {

Melhoria::Melhoria(Posicao posicao, TipoMelhoria tipo)
    : posicao_(posicao)
    , tipo_(tipo)
    , coletada_(false)
{
}

Posicao Melhoria::obterPosicao() const {
    return posicao_;
}

TipoMelhoria Melhoria::obterTipo() const {
    return tipo_;
}

bool Melhoria::foiColetada() const {
    return coletada_;
}

void Melhoria::coletar(Jogador& jogador) {
    if (coletada_) {
        return;
    }

    coletada_ = true;

    switch (tipo_) {
    case TipoMelhoria::AumentarAlcance:
        jogador.aumentarAlcance();
        break;
    case TipoMelhoria::AumentarQuantidadeBomba:
        jogador.aumentarLimiteBombas();
        break;
    case TipoMelhoria::Escudo:
        jogador.ativarEscudo();
        break;
    }
}

} // namespace bomberman
