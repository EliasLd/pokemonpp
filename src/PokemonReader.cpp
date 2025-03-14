#include "PokemonReader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

HandledTypes getHandledType(const std::string& type) {
    
    if (type == "Feu")          return HandledTypes::Feu;
    if (type == "Eau")          return HandledTypes::Eau;
    if (type == "Plante")       return HandledTypes::Plante;
    
    // Not implemented yet
    return HandledTypes::Undefined;
}

std::unordered_map<std::string, std::shared_ptr<Pokemon>> readPokemonFromCSV(const std::string& filename) 
{
    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemons;
    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return pokemons;
    }

    std::string line;
    // Ignore the first line
    std::getline(f, line);

    while (std::getline(f, line)) {

        // Create an input flow to read strings
        std::istringstream flow { line } ;

        std::string name {}, type1 {}, type2 {}, attackName {};
        int hp {}; 
        int attackDamage {};

        std::getline(flow, name, ',');
        std::getline(flow, type1, ',');
        std::getline(flow, type2, ','); 

        // Check if the type read from the file is handled
        HandledTypes r_type1 { getHandledType(type1) };
        HandledTypes r_type2 { getHandledType(type2) };
        if (r_type1 == HandledTypes::Undefined && r_type2 == HandledTypes::Undefined) {
            // if not, skip this one
            continue; 
        } 

        flow >> hp;
        // Ignore the comma after hp field 
        // (Not ignored by default with operator>>)
        flow.ignore(1, ','); 

        std::getline(flow, attackName, ',');

        flow >> attackDamage;

        if (flow.fail()) {
            std::cerr << "Error while reading this line (ignored) : " << line << std::endl;
            continue;
        }

        auto pokemon { createPokemon(name, type1, type2, hp, attackName, attackDamage) };
        pokemons[name] = pokemon;
    }

    f.close();
    return pokemons;
}
