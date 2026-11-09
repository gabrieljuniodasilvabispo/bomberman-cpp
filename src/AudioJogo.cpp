#include "AudioJogo.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <utility>

namespace bomberman {

AudioJogo::AudioJogo(const std::string& pastaSons) {
    const std::array<const char*, QUANTIDADE_SONS> arquivos{
        "step_horizontal.wav", "step_vertical.wav", "bomb_lay.wav",
        "bomb_explosion.wav", "powerup.wav", "bomberman_killed.wav",
        "bomberman-death-sound.wav", "pause.wav"
    };
    for (std::size_t i = 0; i < arquivos.size(); ++i) {
        const auto caminho = std::filesystem::path(pastaSons) / arquivos[i];
        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (buffer->loadFromFile(caminho)) {
            buffers_[i] = std::move(buffer);
        } else {
            std::cerr << "Aviso: efeito de som indisponivel: " << caminho.string() << '\n';
        }
    }
}

const sf::SoundBuffer* AudioJogo::obterBuffer(Som som) const {
    return buffers_[static_cast<std::size_t>(som)].get();
}

void AudioJogo::reproduzir(std::unique_ptr<sf::Sound>& canal, Som som, float volume) {
    const auto* buffer = obterBuffer(som);
    if (!buffer) return;
    if (!canal) {
        canal = std::make_unique<sf::Sound>(*buffer);
    } else {
        canal->stop();
        canal->setBuffer(*buffer);
    }
    canal->setVolume(volume);
    canal->setLooping(false);
    canal->play();
}

void AudioJogo::efeito(Som som, float volume) {
    if (!obterBuffer(som)) return;
    const auto livre = std::find_if(efeitos_.begin(), efeitos_.end(), [](const auto& canal) {
        return !canal || canal->getStatus() == sf::SoundSource::Status::Stopped;
    });
    if (livre != efeitos_.end()) {
        reproduzir(*livre, som, volume);
    } else {
        // Uma cadeia grande reutiliza canais; a quantidade de sons fica limitada.
        reproduzir(efeitos_[proximoCanal_], som, volume);
        proximoCanal_ = (proximoCanal_ + 1) % efeitos_.size();
    }
}

void AudioJogo::tocar(EventoPartida evento) {
    if (pausado_) return;
    switch (evento) {
    case EventoPartida::PassoHorizontal:
        reproduzir(passo_, Som::PassoHorizontal, 18.f);
        break;
    case EventoPartida::PassoVertical:
        reproduzir(passo_, Som::PassoVertical, 18.f);
        break;
    case EventoPartida::BombaColocada:
        efeito(Som::BombaColocada, 45.f);
        break;
    case EventoPartida::BombaExplodiu:
        efeito(Som::Explosao, 55.f);
        break;
    case EventoPartida::MelhoriaColetada:
        efeito(Som::Melhoria, 65.f);
        break;
    case EventoPartida::Derrota: {
        if (passo_) passo_->stop();
        // Os dois arquivos fornecidos sao variantes da derrota do jogador.
        // Alterna entre tentativas, usando a outra variante se uma estiver ausente.
        Som som = derrotaAlternativa_ ? Som::DerrotaAlternativa : Som::Derrota;
        if (!obterBuffer(som)) som = derrotaAlternativa_ ? Som::Derrota : Som::DerrotaAlternativa;
        derrotaAlternativa_ = !derrotaAlternativa_;
        efeito(som, 65.f);
        break;
    }
    }
}

void AudioJogo::pausar() {
    if (pausado_) return;
    pausado_ = true;
    for (auto& canal : efeitos_) {
        if (canal) canal->pause();
    }
    if (passo_) passo_->pause();
    reproduzir(pausa_, Som::Pausa, 45.f);
}

void AudioJogo::retomar() {
    if (!pausado_) return;
    pausado_ = false;
    if (pausa_) pausa_->stop();
    for (auto& canal : efeitos_) {
        if (canal && canal->getStatus() == sf::SoundSource::Status::Paused) canal->play();
    }
    if (passo_ && passo_->getStatus() == sf::SoundSource::Status::Paused) passo_->play();
}

void AudioJogo::parar() {
    for (auto& canal : efeitos_) {
        if (canal) canal->stop();
    }
    if (passo_) passo_->stop();
    if (pausa_) pausa_->stop();
    pausado_ = false;
    proximoCanal_ = 0;
}

} // namespace bomberman
