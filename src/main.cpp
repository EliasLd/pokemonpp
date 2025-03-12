#include "PokemonReader.h"
#include "SelectionMenu.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"

#include <iostream>

using namespace ftxui;

int main() {

    std::string filename = "../data/pokemon.csv"; 
    auto pokemons = readPokemonFromCSV(filename);

    auto screen { ScreenInteractive::Fullscreen() };

    std::vector<std::shared_ptr<Pokemon>> selected {SelectionMenu(screen, pokemons)};

    for(const auto& p: selected) {
        std::cout << p->toString() << std::endl;
    }

    return 0;
}
