#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Trainer.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"

ftxui::Component exitButton(ftxui::ScreenInteractive& screen);
ftxui::Component PlayerStats(const Player& player);
ftxui::Component leaderEntry(ftxui::ScreenInteractive& screen, const GymLeader& leader, const Player& player);
ftxui::Component Title(const Player& player, const std::vector<GymLeader>& leaders);
ftxui::Component movePokemonContainer(std::vector<std::string>& values, std::vector<std::string>& entries, Player& player, int& selected);
ftxui::Component PokemonDetails(Player& player, int& selected, std::vector<std::string>& values, std::vector<std::string>& entries);
ftxui::Component healdButton(int& selected, Player& player);

void updatePokemonsEntries(std::vector<std::string>& values, std::vector<std::string>& entries, Player& player);

void mainMenu(ftxui::ScreenInteractive& screen, GameState& state, Player& player, 
            std::vector<GymLeader>& leaders, 
            std::vector<Master>& masters);

#endif