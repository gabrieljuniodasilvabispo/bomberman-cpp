#include "Partida.hpp"

#include "Bomba.hpp"
#include "Explosao.hpp"
#include "Jogador.hpp"
#include "Mapa.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace bomberman {
namespace {

ConfiguracaoFase criarConfiguracaoPadrao() {
    ConfiguracaoFase configuracao;
    configuracao.iniciosInimigos = {{1, 9}, {7, 1}, {7, 9}};
    for (int linha = 0; linha < configuracao.linhas; ++linha) {
        for (int coluna = 0; coluna < configuracao.colunas; ++coluna) {
            const bool borda = linha == 0 || coluna == 0
                || linha == configuracao.linhas - 1 || coluna == configuracao.colunas - 1;
            if (borda || (linha % 2 == 0 && coluna % 2 == 0)) {
                configuracao.blocos.push_back({{linha, coluna}, TipoBloco::Indestrutivel});
            }
        }
    }
    // A area inicial permite dobrar um corredor para escapar da primeira bomba.
    configuracao.blocos.push_back({{1, 4}, TipoBloco::Destrutivel, TipoMelhoria::BombaExtra});
    configuracao.blocos.push_back({{4, 1}, TipoBloco::Destrutivel, TipoMelhoria::AumentoAlcance});
    configuracao.blocos.push_back({{3, 3}, TipoBloco::Destrutivel, TipoMelhoria::Escudo});
    for (Posicao posicao : {Posicao{1, 7}, Posicao{3, 5}, Posicao{3, 7}, Posicao{3, 9},
                            Posicao{5, 1}, Posicao{5, 3}, Posicao{5, 5}, Posicao{5, 7},
                            Posicao{5, 9}, Posicao{7, 3}, Posicao{7, 5}, Posicao{7, 7}}) {
        configuracao.blocos.push_back({posicao, TipoBloco::Destrutivel});
    }
    return configuracao;
}

std::vector<Bloco> criarBlocos(const ConfiguracaoFase& configuracao,
                              std::vector<std::unique_ptr<Melhoria>>& melhorias) {
    std::vector<Bloco> blocos;
    for (const auto& inicial : configuracao.blocos) {
        Melhoria* melhoria = nullptr;
        if (inicial.melhoria) {
            if (*inicial.melhoria == TipoMelhoria::AumentoVelocidade) {
                throw std::invalid_argument("A fase nao suporta melhoria de velocidade.");
            }
            melhorias.push_back(std::make_unique<Melhoria>(
                sf::Vector2i{inicial.posicao.coluna, inicial.posicao.linha}, *inicial.melhoria));
            melhoria = melhorias.back().get();
        }
        blocos.emplace_back(inicial.posicao, inicial.tipo, melhoria);
    }
    return blocos;
}

bool mesmaPosicao(Posicao a, Posicao b) {
    return a.linha == b.linha && a.coluna == b.coluna;
}

} // namespace

struct Partida::ElementosFase {
    // Destruicao em ordem inversa: referencias de bombas/explosoes nunca ficam pendentes.
    std::vector<std::unique_ptr<Melhoria>> melhorias;
    Mapa mapa;
    Jogador jogador;
    std::vector<Inimigo> inimigos;
    std::vector<std::unique_ptr<Bomba>> bombas;
    std::vector<std::unique_ptr<Explosao>> explosoes;

    explicit ElementosFase(const ConfiguracaoFase& configuracao)
        : mapa(configuracao.linhas, configuracao.colunas, criarBlocos(configuracao, melhorias),
               configuracao.inicioJogador, configuracao.iniciosInimigos, configuracao.saida),
          jogador(mapa.obterInicioJogador()) {
        for (Posicao posicao : mapa.obterIniciosInimigos()) {
            inimigos.emplace_back(posicao);
        }
    }
};

Partida::Partida() : Partida(criarConfiguracaoPadrao()) {}

Partida::Partida(ConfiguracaoFase configuracao) : configuracao_(std::move(configuracao)) {}

Partida::~Partida() = default;

void Partida::iniciarFase() {
    auto novaFase = std::make_unique<ElementosFase>(configuracao_);
    fase_.swap(novaFase);
    estado_ = EstadoPartida::EmAndamento;
    verificarResultado();
}

bool Partida::moverJogador(Posicao deslocamento) {
    if (estado_ != EstadoPartida::EmAndamento
        || !fase_->jogador.mover(deslocamento, fase_->mapa)) {
        return false;
    }
    processarExplosoes();
    resolverContatosEColeta();
    verificarResultado();
    return true;
}

bool Partida::colocarBomba() {
    if (estado_ != EstadoPartida::EmAndamento) {
        return false;
    }
    auto& jogador = fase_->jogador;
    const Posicao posicao = jogador.obterPosicao();
    if (!jogador.ativo() || jogador.obterBombasAtivas() >= jogador.obterLimiteBombas()
        || !fase_->mapa.podeEntrar(posicao)) {
        return false;
    }
    // Inserir antes de registrar evita deixar ocupacao orfa se a alocacao falhar.
    auto bomba = std::make_unique<Bomba>(sf::Vector2i{posicao.coluna, posicao.linha},
                                         jogador.obterAlcance(), jogador);
    fase_->bombas.push_back(std::move(bomba));
    if (!fase_->mapa.registrarBomba(posicao)) {
        fase_->bombas.pop_back();
        return false;
    }
    processarExplosoes();
    resolverContatosEColeta();
    verificarResultado();
    return true;
}

