#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "Pokemon.h"

#include <unordered_map>

std::vector<std::shared_ptr<Pokemon>> SelectionMenu(
    ftxui::ScreenInteractive& screen, 
    const std::unordered_map<std::string, std::shared_ptr<Pokemon>>& pokemon_map);

#endif