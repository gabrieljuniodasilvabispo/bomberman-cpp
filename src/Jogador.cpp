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
    const bool passoVertical = deslocamento.coluna == 0
        && (deslocamento.linha == -1 || deslocamento.linha == 1);
    const bool passoHorizontal = deslocamento.linha == 0
        && (deslocamento.coluna == -1 || deslocamento.coluna == 1);
    if (!ativo_ || (!passoVertical && !passoHorizontal)) {
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

int Jogador::obterBombasAtivas() const {
    return bombasAtivas_;
}

bool Jogador::reservarVagaBomba() {
    if (!ativo_ || bombasAtivas_ >= limiteBombas_) {
        return false;
    }
    ++bombasAtivas_;
    return true;
}

void Jogador::liberarVagaBomba() {
    if (bombasAtivas_ > 0) {
        --bombasAtivas_;
    }
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
