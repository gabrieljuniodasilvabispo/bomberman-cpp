#include "Jogador.hpp"

#include "Mapa.hpp"

namespace bomberman {

Jogador::Jogador(Posicao posicao)
    : posicao_(posicao)
    , ativo_(true)
    , escudo_(false)
    , alcance_(2)
    , limiteBombas_(1)
{
}

Posicao Jogador::obterPosicao() const {
    return posicao_;
}

bool Jogador::ativo() const {
    return ativo_;
}

bool Jogador::mover(Posicao deslocamento, ::Mapa& mapa) {
    if (!ativo_) {
        return false;
    }

    Posicao destino{
        posicao_.linha + deslocamento.linha,
        posicao_.coluna + deslocamento.coluna
    };

    if (!mapa.dentroDosLimites(destino)) {
        return false;
    }

    if (!mapa.podeEntrar(destino)) {
        return false;
    }

    posicao_ = destino;
    return true;
}

int Jogador::obterAlcance() const {
    return alcance_;
}

int Jogador::obterLimiteBombas() const {
    return limiteBombas_;
}

bool Jogador::temEscudo() const {
    return escudo_;
}

void Jogador::receberExplosao() {
    if (escudo_) {
        escudo_ = false;
        return;
    }

    ativo_ = false;
}

void Jogador::receberContatoInimigo() {
    ativo_ = false;
}

//implementações realcionadas ao Melhoria.cpp
void Jogador::aumentarAlcance() {
    alcance_++; 
}

void Jogador::aumentarLimiteBombas() {
    limiteBombas_++;
}

void Jogador::ativarEscudo() {
    escudo_ = true;
}
} // namespace bomberman
