#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "Pokemon.h"

std::vector<std::shared_ptr<Pokemon>> SelectionMenu(ftxui::ScreenInteractive& screen,
                                                    std::vector<std::shared_ptr<Pokemon>>& pokemon_list);


#endif