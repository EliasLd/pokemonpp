#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

enum class GameState {
    StartMenu,
    Introduction,
    MainMenu,
    SelectionMenu,
    Credits,
    Exit,
    EndMenu,
};

void runGame();

extern const std::string pokemon_filename;
extern const std::string leader_filename;

#endif
