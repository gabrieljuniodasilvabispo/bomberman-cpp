#include "apoio.hpp"
#include "Bomba.hpp"
#include "Explosao.hpp"
#include "Partida.hpp"

#include <algorithm>
#include <stdexcept>

namespace {

using bomberman::ConfiguracaoFase;
using bomberman::EstadoPartida;
using bomberman::EventoPartida;
using bomberman::Partida;
using bomberman::TipoMelhoria;

ConfiguracaoFase faseAberta() {
    ConfiguracaoFase fase;
    fase.linhas = 5;
    fase.colunas = 7;
    fase.inicioJogador = {2, 1};
    fase.saida = {4, 6};
    return fase;
}

ConfiguracaoFase faseComMelhoria(TipoMelhoria tipo) {
    auto fase = faseAberta();
    fase.blocos.push_back({{2, 3}, TipoBloco::Destrutivel, tipo});
    return fase;
}

// O percurso usa somente comandos publicos e permite observar a revelacao
// antes da coleta. A celula (1,0) fica fora da cruz da bomba em (2,1).
void revelarMelhoria(Partida& partida) {
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::milliseconds(3600));
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    REQUIRE(partida.obterMapa().obterBloco({2, 3}) == nullptr);
    REQUIRE(partida.obterMapa().obterMelhoria({2, 3}) != nullptr);
    REQUIRE(partida.obterQuantidadeExplosoes() == 0);
}

void coletarMelhoriaRevelada(Partida& partida) {
    REQUIRE(partida.moverJogador({0, 1}));
    REQUIRE(partida.moverJogador({0, 1}));
    REQUIRE(partida.moverJogador({0, 1}));
    REQUIRE(partida.moverJogador({1, 0}));
    REQUIRE(partida.obterMapa().obterMelhoria({2, 3}) == nullptr);
}

} // namespace

TEST_CASE("Partida sem inicio nao oferece entidades nem aceita acoes") {
    Partida partida;
    CHECK(partida.obterEstado() == EstadoPartida::NaoIniciada);
    CHECK_FALSE(partida.todosInimigosEliminados());
    CHECK(partida.obterInimigos().empty());
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK_THROWS_AS(partida.obterMapa(), std::logic_error);
    CHECK_THROWS_AS(partida.obterJogador(), std::logic_error);
    CHECK_FALSE(partida.moverJogador({0, 1}));
    CHECK_FALSE(partida.colocarBomba());
    partida.atualizar(sf::seconds(1));
    CHECK(partida.obterEstado() == EstadoPartida::NaoIniciada);
}

TEST_CASE("Partida padrao cria mapa jogador e inimigos reais em celulas livres") {
    Partida partida;
    partida.iniciarFase();
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    const auto& mapa = partida.obterMapa();
    const auto& jogador = partida.obterJogador();
    conferirPosicao(jogador.obterPosicao(), mapa.obterInicioJogador());
    CHECK(jogador.ativo());
    CHECK(mapa.podeEntrar(jogador.obterPosicao()));
    REQUIRE_FALSE(partida.obterInimigos().empty());
    CHECK_FALSE(partida.todosInimigosEliminados());
    for (const auto& inimigo : partida.obterInimigos()) {
        CHECK(inimigo.ativo());
        CHECK(mapa.podeEntrar(inimigo.obterPosicao()));
        const Posicao posicao = inimigo.obterPosicao();
        const Posicao inicio = jogador.obterPosicao();
        CHECK_FALSE((posicao.linha == inicio.linha && posicao.coluna == inicio.coluna));
    }
    bool temParede = false;
    bool temDestrutivel = false;
    for (int linha = 0; linha < mapa.obterLinhas(); ++linha) {
        for (int coluna = 0; coluna < mapa.obterColunas(); ++coluna) {
            const auto* bloco = mapa.obterBloco({linha, coluna});
            if (bloco != nullptr) {
                temParede = temParede || bloco->obterTipo() == TipoBloco::Indestrutivel;
                temDestrutivel = temDestrutivel || bloco->obterTipo() == TipoBloco::Destrutivel;
            }
        }
    }
    CHECK(temParede);
    CHECK(temDestrutivel);
}

