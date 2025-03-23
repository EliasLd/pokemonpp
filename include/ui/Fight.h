#ifndef POKEMON_FIGHT_U
#define POKEMON_FIGHT_U

#include "Game.h"
#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"

bool isOpponentMaster(Trainer& opponent);
bool allPokemonsKO(const std::vector<std::shared_ptr<Pokemon>> pokemon_list);
void Fight(ftxui::ScreenInteractive& screen, Player& player, GymLeader& opponent);

#endif