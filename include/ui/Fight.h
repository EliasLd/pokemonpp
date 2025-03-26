#ifndef POKEMON_FIGHT_U
#define POKEMON_FIGHT_U

#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

ftxui::Component fightHeader(const Player& player, const Trainer& opponent);
void opponentTurn(bool& is_player_turn, std::vector<std::shared_ptr<Pokemon>>& player_pokemons, 
    int& player_index, std::shared_ptr<Pokemon>& opponent_pokemon, 
    Player& player, Trainer& opponent, 
    std::vector<ftxui::Element>& logs, ftxui::ScreenInteractive& screen);

void Fight(ftxui::ScreenInteractive& screen, Player& player, Trainer& opponent);

#endif