TEST_CASE("Partida valida a configuracao antes de iniciar") {
    auto fase = faseAberta();
    SUBCASE("dimensoes invalidas") { fase.linhas = 0; }
    SUBCASE("personagens sobrepostos") { fase.iniciosInimigos = {fase.inicioJogador}; }
    SUBCASE("saida fora da arena") { fase.saida = {5, 6}; }
    SUBCASE("melhoria de velocidade nao implementada") {
        fase.blocos.push_back({{2, 3}, TipoBloco::Destrutivel,
                              TipoMelhoria::AumentoVelocidade});
    }
    SUBCASE("melhoria em parede indestrutivel") {
        fase.blocos.push_back({{2, 3}, TipoBloco::Indestrutivel, TipoMelhoria::Escudo});
    }
    Partida partida(fase);
    CHECK_THROWS_AS(partida.iniciarFase(), std::invalid_argument);
    CHECK(partida.obterEstado() == EstadoPartida::NaoIniciada);
    CHECK(partida.obterQuantidadeBombas() == 0);
}

TEST_CASE("Partida movimenta por celulas e sincroniza limite e ocupacao de bombas") {
    auto fase = faseAberta();
    fase.blocos.push_back({{2, 2}, TipoBloco::Indestrutivel});
    Partida partida(fase);
    partida.iniciarFase();
    CHECK_FALSE(partida.moverJogador({0, 1}));
    CHECK_FALSE(partida.moverJogador({1, 1}));
    CHECK_FALSE(partida.moverJogador({0, 2}));
    CHECK_FALSE(partida.moverJogador({0, 0}));
    conferirPosicao(partida.obterJogador().obterPosicao(), {2, 1});
    REQUIRE(partida.colocarBomba());
    CHECK_FALSE(partida.colocarBomba());
    CHECK(partida.obterQuantidadeBombas() == 1);
    CHECK(partida.obterJogador().obterBombasAtivas() == 1);
    CHECK(partida.obterMapa().temBomba({2, 1}));
    CHECK(partida.obterBomba(0).getPosicao() == sf::Vector2i(1, 2));
    REQUIRE(partida.moverJogador({-1, 0}));
    CHECK_FALSE(partida.moverJogador({1, 0}));
    CHECK_FALSE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({0, -1}));
    CHECK_FALSE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::milliseconds(2990));
    CHECK(partida.obterQuantidadeBombas() == 1);
    partida.atualizar(sf::milliseconds(10));
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 1);
    CHECK_FALSE(partida.obterMapa().temBomba({2, 1}));
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    REQUIRE(partida.colocarBomba());
    CHECK(partida.obterQuantidadeBombas() == 1);
}

TEST_CASE("Partida ignora tempos invalidos e pausas longas sem adiantar bombas") {
    Partida partida(faseAberta());
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::Time::Zero);
    partida.atualizar(sf::seconds(-1));
    partida.atualizar(sf::seconds(6));
    partida.atualizar(sf::milliseconds(2990));
    CHECK(partida.obterQuantidadeBombas() == 1);
    partida.atualizar(sf::milliseconds(10));
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
}

TEST_CASE("Partida aplica dano mesmo quando um quadro abrange toda a explosao") {
    Partida partida(faseAberta());
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba());
    partida.atualizar(sf::seconds(4));
    REQUIRE(partida.obterEstado() == EstadoPartida::Derrota);
    CHECK_FALSE(partida.obterJogador().ativo());
    const auto explosoes = partida.obterQuantidadeExplosoes();
    CHECK_FALSE(partida.moverJogador({-1, 0}));
    CHECK_FALSE(partida.colocarBomba());
    partida.atualizar(sf::seconds(1));
    CHECK(partida.obterQuantidadeExplosoes() == explosoes);
    conferirPosicao(partida.obterJogador().obterPosicao(), {2, 1});
    partida.reiniciarFase();
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterJogador().ativo());
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
}

TEST_CASE("Partida derrota ao entrar na celula de um inimigo") {
    auto fase = faseAberta();
    fase.iniciosInimigos = {{2, 2}};
    Partida partida(fase);
    partida.iniciarFase();
    REQUIRE(partida.moverJogador({0, 1}));
    CHECK(partida.obterEstado() == EstadoPartida::Derrota);
    CHECK_FALSE(partida.obterJogador().ativo());
}

