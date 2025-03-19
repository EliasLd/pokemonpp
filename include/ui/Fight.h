#ifndef POKEMON_FIGHT_U
#define POKEMON_FIGHT_U

#include "Game.h"
#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"

bool isOpponentMaster(Trainer& opponent);
void Fight(ftxui::ScreenInteractive& screen, GameState& state, Player& player, Trainer& opponent);

#endif