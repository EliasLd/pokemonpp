#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"

#include <unordered_map>
#include <vector>
#include <string>

enum class GameState {
    StartMenu,
    Introduction,
    MainMenu,
    SelectionMenu,
    Exit,
    EndMenu,
};

class GameController {
public:
    GameController();
    void runGame();
    void resetToGameStart();

private:
    GameState state { GameState::StartMenu };
    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemons;
    std::vector<GymLeader> leaders;
    std::vector<Master> masters;
    Player player {};
    ftxui::ScreenInteractive screen { ftxui::ScreenInteractive::Fullscreen() };
};

extern const std::string pokemon_filename;
extern const std::string leader_filename;

#endif
