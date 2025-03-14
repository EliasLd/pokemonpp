#include "PokemonReader.h"
#include "StartMenu.h"
#include "SelectionMenu.h"
#include "GameState.h"
#include "Trainer.h"
#include "Introduction.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"

#include <iostream>
#include <unordered_map>

using namespace ftxui;

int main() 
{
    GameState state { GameState::StartMenu };

    std::vector<std::shared_ptr<Pokemon>> selected_pokemons {};
    std::string filename { "../data/pokemon.csv" };
    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemons { readPokemonFromCSV(filename) };
    std::vector<GymLeader> leaders { readGymLeadersFromCSV("../data/leaders.csv", pokemons)};
    Player player {"", selected_pokemons};
    std::string player_name {};
    
    auto screen { ScreenInteractive::Fullscreen() };

    while(state != GameState::Exit) {
        switch(state) 
        {
        case GameState::StartMenu:
            state = StartMenu(screen);
            break;

        case GameState::Introduction:
            player_name = IntroductionMenu(screen, state);
            player.setName(player_name);
            break;
        
        case GameState::SelectionMenu:
            selected_pokemons = SelectionMenu(screen, pokemons);
            player.setPokemons(selected_pokemons);

            std::cout << player.toString() << std::endl;            
            for(const auto& p: player.getPokemons())
                std::cout << p->getName() << " ";
            
            std::cout << std::endl;
            
            state = GameState::Exit;
            break;

        case GameState::Exit:
            break;

        default:
            break;
        }
    }
}
