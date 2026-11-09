/******************************************************************************

CRC09 - Menu

Mantem opcoes, selecao, navegacao circular e textos de instrucoes.
Retorna a opcao confirmada para as funcoes de interface em main.cpp,
que desenham as telas, leem o teclado e encaminham o inicio a Partida.
O retorno das instrucoes e o fechamento da janela pertencem a interface.
As responsabilidades e colaboracoes estao detalhadas em design/cartoes-crc.md.

******************************************************************************/

#include "Menu.hpp"

namespace bomberman {

	Menu::Menu ()
	: opcoes_({"Novo Jogo", "Instrucoes", "Sair"}), 
	  indiceSelecionado_(0), 
	  ativo_(true) {}

	void Menu::navegarAnterior () {
		//Disparado uma vez quando Up é pressionado
		indiceSelecionado_--;
		if(indiceSelecionado_<0) indiceSelecionado_ = static_cast<int>(opcoes_.size()) -1;
	}	

	void Menu::navegarProxima (){
		//Disparado uma vez quando Down é pressionado
		indiceSelecionado_++;
		if(indiceSelecionado_> static_cast<int>(opcoes_.size()) -1) indiceSelecionado_ = 0;   
	}

	OpcaoMenu Menu::confirmarSelecao() { 
		if(static_cast<int>(OpcaoMenu::NovoJogo) == indiceSelecionado_) return OpcaoMenu::NovoJogo;
		if(static_cast<int>(OpcaoMenu::Instrucoes) == indiceSelecionado_) return OpcaoMenu::Instrucoes;
		if(static_cast<int>(OpcaoMenu::Sair) == indiceSelecionado_) return OpcaoMenu::Sair;
		return OpcaoMenu::Nenhuma;
	}
	
	void Menu::atualizar (sf::Time deltaTempo) {
		(void)deltaTempo;
	}

	bool Menu::ativo() const {
		return ativo_;
	}

	int Menu::getIndiceSelecionado() const {
		return indiceSelecionado_;
	}

	const std::vector<std::string>& Menu::getOpcoes() const {
		return opcoes_;
	}

	const std::vector<SecaoInstrucao>& Menu::getInstrucoes() {
    static const std::vector<SecaoInstrucao> secoes = {
        {
            "OBJETIVO",
            {"Elimine todos os inimigos e alcance a saida da fase."}
        },
        {
            "CONTROLES",
            {
                "Setas ou W A S D - Mover o personagem",
                "Espaco - Posicionar uma bomba",
                "R - Repetir o mapa; Enter tambem apos o resultado",
                "Esc - Voltar ao menu; outra janela pausa a partida"
            }
        },
        {
            "BOMBAS E EXPLOSOES",
            {
                "A bomba explode em 3 segundos, nas quatro direcoes.",
                "Paredes e o primeiro bloco atingido interrompem o fogo.",
                "Blocos sao destruidos; bombas atingidas explodem em cadeia."
            }
        },
        {
            "MELHORIAS (escondidas nos blocos)",
            {
                "A - Alcance: aumenta o alcance da explosao",
                "B - Bomba Extra: permite mais bombas simultaneas",
                "E - Escudo: protege de uma explosao"
            }
        },
        {
            "RESULTADO",
            {
                "A saida fica verde quando todos os inimigos forem eliminados.",
                "A porta leva ao proximo mapa; conclua os quatro para vencer.",
                "Fogo sem escudo ou contato com inimigo causam derrota."
            }
        }
    };
    return secoes;
}
	
}//namespace bomberman
