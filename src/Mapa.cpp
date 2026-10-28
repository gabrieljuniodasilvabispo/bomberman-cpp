#include "Mapa.hpp"

#include <stdexcept>

Mapa::Mapa(int linhas, int colunas, const std::vector<Bloco>& blocos,
           Posicao inicioJogador, const std::vector<Posicao>& iniciosInimigos,
           Posicao saida)
    : linhas_(linhas), colunas_(colunas), inicioJogador_(inicioJogador),
      iniciosInimigos_(iniciosInimigos), saida_(saida),
      blocosIniciais_(blocos), blocos_(blocos) {

    if (linhas_ <= 0 || colunas_ <= 0) {
        throw std::invalid_argument("As dimensoes do mapa devem ser positivas.");
    }

    celulas_.assign(linhas_, std::vector<Celula>(colunas_));

    int indice = 0;
    for (const Bloco& bloco : blocos_) {
        const Posicao posicao = bloco.obterPosicao();
        if (!dentroDosLimites(posicao) || bloco.estaDestruido()) {
            throw std::invalid_argument("Bloco fora da grade ou ja destruido.");
        }

        Celula& celula = celulas_[posicao.linha][posicao.coluna];
        if (celula.indiceBloco != -1) {
            throw std::invalid_argument("Dois blocos ocupam a mesma celula.");
        }
        celula.indiceBloco = indice;
        ++indice;
    }

    if (!podeEntrar(inicioJogador_)) {
        throw std::invalid_argument("Inicio do jogador fora da grade ou bloqueado.");
    }

    std::vector<Posicao> iniciosOcupados{inicioJogador_};
    for (Posicao inicio : iniciosInimigos_) {
        if (!podeEntrar(inicio)) {
            throw std::invalid_argument("Inicio de inimigo fora da grade ou bloqueado.");
        }
        for (Posicao ocupado : iniciosOcupados) {
            if (inicio.linha == ocupado.linha && inicio.coluna == ocupado.coluna) {
                throw std::invalid_argument("Personagens com posicoes iniciais sobrepostas.");
            }
        }
        iniciosOcupados.push_back(inicio);
    }

    if (!podeEntrar(saida_)) {
        throw std::invalid_argument("Saida fora da grade ou bloqueada.");
    }
}

int Mapa::obterLinhas() const {
    return linhas_;
}

int Mapa::obterColunas() const {
    return colunas_;
}

bool Mapa::dentroDosLimites(Posicao posicao) const {
    return posicao.linha >= 0 && posicao.linha < linhas_
        && posicao.coluna >= 0 && posicao.coluna < colunas_;
}

Posicao Mapa::obterInicioJogador() const {
    return inicioJogador_;
}

std::vector<Posicao> Mapa::obterIniciosInimigos() const {
    return iniciosInimigos_;
}

Posicao Mapa::obterSaida() const {
    return saida_;
}

bool Mapa::podeEntrar(Posicao posicao) const {
    return dentroDosLimites(posicao)
        && obterBloco(posicao) == nullptr
        && !temBomba(posicao);
}

Bloco* Mapa::obterBloco(Posicao posicao) {
    if (!dentroDosLimites(posicao)) {
        return nullptr;
    }

    const int indice = celulas_[posicao.linha][posicao.coluna].indiceBloco;
    if (indice == -1 || blocos_[indice].estaDestruido()) {
        return nullptr;
    }
    return &blocos_[indice];
}

const Bloco* Mapa::obterBloco(Posicao posicao) const {
    if (!dentroDosLimites(posicao)) {
        return nullptr;
    }

    const int indice = celulas_[posicao.linha][posicao.coluna].indiceBloco;
    if (indice == -1 || blocos_[indice].estaDestruido()) {
        return nullptr;
    }
    return &blocos_[indice];
}

void Mapa::liberarBlocoDestruido(Posicao posicao) {
    if (!dentroDosLimites(posicao)) {
        return;
    }

    Celula& celula = celulas_[posicao.linha][posicao.coluna];
    if (celula.indiceBloco != -1 && blocos_[celula.indiceBloco].estaDestruido()) {
        // Retira a ocupacao, mas preserva o objeto e os outros dados da celula.
        celula.indiceBloco = -1;
    }
}

bool Mapa::registrarBomba(Posicao posicao) {
    if (!podeEntrar(posicao)) {
        return false;
    }

    celulas_[posicao.linha][posicao.coluna].temBomba = true;
    return true;
}

void Mapa::removerBomba(Posicao posicao) {
    if (dentroDosLimites(posicao)) {
        celulas_[posicao.linha][posicao.coluna].temBomba = false;
    }
}

bool Mapa::temBomba(Posicao posicao) const {
    return dentroDosLimites(posicao)
        && celulas_[posicao.linha][posicao.coluna].temBomba;
}

void Mapa::registrarMelhoria(Posicao posicao, Melhoria& melhoria) {
    if (!dentroDosLimites(posicao) || obterBloco(posicao) != nullptr) {
        return;
    }

    Celula& celula = celulas_[posicao.linha][posicao.coluna];
    if (celula.melhoria == nullptr) {
        celula.melhoria = &melhoria;
    }
}

Melhoria* Mapa::obterMelhoria(Posicao posicao) const {
    if (!dentroDosLimites(posicao)) {
        return nullptr;
    }
    return celulas_[posicao.linha][posicao.coluna].melhoria;
}

void Mapa::removerMelhoria(Posicao posicao) {
    if (dentroDosLimites(posicao)) {
        celulas_[posicao.linha][posicao.coluna].melhoria = nullptr;
    }
}

void Mapa::reiniciar() {
    blocos_ = blocosIniciais_;
    // Celulas novas comecam sem blocos, bombas ou melhorias reveladas.
    celulas_.assign(linhas_, std::vector<Celula>(colunas_));

    int indice = 0;
    for (const Bloco& bloco : blocos_) {
        const Posicao posicao = bloco.obterPosicao();
        celulas_[posicao.linha][posicao.coluna].indiceBloco = indice;
        ++indice;
    }
}