TEST_CASE("Partida detecta contato durante quadro longo e congela no primeiro encontro") {
    auto fase = faseAberta();
    fase.linhas = 1;
    fase.colunas = 2;
    fase.inicioJogador = {0, 0};
    fase.iniciosInimigos = {{0, 1}};
    fase.saida = {0, 1};
    Partida partida(fase);
    partida.iniciarFase();
    // O inimigo so tem uma direcao. Apos 500 ms encontra o jogador;
    // processar todos os 5 s antes do contato ocultaria esse encontro.
    partida.atualizar(sf::seconds(5));
    CHECK(partida.obterEstado() == EstadoPartida::Derrota);
    CHECK_FALSE(partida.obterJogador().ativo());
    conferirPosicao(partida.obterInimigos().front().obterPosicao(), {0, 0});
    partida.atualizar(sf::seconds(1));
    conferirPosicao(partida.obterInimigos().front().obterPosicao(), {0, 0});
}

TEST_CASE("Partida revela coleta e remove cada melhoria obrigatoria") {
    auto tipo = TipoMelhoria::AumentoAlcance;
    SUBCASE("alcance") { tipo = TipoMelhoria::AumentoAlcance; }
    SUBCASE("bomba extra") { tipo = TipoMelhoria::BombaExtra; }
    SUBCASE("escudo") { tipo = TipoMelhoria::Escudo; }
    Partida partida(faseComMelhoria(tipo));
    partida.iniciarFase();
    revelarMelhoria(partida);
    CHECK(partida.obterMapa().obterMelhoria({2, 3})->getTipo() == tipo);
    coletarMelhoriaRevelada(partida);
    CHECK(partida.obterJogador().obterAlcance() ==
          (tipo == TipoMelhoria::AumentoAlcance ? 3 : 2));
    CHECK(partida.obterJogador().obterLimiteBombas() ==
          (tipo == TipoMelhoria::BombaExtra ? 2 : 1));
    CHECK(partida.obterJogador().temEscudo() == (tipo == TipoMelhoria::Escudo));
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({1, 0}));
    CHECK(partida.obterJogador().obterAlcance() ==
          (tipo == TipoMelhoria::AumentoAlcance ? 3 : 2));
    CHECK(partida.obterJogador().obterLimiteBombas() ==
          (tipo == TipoMelhoria::BombaExtra ? 2 : 1));
}

TEST_CASE("Partida detona a cadeia imediatamente antes do segundo temporizador") {
    Partida partida(faseComMelhoria(TipoMelhoria::BombaExtra));
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba()); // (2,3), primeira bomba.
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::seconds(1));
    REQUIRE(partida.colocarBomba()); // (2,2), um segundo mais nova.
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::seconds(2));
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterQuantidadeBombas() == 0);
    REQUIRE(partida.obterQuantidadeExplosoes() == 2);
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    CHECK_FALSE(partida.obterMapa().temBomba({2, 3}));
    CHECK_FALSE(partida.obterMapa().temBomba({2, 2}));
    bool atingiuColunaDaSegundaBomba = false;
    for (std::size_t i = 0; i < partida.obterQuantidadeExplosoes(); ++i) {
        const auto& celulas = partida.obterExplosao(i).getCelulasAtingidas();
        atingiuColunaDaSegundaBomba = atingiuColunaDaSegundaBomba ||
            std::find(celulas.begin(), celulas.end(), sf::Vector2i(2, 0)) != celulas.end();
    }
    CHECK(atingiuColunaDaSegundaBomba);
}

TEST_CASE("Partida preserva escudo por toda a explosao e perde na seguinte") {
    Partida partida(faseComMelhoria(TipoMelhoria::Escudo));
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba());
    partida.atualizar(sf::milliseconds(3600));
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterJogador().ativo());
    CHECK_FALSE(partida.obterJogador().temEscudo());
    REQUIRE(partida.colocarBomba());
    partida.atualizar(sf::seconds(3));
    CHECK(partida.obterEstado() == EstadoPartida::Derrota);
}

