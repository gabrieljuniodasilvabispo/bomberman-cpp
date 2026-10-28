#include "Bloco.hpp"
#include "Mapa.hpp"

Bloco::Bloco(Posicao posicao, TipoBloco tipo, Melhoria* melhoria)
    : posicao_(posicao), tipo_(tipo), destruido_(false), melhoria_(melhoria) {
}

Posicao Bloco::obterPosicao() const {
    return posicao_;
}

TipoBloco Bloco::obterTipo() const {
    return tipo_;
}

bool Bloco::estaDestruido() const {
    return destruido_;
}

bool Bloco::impedePassagem() const {
    return !destruido_;
}

bool Bloco::receberExplosao(Mapa& mapa) {
    if (destruido_) {
        return false;
    }

    if (tipo_ == TipoBloco::Indestrutivel) {
        return true;
    }

    destruido_ = true;
    mapa.liberarBlocoDestruido(posicao_);

    if (melhoria_ != nullptr) {
        mapa.registrarMelhoria(posicao_, *melhoria_);
    }

    // Mesmo destruido agora, o bloco interrompe esta propagacao.
    return true;
}
