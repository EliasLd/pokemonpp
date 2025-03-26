#ifndef DATA_READER_H
#define DATA_READER_H

#include "Pokemon.h"
#include "Trainer.h"

#include <vector>
#include <string>
#include <unordered_map>

enum class HandledTypes {
    Feu,
    Eau,
    Plante,
    Sol,
    Electrik,
    Poison,
    Psy,
    Combat,
    Dragon,
    Vol,
    Undefined
};

HandledTypes getHandledType(const std::string& type);

std::unordered_map<std::string, std::shared_ptr<Pokemon>> readPokemonFromCSV(const std::string& filename);
std::vector<GymLeader> readGymLeadersFromCSV(const std::string& filename, std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemon_map);
std::vector<Master> readMasterFromCSV(const std::string& filename, std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemon_map);

#endif