TEST_CASE("Partida detona bomba colocada no fogo no proprio comando") {
    Partida partida(faseComMelhoria(TipoMelhoria::Escudo));
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba());
    partida.atualizar(sf::seconds(3));
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    REQUIRE(partida.obterQuantidadeExplosoes() == 1);
    REQUIRE_FALSE(partida.obterJogador().temEscudo());
    REQUIRE(partida.colocarBomba());
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 2);
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    CHECK_FALSE(partida.obterMapa().temBomba({2, 3}));
    CHECK(partida.obterEstado() == EstadoPartida::Derrota);
}

TEST_CASE("Partida exige eliminar inimigos e chegar a saida para vencer") {
    auto fase = faseAberta();
    fase.inicioJogador = {0, 1};
    fase.iniciosInimigos = {{0, 0}};
    fase.blocos = {{{1, 0}, TipoBloco::Indestrutivel}};
    fase.saida = {1, 2};
    Partida partida(fase);
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba()); // Fecha a unica passagem do inimigo.
    REQUIRE(partida.moverJogador({0, 1}));
    REQUIRE(partida.moverJogador({1, 0}));
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento); // Saida com inimigo vivo.
    SUBCASE("jogador chega na saida depois da eliminacao") {
        REQUIRE(partida.moverJogador({1, 0}));
        partida.atualizar(sf::seconds(3));
        REQUIRE(partida.todosInimigosEliminados());
        CHECK_FALSE(partida.obterInimigos().front().ativo());
        CHECK(partida.obterEstado() == EstadoPartida::EmAndamento); // Fora da saida.
        REQUIRE(partida.moverJogador({-1, 0}));
    }
    SUBCASE("jogador espera na saida pela eliminacao") {
        partida.atualizar(sf::seconds(3));
        REQUIRE(partida.todosInimigosEliminados());
    }
    CHECK(partida.obterEstado() == EstadoPartida::Vitoria);
    CHECK_FALSE(partida.moverJogador({1, 0}));
    CHECK_FALSE(partida.colocarBomba());
    partida.reiniciarFase();
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterJogador().ativo());
    CHECK(partida.obterInimigos().front().ativo());
    conferirPosicao(partida.obterInimigos().front().obterPosicao(), {0, 0});
    CHECK_FALSE(partida.todosInimigosEliminados());
}

TEST_CASE("Partida prioriza derrota se ultimo inimigo e jogador morrem na saida") {
    auto fase = faseAberta();
    fase.inicioJogador = {0, 1};
    fase.iniciosInimigos = {{0, 0}};
    fase.blocos = {{{1, 0}, TipoBloco::Indestrutivel}};
    fase.saida = fase.inicioJogador;
    Partida partida(fase);
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba());
    partida.atualizar(sf::seconds(3));
    CHECK(partida.todosInimigosEliminados());
    CHECK_FALSE(partida.obterJogador().ativo());
    CHECK(partida.obterEstado() == EstadoPartida::Derrota);
    partida.reiniciarFase();
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterJogador().ativo());
    CHECK(partida.obterInimigos().front().ativo());
    CHECK_FALSE(partida.todosInimigosEliminados());
    CHECK(partida.obterQuantidadeExplosoes() == 0);
}

TEST_CASE("Partida sem inimigos inicia vitoriosa somente se jogador estiver na saida") {
    auto fase = faseAberta();
    fase.saida = fase.inicioJogador;
    Partida partida(fase);
    partida.iniciarFase();
    CHECK(partida.todosInimigosEliminados());
    CHECK(partida.obterEstado() == EstadoPartida::Vitoria);
}

