#include "Pokemon.h"
#include "Trainer.h"

#include <fstream>
#include <sstream>

Trainer::Trainer(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : name { name }
    , pokemons { std::move(pokemons) } {}

Player::Player(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : Trainer(name, pokemons) {}

const std::string Player::toString() const 
{
    std::string s {};
    s += name + " - " + std::to_string(badges) + " badge(s) - ";
    s += std::to_string(wins) + " win(s) - ";
    s += std::to_string(defeats) + " defeat(s)";
    return s;
}

GymLeader::GymLeader(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons,
                    const std::string& gym_name, const std::string& badge)
    : Trainer(name, pokemons)
    , gym_name { gym_name }
    , badge { badge } {}

const std::string GymLeader::toString() const
{
    std::string s {};
    s += "Gym leader " + name + " - ";
    s += gym_name + " - Badge: " + badge;
    return s;
}

Master::Master(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : Trainer(name, pokemons) {}

const std::string Master::toString() const 
{
    std::string s {};
    s += "Pokemon master " + name;
    s += " (All Pokemon deal +25% damages)";
    return s;
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
        std::string name {}, gym_name {}, badge {};
        std::vector<std::shared_ptr<Pokemon>> pokemons {};
        std::string pokemon_token {};

        std::getline(ss, name, ',');
        std::getline(ss, gym_name, ',');
        std::getline(ss, badge, ',');

        // Read pokemons (up to 6 tokens)
        while(std::getline(ss, pokemon_token, ',')) {

            if(!pokemon_token.empty()) {

                std::shared_ptr<Pokemon> pokemon { pokemon_map[pokemon_token] };

                if(pokemon)
                    pokemons.push_back(pokemon);
                else
                    std::cerr << "Error: cannot find Pokemon " << pokemon_token << std::endl;

            }
        }

        GymLeader leader {name, pokemons, gym_name, badge};
        leaders.push_back(leader);

    }

    f.close();
    return leaders;

}