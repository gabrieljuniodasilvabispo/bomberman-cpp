#include "Explosao.hpp"

#include "Bomba.hpp"
#include "Bloco.hpp"
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Mapa.hpp"
#include "Melhoria.hpp"

#include <algorithm>
#include <array>

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
}

void Explosao::propagar()
{
    celulasAtingidas_.clear();
    celulasAtingidas_.push_back(origem_);

    for (const sf::Vector2i& direcao : DIRECOES)
    {
        for (int passo = 1; passo <= alcance_; ++passo)
        {
            const sf::Vector2i posicao = origem_ + direcao * passo;

            if (!mapa_.dentroDosLimites(posicao))
            {
                break;
            }

            celulasAtingidas_.push_back(posicao);

            Bloco* bloco = mapa_.getBloco(posicao);
            if (bloco == nullptr)
            {
                // Célula livre: a explosão continua se propagando nessa direção.
                continue;
            }

            const bool interrompePropagacao = bloco->receberImpacto();

            if (bloco->estaDestruido())
            {
                std::unique_ptr<Melhoria> melhoriaRevelada = bloco->revelarMelhoria();
                mapa_.liberarCelula(posicao);

                if (melhoriaRevelada)
                {
                    melhoriaRevelada->revelar();
                    mapa_.adicionarMelhoriaAtiva(std::move(melhoriaRevelada));
                }
            }

            if (interrompePropagacao)
            {
                break;
            }
        }
    }
}

bool Explosao::ativa() const
{
    return tempoRestante_ > sf::Time::Zero;
}

void Explosao::atualizar(sf::Time deltaTempo)
{
    tempoRestante_ -= deltaTempo;
}

const std::vector<sf::Vector2i>& Explosao::getCelulasAtingidas() const
{
    return celulasAtingidas_;
}

void Explosao::aplicarEm(Jogador& jogador)
{
    const bool atingido = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(), jogador.getPosicao())
        != celulasAtingidas_.end();

    if (atingido)
    {
        jogador.serAtingido();
    }
}

void Explosao::aplicarEm(Inimigo& inimigo)
{
    if (!inimigo.estaAtivo())
    {
        return;
    }

    const bool atingido = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(), inimigo.getPosicao())
        != celulasAtingidas_.end();

    if (atingido)
    {
        inimigo.eliminar();
    }
}

void Explosao::aplicarEm(Bomba& bomba)
{
    const bool atingida = std::find(celulasAtingidas_.begin(), celulasAtingidas_.end(), bomba.getPosicao())
        != celulasAtingidas_.end();

    if (atingida)
    {
        bomba.anteciparDetonacao();
    }
}

} // namespace bomberman