TEST_CASE("Porta concluida permite novo mapa sem bombas fogo itens ou eventos anteriores") {
    auto fase = faseComMelhoria(TipoMelhoria::BombaExtra);
    fase.saida = {4, 0};
    ConfiguracaoFase segunda;
    segunda.iniciosInimigos = {{1, 9}, {7, 1}, {7, 9}};
    Partida partida(std::vector<ConfiguracaoFase>{fase, segunda});
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::seconds(3));
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({0, -1}));
    REQUIRE(partida.moverJogador({1, 0}));
    REQUIRE(partida.moverJogador({1, 0}));
    REQUIRE(partida.moverJogador({1, 0}));
    REQUIRE(partida.obterEstado() == EstadoPartida::Vitoria);
    REQUIRE(partida.obterQuantidadeBombas() == 1);
    REQUIRE(partida.obterQuantidadeExplosoes() == 1);
    REQUIRE(partida.obterJogador().obterLimiteBombas() == 2);

    REQUIRE(partida.avancarFase());
    CHECK(partida.obterNumeroFase() == 2);
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterMapa().obterLinhas() == 9);
    CHECK(partida.obterMapa().obterColunas() == 11);
    conferirPosicao(partida.obterJogador().obterPosicao(), partida.obterMapa().obterInicioJogador());
    CHECK(partida.obterJogador().ativo());
    CHECK(partida.obterJogador().obterLimiteBombas() == 1);
    CHECK(partida.obterJogador().obterAlcance() == 2);
    CHECK_FALSE(partida.obterJogador().temEscudo());
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK(partida.obterInimigos().size() == 3);
    CHECK_FALSE(partida.todosInimigosEliminados());
    CHECK(partida.consumirEventos().empty());
    const auto saida = partida.obterMapa().obterSaida();
    partida.reiniciarFase();
    CHECK(partida.obterNumeroFase() == 2);
    conferirPosicao(partida.obterMapa().obterSaida(), saida);
    partida.iniciarFase();
    CHECK(partida.obterNumeroFase() == 1);
    conferirPosicao(partida.obterMapa().obterSaida(), fase.saida);
}

TEST_CASE("Reinicio restaura terreno melhorias atributos e limpa bombas e explosoes") {
    Partida partida(faseComMelhoria(TipoMelhoria::BombaExtra));
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba()); // (2,3).
    REQUIRE(partida.moverJogador({0, -1}));
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::seconds(3));
    REQUIRE(partida.obterQuantidadeExplosoes() == 1);
    REQUIRE(partida.colocarBomba()); // (1,1), fora da explosao anterior.
    REQUIRE(partida.obterQuantidadeBombas() == 1);
    partida.reiniciarFase();
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    CHECK(partida.obterJogador().obterLimiteBombas() == 1);
    CHECK(partida.obterJogador().obterAlcance() == 2);
    CHECK_FALSE(partida.obterJogador().temEscudo());
    CHECK(partida.obterJogador().ativo());
    conferirPosicao(partida.obterJogador().obterPosicao(), {2, 1});
    CHECK(partida.obterMapa().obterBloco({2, 3}) != nullptr);
    CHECK(partida.obterMapa().obterMelhoria({2, 3}) == nullptr);
    CHECK_FALSE(partida.obterMapa().temBomba({1, 1}));
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    CHECK(partida.obterJogador().obterLimiteBombas() == 2);
}

TEST_CASE("Partida encerrada congela e pode iniciar novamente") {
    Partida partida(faseAberta());
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba());
    partida.encerrarFase();
    CHECK(partida.obterEstado() == EstadoPartida::Encerrada);
    CHECK_FALSE(partida.moverJogador({0, 1}));
    CHECK_FALSE(partida.colocarBomba());
    partida.atualizar(sf::seconds(4));
    CHECK(partida.obterQuantidadeBombas() == 1);
    partida.iniciarFase();
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
}

TEST_CASE("Eventos registram somente movimentos e bombas aceitos e sao consumidos uma vez") {
    auto fase = faseAberta();
    fase.blocos.push_back({{2, 2}, TipoBloco::Indestrutivel});
    Partida partida(fase);
    CHECK_FALSE(partida.moverJogador({0, 1}));
    CHECK_FALSE(partida.colocarBomba());
    CHECK(partida.consumirEventos().empty());
    partida.iniciarFase();
    CHECK(partida.consumirEventos().empty());
    CHECK_FALSE(partida.moverJogador({0, 1})); // Parede.
    CHECK_FALSE(partida.moverJogador({1, 1})); // Diagonal.
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.colocarBomba());
    CHECK_FALSE(partida.colocarBomba()); // Celula ocupada e limite atingido.
    REQUIRE(partida.moverJogador({0, 1}));
    CHECK_FALSE(partida.colocarBomba()); // Limite atingido em celula livre.
    CHECK_FALSE(partida.moverJogador({1, 0})); // Mesma parede, por outro lado.
    CHECK((partida.consumirEventos() == std::vector<EventoPartida>{
        EventoPartida::PassoVertical, EventoPartida::BombaColocada,
        EventoPartida::PassoHorizontal}));
    CHECK(partida.consumirEventos().empty());
}