void Partida::processarExplosoes() {
    const auto aplicar = [this](Explosao& explosao) {
        explosao.aplicarEm(fase_->jogador);
        for (auto& inimigo : fase_->inimigos) {
            explosao.aplicarEm(inimigo);
        }
        for (auto& bomba : fase_->bombas) {
            explosao.aplicarEm(*bomba);
        }
    };
    for (auto& explosao : fase_->explosoes) {
        aplicar(*explosao);
    }
    // Cada nova explosao pode antecipar outra bomba, inclusive uma ja visitada.
    // Retomar a busca resolve a cadeia inteira antes de verificar o resultado.
    auto& bombas = fase_->bombas;
    while (true) {
        auto pronta = std::find_if(bombas.begin(), bombas.end(),
                                  [](const auto& bomba) { return bomba->prontaParaExplodir(); });
        if (pronta == bombas.end()) {
            break;
        }
        fase_->explosoes.reserve(fase_->explosoes.size() + 1);
        auto explosao = (*pronta)->detonar(fase_->mapa);
        bombas.erase(pronta);
        fase_->explosoes.push_back(std::move(explosao));
        aplicar(*fase_->explosoes.back());
    }
}

void Partida::resolverContatosEColeta() {
    for (auto& inimigo : fase_->inimigos) {
        inimigo.verificarContato(fase_->jogador);
    }
    if (fase_->jogador.ativo()) {
        const Posicao posicao = fase_->jogador.obterPosicao();
        if (auto* melhoria = fase_->mapa.obterMelhoria(posicao)) {
            melhoria->aplicarEm(fase_->jogador);
            fase_->mapa.removerMelhoria(posicao);
        }
    }
}

void Partida::atualizar(sf::Time deltaTempo) {
    if (estado_ != EstadoPartida::EmAndamento || deltaTempo <= sf::Time::Zero
        || deltaTempo > sf::seconds(5)) {
        return;
    }
    auto restante = deltaTempo.asMicroseconds();
    while (restante > 0 && estado_ == EstadoPartida::EmAndamento) {
        const sf::Time passo = sf::microseconds(std::min(restante, sf::milliseconds(10).asMicroseconds()));
        restante -= passo.asMicroseconds();
        auto& explosoes = fase_->explosoes;
        for (auto& explosao : explosoes) {
            explosao->atualizar(passo);
        }
        explosoes.erase(std::remove_if(explosoes.begin(), explosoes.end(),
                                      [](const auto& explosao) { return !explosao->ativa(); }),
                        explosoes.end());
        for (auto& bomba : fase_->bombas) {
            bomba->atualizar(passo);
        }
        processarExplosoes();
        if (fase_->jogador.ativo()) {
            for (auto& inimigo : fase_->inimigos) {
                inimigo.atualizar(passo, fase_->mapa);
            }
            // Inimigos que entram no fogo sao eliminados antes de causar contato.
            processarExplosoes();
        }
        resolverContatosEColeta();
        verificarResultado();
    }
}

bool Partida::todosInimigosEliminados() const {
    return fase_ && std::none_of(fase_->inimigos.begin(), fase_->inimigos.end(),
                                [](const Inimigo& inimigo) { return inimigo.ativo(); });
}

void Partida::verificarResultado() {
    if (!fase_->jogador.ativo()) {
        estado_ = EstadoPartida::Derrota;
    } else if (todosInimigosEliminados()
               && mesmaPosicao(fase_->jogador.obterPosicao(), fase_->mapa.obterSaida())) {
        estado_ = EstadoPartida::Vitoria;
    }
}

void Partida::encerrarFase() {
    if (estado_ == EstadoPartida::EmAndamento) {
        estado_ = EstadoPartida::Encerrada;
    }
}

void Partida::reiniciarFase() {
    iniciarFase();
}

EstadoPartida Partida::obterEstado() const { return estado_; }

const Mapa& Partida::obterMapa() const {
    if (!fase_) {
        throw std::logic_error("A fase ainda nao foi iniciada.");
    }
    return fase_->mapa;
}

const Jogador& Partida::obterJogador() const {
    if (!fase_) {
        throw std::logic_error("A fase ainda nao foi iniciada.");
    }
    return fase_->jogador;
}

const std::vector<Inimigo>& Partida::obterInimigos() const {
    static const std::vector<Inimigo> vazios;
    return fase_ ? fase_->inimigos : vazios;
}

std::size_t Partida::obterQuantidadeBombas() const { return fase_ ? fase_->bombas.size() : 0; }
std::size_t Partida::obterQuantidadeExplosoes() const { return fase_ ? fase_->explosoes.size() : 0; }

const Bomba& Partida::obterBomba(std::size_t indice) const {
    if (!fase_) {
        throw std::out_of_range("Nao existem bombas antes do inicio da fase.");
    }
    return *fase_->bombas.at(indice);
}

const Explosao& Partida::obterExplosao(std::size_t indice) const {
    if (!fase_) {
        throw std::out_of_range("Nao existem explosoes antes do inicio da fase.");
    }
    return *fase_->explosoes.at(indice);
}

} // namespace bomberman
