#include "Explosao.hpp"

#include "Bomba.hpp"
#include "Bloco.hpp"
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Mapa.hpp"
#include "Melhoria.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

namespace bomberman {

namespace {
// Duração do efeito visual/de colisão da explosão, em segundos.
constexpr float DURACAO_EFEITO_SEGUNDOS = 0.5f;

// As quatro direções possíveis, como deslocamento de uma célula.
constexpr std::array<sf::Vector2i, 4> DIRECOES = {
    sf::Vector2i{0, -1}, // cima
    sf::Vector2i{0, 1},  // baixo
    sf::Vector2i{-1, 0}, // esquerda
    sf::Vector2i{1, 0}   // direita
};
} // namespace

Explosao::Explosao(sf::Vector2i origem, int alcance, Mapa& mapa)
    : origem_(origem)
    , alcance_(alcance)
    , mapa_(mapa)
    , tempoRestante_(sf::seconds(DURACAO_EFEITO_SEGUNDOS))
{
    if (alcance_ <= 0 || !mapa_.dentroDosLimites({origem_.y, origem_.x})) {
        throw std::invalid_argument("Origem ou alcance de explosao invalido.");
    }
}

void Explosao::propagar()
{
    if (propagada_ || !ativa()) {
        return;
    }
    celulasAtingidas_.clear();
    celulasAtingidas_.push_back(origem_);
    mapa_.removerMelhoria({origem_.y, origem_.x});

    for (const sf::Vector2i& direcao : DIRECOES)
    {
        for (int passo = 1; passo <= alcance_; ++passo)
        {
            const sf::Vector2i posicao = origem_ + direcao * passo;

            const Posicao celula{posicao.y, posicao.x};
            if (!mapa_.dentroDosLimites(celula))
            {
                break;
            }

            celulasAtingidas_.push_back(posicao);

            Bloco* bloco = mapa_.obterBloco(celula);
            if (bloco == nullptr)
            {
                mapa_.removerMelhoria(celula);
                continue;
            }

            // O bloco libera sua celula e registra a melhoria que escondia.
            const bool interrompePropagacao = bloco->receberExplosao(mapa_);

            if (interrompePropagacao)
            {
                break;
            }
        }
    }
    propagada_ = true;
}

bool Explosao::ativa() const
{
    return tempoRestante_ > sf::Time::Zero;
}

void Explosao::atualizar(sf::Time deltaTempo)
{
    if (deltaTempo > sf::Time::Zero) {
        tempoRestante_ -= deltaTempo;
    }
}

const std::vector<sf::Vector2i>& Explosao::getCelulasAtingidas() const
{
    return celulasAtingidas_;
}

void Explosao::aplicarEm(Jogador& jogador)
{
    if (!ativa() || !jogador.ativo()
        || std::find(jogadoresAtingidos_.begin(), jogadoresAtingidos_.end(), &jogador)
            != jogadoresAtingidos_.end()) {
        return;
    }
    const Posicao posicao = jogador.obterPosicao();
    const bool atingido = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(),
                                   sf::Vector2i{posicao.coluna, posicao.linha})
        != celulasAtingidas_.end();

    if (atingido)
    {
        jogadoresAtingidos_.push_back(&jogador);
        jogador.receberExplosao();
    }
}

void Explosao::aplicarEm(Inimigo& inimigo)
{
    if (!ativa() || !inimigo.ativo())
    {
        return;
    }

    const Posicao posicao = inimigo.obterPosicao();
    const bool atingido = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(),
                                   sf::Vector2i{posicao.coluna, posicao.linha})
        != celulasAtingidas_.end();

    if (atingido)
    {
        inimigo.receberExplosao();
    }
}

void Explosao::aplicarEm(Bomba& bomba)
{
    if (!ativa()) {
        return;
    }
    const bool atingida = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(), bomba.getPosicao())
        != celulasAtingidas_.end();

    if (atingida)
    {
        bomba.anteciparDetonacao();
    }
}

} // namespace bomberman
