#include "Jogador.hpp"

#include "Mapa.hpp"
#include <limits>
#include <stdexcept>

namespace bomberman {

Jogador::Jogador(Posicao posicao)
    : posicao_(posicao)
    , ativo_(true)
    , escudo_(false)
    , alcance_(2)
    , limiteBombas_(1)
{
    if (posicao_.linha < 0 || posicao_.coluna < 0) {
        throw std::invalid_argument("A posicao do jogador nao pode ser negativa.");
    }
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
    if (!ativo_ || (!passoVertical && !passoHorizontal)
        || !mapa.dentroDosLimites(posicao_)) {
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
    if (alcance_ == std::numeric_limits<int>::max()) {
        throw std::overflow_error("O alcance do jogador atingiu o limite numerico.");
    }
    alcance_++; 
}

void Jogador::aumentarLimiteBombas() {
    if (limiteBombas_ == std::numeric_limits<int>::max()) {
        throw std::overflow_error("O limite de bombas atingiu o maximo numerico.");
    }
    limiteBombas_++;
}

void Jogador::ativarEscudo() {
    escudo_ = true;
}
} // namespace bomberman
