#include "Bomba.hpp"

#include "Explosao.hpp"
#include "Jogador.hpp"

namespace bomberman {

namespace {
// Tempo padrão até a detonação, em segundos. Mantido aqui (e não no header)
// porque é um detalhe de implementação, não parte da interface da classe.
constexpr float TEMPO_DETONACAO_SEGUNDOS = 3.0f;
} // namespace

Bomba::Bomba(sf::Vector2i posicao, int alcance, Jogador& responsavel)
    : posicao_(posicao)
    , alcance_(alcance)
    , responsavel_(responsavel)
    , tempoRestante_(sf::seconds(TEMPO_DETONACAO_SEGUNDOS))
{
}

sf::Vector2i Bomba::getPosicao() const
{
    return posicao_;
}

int Bomba::getAlcance() const
{
    return alcance_;
}

bool Bomba::prontaParaExplodir() const
{
    return !detonada_ && tempoRestante_ <= sf::Time::Zero;
}

void Bomba::atualizar(sf::Time deltaTempo)
{
    if (detonada_)
    {
        return;
    }

    tempoRestante_ -= deltaTempo;
}

void Bomba::anteciparDetonacao()
{
    // Zera o tempo restante: no próximo ciclo, prontaParaExplodir() passa a
    // retornar true e quem estiver atualizando a partida chama detonar().
    tempoRestante_ = sf::Time::Zero;
}

std::unique_ptr<Explosao> Bomba::detonar(Mapa& mapa)
{
    detonada_ = true;

    // Devolve a vaga de bomba ao jogador responsável, conforme o CRC04.
    responsavel_.liberarVagaBomba();

    auto explosao = std::make_unique<Explosao>(posicao_, alcance_, mapa);
    explosao->propagar();

    return explosao;
}

} // namespace bomberman
