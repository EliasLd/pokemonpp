#include "Game.h"
#include "PokemonReader.h"

#include "StartMenu.h"
#include "Introduction.h"
#include "SelectionMenu.h"
#include "MainMenu.h"

#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"

const std::string pokemon_filename  {"../data/pokemon.csv"};
const std::string leader_filename   {"../data/leaders.csv"};
const std::string master_filename   {"../data/maitres.csv"};

void runGame()
{
    GameState state { GameState::StartMenu };

    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemons {readPokemonFromCSV(pokemon_filename)};

    std::vector<GymLeader> leaders {readGymLeadersFromCSV(leader_filename, pokemons)};
    std::vector<Master> masters {};
    
    Player player {};
    std::vector<std::shared_ptr<Pokemon>> player_pokemons {};
    std::string player_name {};
    
    // Game screen (shared across all components)
    auto screen {ftxui::ScreenInteractive::Fullscreen()};

    while(state != GameState::Exit) 
    {
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
            player_pokemons = SelectionMenu(screen, pokemons);
            player.setPokemons(player_pokemons);
            
            state = GameState::MainMenu;
            break;

        case GameState::MainMenu:
            mainMenu(screen, state, player, leaders, masters);
            break;

        case GameState::Fight:
            state = GameState::MainMenu;
            break;

        case GameState::Exit:
            break;
        
        default:
            break;
        }   
    }

}