TEST_CASE("Evento de detonacao permanece mesmo se a explosao expirar no mesmo quadro") {
    Partida partida(faseAberta());
    partida.iniciarFase();
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.consumirEventos();
    partida.atualizar(sf::milliseconds(3600));
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK((partida.consumirEventos() == std::vector<EventoPartida>{EventoPartida::BombaExplodiu}));
    partida.atualizar(sf::seconds(1));
    CHECK(partida.consumirEventos().empty());
}

TEST_CASE("Reacao em cadeia emite uma detonacao para cada bomba mesmo em quadro longo") {
    Partida partida(faseComMelhoria(TipoMelhoria::BombaExtra));
    partida.iniciarFase();
    revelarMelhoria(partida);
    coletarMelhoriaRevelada(partida);
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({0, -1}));
    partida.atualizar(sf::seconds(1));
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({0, -1}));
    partida.consumirEventos();
    partida.atualizar(sf::milliseconds(2600));
    REQUIRE(partida.obterEstado() == EstadoPartida::EmAndamento);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK((partida.consumirEventos() == std::vector<EventoPartida>{
        EventoPartida::BombaExplodiu, EventoPartida::BombaExplodiu}));
}

TEST_CASE("Cada melhoria emite coleta uma unica vez e somente ao entrar na sua celula") {
    auto tipo = TipoMelhoria::AumentoAlcance;
    SUBCASE("alcance") { tipo = TipoMelhoria::AumentoAlcance; }
    SUBCASE("bomba extra") { tipo = TipoMelhoria::BombaExtra; }
    SUBCASE("escudo") { tipo = TipoMelhoria::Escudo; }
    Partida partida(faseComMelhoria(tipo));
    partida.iniciarFase();
    revelarMelhoria(partida);
    auto eventos = partida.consumirEventos();
    CHECK(std::count(eventos.begin(), eventos.end(), EventoPartida::MelhoriaColetada) == 0);
    coletarMelhoriaRevelada(partida);
    eventos = partida.consumirEventos();
    REQUIRE_FALSE(eventos.empty());
    CHECK(eventos.back() == EventoPartida::MelhoriaColetada);
    CHECK(std::count(eventos.begin(), eventos.end(), EventoPartida::MelhoriaColetada) == 1);
    REQUIRE(partida.moverJogador({-1, 0}));
    REQUIRE(partida.moverJogador({1, 0}));
    eventos = partida.consumirEventos();
    CHECK(std::count(eventos.begin(), eventos.end(), EventoPartida::MelhoriaColetada) == 0);
}

TEST_CASE("Derrota emite um unico evento por contato ou explosao") {
    auto fase = faseAberta();
    fase.iniciosInimigos = {{2, 2}};
    Partida partida(fase);
    partida.iniciarFase();
    SUBCASE("contato") {
        REQUIRE(partida.moverJogador({0, 1}));
    }
    SUBCASE("explosao") {
        REQUIRE(partida.colocarBomba());
        // Evita que o movimento aleatorio do inimigo determine a causa da derrota.
        // A bomba bloqueia sua entrada na celula do jogador.
        partida.consumirEventos();
        partida.atualizar(sf::seconds(3));
    }
    REQUIRE(partida.obterEstado() == EstadoPartida::Derrota);
    auto eventos = partida.consumirEventos();
    CHECK(std::count(eventos.begin(), eventos.end(), EventoPartida::Derrota) == 1);
    CHECK_FALSE(partida.moverJogador({-1, 0}));
    CHECK_FALSE(partida.colocarBomba());
    partida.atualizar(sf::seconds(4));
    CHECK(partida.consumirEventos().empty());
}

TEST_CASE("Inicio reinicio e encerramento descartam eventos da tentativa anterior") {
    Partida partida(faseAberta());
    partida.iniciarFase();
    REQUIRE(partida.moverJogador({0, 1}));
    REQUIRE(partida.colocarBomba());
    SUBCASE("inicio") { partida.iniciarFase(); }
    SUBCASE("reinicio") { partida.reiniciarFase(); }
    SUBCASE("encerramento") { partida.encerrarFase(); }
    CHECK(partida.consumirEventos().empty());
}
