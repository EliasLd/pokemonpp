#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Trainer.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"

void mainMenu(ftxui::ScreenInteractive& screen, GameState& state, Player& player, 
            std::vector<GymLeader>& leaders, 
            std::vector<Master>& masters);

#endif