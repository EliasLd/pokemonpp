#include "PokemonReader.h"
#include "StartMenu.h"
#include "SelectionMenu.h"
#include "GameState.h"

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

    auto screen { ScreenInteractive::Fullscreen() };

    while(state != GameState::Exit) {
        switch(state) 
        {
        case GameState::StartMenu:
            state = StartMenu(screen);
            break;

        case GameState::SelectionMenu:
            selected_pokemons = SelectionMenu(screen, pokemons);
            for(const auto& p: selected_pokemons)
                std::cout << p->toString() << std::endl;
            
            state = GameState::Exit;
            break;

        case GameState::Exit:
            break;

        default:
            break;
        }
    }
}
