#ifndef POKEMONREADER_H
#define POKEMONREADER_H

#include "Pokemon.h"
#include <vector>
#include <string>

enum class HandledTypes {
    Feu,
    Eau,
    Plante,
    Undefined
};

HandledTypes getHandledType(const std::string& type);

std::vector<std::shared_ptr<Pokemon>> readPokemonFromCSV(const std::string& filename);

#endif
