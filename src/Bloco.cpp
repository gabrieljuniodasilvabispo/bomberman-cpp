#include "Bloco.hpp"
#include "Mapa.hpp"
#include "Melhoria.hpp"
#include <stdexcept>

Bloco::Bloco(Posicao posicao, TipoBloco tipo, Melhoria* melhoria)
    : posicao_(posicao), tipo_(tipo), destruido_(false), melhoria_(melhoria) {
    if (posicao.linha < 0 || posicao.coluna < 0
        || (tipo != TipoBloco::Destrutivel && tipo != TipoBloco::Indestrutivel)) {
        throw std::invalid_argument("Posicao ou tipo de bloco invalido.");
    }
    if (melhoria != nullptr) {
        const auto posicaoItem = melhoria->getPosicao();
        if (tipo != TipoBloco::Destrutivel || posicaoItem.x != posicao.coluna
            || posicaoItem.y != posicao.linha) {
            throw std::invalid_argument("Melhoria deve ocupar a mesma celula de um bloco destrutivel.");
        }
    }
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

    if (mapa.obterBloco(posicao_) != this) {
        throw std::invalid_argument("O bloco nao pertence ao mapa informado.");
    }
    if (melhoria_ != nullptr && melhoria_->coletada()) {
        throw std::logic_error("A melhoria escondida ja foi coletada.");
    }
    if (melhoria_ != nullptr) {
        const auto posicaoItem = melhoria_->getPosicao();
        if (posicaoItem.x != posicao_.coluna || posicaoItem.y != posicao_.linha) {
            throw std::logic_error("A melhoria escondida mudou de posicao.");
        }
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
