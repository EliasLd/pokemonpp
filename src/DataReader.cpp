#include "DataReader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

HandledTypes getHandledType(const std::string& type) {
    
    if (type == "Feu")          return HandledTypes::Feu;
    if (type == "Eau")          return HandledTypes::Eau;
    if (type == "Plante")       return HandledTypes::Plante;
    if (type == "Sol")          return HandledTypes::Sol;
    if (type == "Électrik")     return HandledTypes::Electrik;
    if (type == "Poison")       return HandledTypes::Poison;
    if (type == "Psy")          return HandledTypes::Psy;
    if (type == "Combat")       return HandledTypes::Combat;
    if (type == "Dragon")       return HandledTypes::Dragon;
    if (type == "Vol")          return HandledTypes::Vol;
    
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

std::vector<GymLeader> readGymLeadersFromCSV(
    const std::string& filename,
    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemon_map)
{
    std::vector<GymLeader> leaders {};
    std::ifstream f(filename);

    if(!f.is_open()) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return leaders;
    }
    
    std::string line;
    // Ignore the first line
    std::getline(f, line);

    while(std::getline(f, line)) {

        std::stringstream ss(line);
        std::string name {}, gym_name {}, badge {}, badge_condition_str {};
        std::vector<std::shared_ptr<Pokemon>> pokemons {};
        std::string pokemon_token {};

        std::getline(ss, name, ',');
        std::getline(ss, gym_name, ',');
        std::getline(ss, badge, ',');
        std::getline(ss, badge_condition_str, ',');
        
        int badges_condition { std::stoi(badge_condition_str) };

        // Read pokemons (up to 6 tokens)
        while(std::getline(ss, pokemon_token, ',')) {

            if(!pokemon_token.empty()) {

                std::shared_ptr<Pokemon> pokemon { pokemon_map[pokemon_token]->clone() };

                if(pokemon)
                    pokemons.push_back(pokemon);
                else
                    std::cerr << "Error: cannot find Pokemon " << pokemon_token << std::endl;

            }
        }

        GymLeader leader {name, pokemons, gym_name, badge, badges_condition};
        leaders.push_back(leader);

    }

    f.close();
    return leaders;

}

std::vector<Master> readMasterFromCSV(const std::string& filename, const std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemon_map)
{
    std::vector<Master> masters {};
    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return masters;
    }
    
    std::string line;
    // Ignore the first line
    std::getline(f, line);

    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string name;
        std::vector<std::shared_ptr<Pokemon>> pokemons {};
        std::string pokemon_token;

        std::getline(ss, name, ',');

        // Read pokemons (up to 6 tokens)
        while (std::getline(ss, pokemon_token, ',')) {
            if (!pokemon_token.empty()) {
                auto it = pokemon_map.find(pokemon_token);
                if (it != pokemon_map.end()) {
                    pokemons.push_back(it->second->clone());
                } else {
                    std::cerr << "Error: cannot find Pokemon " << pokemon_token << std::endl;
                }
            }
        }

        masters.emplace_back(name, pokemons);
    }

    return masters;
}

