#ifndef BOMBERMAN_AUDIO_JOGO_HPP
#define BOMBERMAN_AUDIO_JOGO_HPP

#include "Partida.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <string>

namespace bomberman {

/**
 * @brief Reproduz os efeitos da interface sem alterar as regras da partida.
 * @note Os arquivos sao carregados uma vez. Um arquivo ausente desativa apenas
 * aquele efeito e gera um aviso no terminal. Crie somente na interface grafica.
 */
class AudioJogo {
public:
    explicit AudioJogo(const std::string& pastaSons);
    AudioJogo(const AudioJogo&) = delete;
    AudioJogo& operator=(const AudioJogo&) = delete;
    AudioJogo(AudioJogo&&) = delete;
    AudioJogo& operator=(AudioJogo&&) = delete;

    /** @brief Reproduz uma ocorrencia, ignorando comandos enquanto pausado. */
    void tocar(EventoPartida evento);
    /** @brief Pausa efeitos em andamento e toca o aviso de pausa uma vez. */
    void pausar();
    /** @brief Interrompe o aviso e retoma somente os efeitos que foram pausados. */
    void retomar();
    /** @brief Interrompe todos os efeitos ao reiniciar ou voltar ao menu. */
    void parar();

private:
    enum class Som : std::size_t {
        PassoHorizontal, PassoVertical, BombaColocada, Explosao,
        Melhoria, Derrota, DerrotaAlternativa, Pausa, Quantidade
    };
    static constexpr std::size_t QUANTIDADE_SONS = static_cast<std::size_t>(Som::Quantidade);
    static constexpr std::size_t MAXIMO_EFEITOS = 8;

    // A ordem importa: canais sao destruidos antes dos buffers que referenciam.
    std::array<std::unique_ptr<sf::SoundBuffer>, QUANTIDADE_SONS> buffers_;
    std::array<std::unique_ptr<sf::Sound>, MAXIMO_EFEITOS> efeitos_;
    std::unique_ptr<sf::Sound> passo_;
    std::unique_ptr<sf::Sound> pausa_;
    std::size_t proximoCanal_ = 0;
    bool pausado_ = false;
    bool derrotaAlternativa_ = false;

    const sf::SoundBuffer* obterBuffer(Som som) const;
    void reproduzir(std::unique_ptr<sf::Sound>& canal, Som som, float volume);
    void efeito(Som som, float volume);
};

} // namespace bomberman

#endif
