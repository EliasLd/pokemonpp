#include "Game.h"
#include "DataReader.h"
#include "StartMenu.h"
#include "Introduction.h"
#include "SelectionMenu.h"
#include "MainMenu.h"
#include "Trainer.h"
#include "EndMenu.h"
#include "FightUtils.h"

#include "ftxui/component/screen_interactive.hpp"

const std::string pokemon_filename  {"../data/pokemon.csv"};
const std::string leader_filename   {"../data/leaders.csv"};
const std::string master_filename   {"../data/maitres.csv"};

void resetToGameStart(
    std::unordered_map<std::string, std::shared_ptr<Pokemon>>& pokemons,
    std::vector<GymLeader>& leaders,
    std::vector<Master>& masters,
    Player& player
) {
    std::vector<std::shared_ptr<Pokemon>> p;
    p.reserve(pokemons.size());
    for (const auto& pair : pokemons) {
        p.push_back(pair.second);
    }
    for (auto& leader : leaders) {
        for (const auto& pok : leader.getPokemons()) {
            p.push_back(pok);
        }
        leader.setDefeated(false);
    }
    for (auto& master : masters) {
        for (const auto& pok : master.getPokemons()) {
            p.push_back(pok);
        }
        master.setDefeated(false);
    }
    resetPokemonHp(p);
    player.setNbPotions(default_potions);
    player.setWins(0);
    player.setDefeats(0);
    player.setBadges(0);
}

void runGame()
{
    GameState state { GameState::StartMenu };

    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemons {readPokemonFromCSV(pokemon_filename)};
    std::vector<GymLeader> leaders {readGymLeadersFromCSV(leader_filename, pokemons)};
    std::vector<Master> masters {readMasterFromCSV(master_filename, pokemons)};
    
    Player player {};
    std::vector<std::shared_ptr<Pokemon>> player_pokemons {};
    std::string player_name {};
    bool victory {};
    
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
            victory = mainMenu(screen, state, player, leaders, masters);
            break;

        case GameState::Exit:
            break;

        case GameState::EndMenu:
            state = EndMenu(screen, victory);
            resetToGameStart(pokemons, leaders, masters, player);
            break;
        
        default:
            throw std::runtime_error("Unreachable");
            break;
        }   
    }

}