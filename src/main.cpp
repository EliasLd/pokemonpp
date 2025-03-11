#include "PokemonReader.h"
#include <iostream>

int main() {

    std::string filename = "../data/pokemon.csv"; 
    auto pokemons = readPokemonFromCSV(filename);

    for (const auto& pokemon : pokemons) {
        pokemon->display();
    }

    return 0;
}
