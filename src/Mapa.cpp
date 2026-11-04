#include "Mapa.hpp"
#include "Melhoria.hpp"

#include <stdexcept>

Mapa::Mapa(int linhas, int colunas, const std::vector<Bloco>& blocos,
           Posicao inicioJogador, const std::vector<Posicao>& iniciosInimigos,
           Posicao saida)
    : linhas_(linhas), colunas_(colunas), inicioJogador_(inicioJogador),
      saida_(saida) {

    if (linhas_ <= 0 || colunas_ <= 0) {
        throw std::invalid_argument("As dimensoes do mapa devem ser positivas.");
    }

    // Divide antes de multiplicar: mesmo INT_MAX nao causa overflow.
    if (linhas_ > MAXIMO_CELULAS / colunas_) {
        throw std::length_error("O mapa excede o limite de um milhao de celulas.");
    }
    const auto quantidadeCelulas = static_cast<std::size_t>(linhas_) * colunas_;
    if (blocos.size() > quantidadeCelulas || iniciosInimigos.size() >= quantidadeCelulas) {
        throw std::invalid_argument("Quantidade de blocos ou personagens excede a arena.");
    }
    blocosIniciais_ = blocos;
    blocos_ = blocos;
    iniciosInimigos_ = iniciosInimigos;

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

    std::vector<bool> iniciosOcupados(quantidadeCelulas, false);
    const auto indiceInicial = [this](Posicao posicao) {
        return static_cast<std::size_t>(posicao.linha) * colunas_ + posicao.coluna;
    };
    iniciosOcupados[indiceInicial(inicioJogador_)] = true;
    for (Posicao inicio : iniciosInimigos_) {
        if (!podeEntrar(inicio)) {
            throw std::invalid_argument("Inicio de inimigo fora da grade ou bloqueado.");
        }
        const auto indiceCelula = indiceInicial(inicio);
        if (iniciosOcupados[indiceCelula]) {
            throw std::invalid_argument("Personagens com posicoes iniciais sobrepostas.");
        }
        iniciosOcupados[indiceCelula] = true;
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
    const auto posicaoItem = melhoria.getPosicao();
    if (!dentroDosLimites(posicao) || obterBloco(posicao) != nullptr
        || posicaoItem.x != posicao.coluna || posicaoItem.y != posicao.linha
        || melhoria.coletada()) {
        throw std::invalid_argument("Melhoria indisponivel ou posicao de registro invalida.");
    }

    Celula& celula = celulas_[posicao.linha][posicao.coluna];
    if (celula.melhoria != nullptr && celula.melhoria != &melhoria) {
        throw std::logic_error("Outra melhoria ja ocupa esta celula.");
    }
    celula.melhoria = &melhoria;
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
    // Prepara o novo estado antes de invalidar os objetos atuais.
    auto novosBlocos = blocosIniciais_;
    std::vector<std::vector<Celula>> novasCelulas(linhas_, std::vector<Celula>(colunas_));

    int indice = 0;
    for (const Bloco& bloco : novosBlocos) {
        const Posicao posicao = bloco.obterPosicao();
        novasCelulas[posicao.linha][posicao.coluna].indiceBloco = indice;
        ++indice;
    }
    blocos_.swap(novosBlocos);
    celulas_.swap(novasCelulas);
}
