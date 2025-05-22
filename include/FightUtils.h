#ifndef FIGHT_UTIL_H
#define FIGHT_UTIL_H

#include "Trainer.h"

bool isOpponentMaster(Trainer& opponent);
void updatePokemonIndex(int& index, std::vector<std::shared_ptr<Pokemon>>& pokemon_list);
bool allPokemonsKO(const std::vector<std::shared_ptr<Pokemon>>& pokemon_list);
void resetPokemonHp(const std::vector<std::shared_ptr<Pokemon>>& pokemon_list);
float getDamagesMultiplicator(std::shared_ptr<Pokemon>& src, std::shared_ptr<Pokemon>& target);
bool defeatedAllGym(const std::vector<GymLeader>& leaders);
bool defeatedAllMasters(const std::vector<Master>& masters);
bool isVictory(const std::vector<GymLeader>& leaders, const std::vector<Master>& masters);
bool isDefeat(const Player& player);

#endif
