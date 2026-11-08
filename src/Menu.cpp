/******************************************************************************

CRC09 - Menu

Responsabilidades
-Apresentar o menu inicial do jogo.
-Exibir as opcoes de iniciar partida, consultar comandos e sair.
-Armazenar a opcao atualmente selecionada.
-Interpretar os comandos de navegacao e confirmacao do menu.
-Solicitar o inicio de uma partida individual quando essa opcao for confirmada.
-Apresentar os comandos do jogador antes do inicio da partida.
-Permitir o retorno da tela de comandos ao menu inicial.
-Sinalizar o pedido de encerramento do jogo ao selecionar sair.

Colaboracoes
Partida: recebe a solicitacao para iniciar uma partida individual.
Jogador: fornece a descricao dos comandos para exibicao, sem exigir uma partida 
em andamento.

******************************************************************************/

#include "Menu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

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
		//vou fazer-la em paralelo com o Render da main
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
                "Esc - Voltar ao menu principal"
            }
        },
        {
            "BOMBAS E EXPLOSOES",
            {
                "A bomba explode apos um intervalo, nas quatro direcoes.",
                "Paredes bloqueiam a explosao; blocos destrutiveis sao",
                "removidos e interrompem a propagacao.",
                "Bombas atingidas explodem na hora: reacao em cadeia!",
                "Cuidado: a explosao tambem elimina voce!"
            }
        },
        {
            "MELHORIAS (escondidas nos blocos)",
            {
                "Alcance - Aumenta o alcance da explosao",
                "Bomba Extra - Permite mais bombas simultaneas",
                "Escudo - Protege de uma explosao"
            }
        },
        {
            "DERROTA",
            {
                "Ser atingido por uma explosao sem escudo.",
                "Encostar em um inimigo (o escudo nao protege do contato!)."
            }
        }
    };
    return secoes;
}
	
}//namespace bomberman
