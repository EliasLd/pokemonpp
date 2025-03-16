#include "Pokemon.h"
#include "Trainer.h"

#include <fstream>
#include <sstream>
#include <algorithm>

// Base Constructor
Trainer::Trainer(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : name { name }
    , pokemons { std::move(pokemons) } {}

// Base Accessors
const std::vector<std::shared_ptr<Pokemon>>& Trainer::getPokemons() const { return pokemons; }
const std::string& Trainer::getName() const { return name;}

// Player definition

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

void Player::setName(const std::string& new_name) { name = new_name; }
void Player::setPokemons(const std::vector<std::shared_ptr<Pokemon>>& new_pokemons) { pokemons = new_pokemons; }

int Player::getBadges()     const   { return badges; }
int Player::getWins()       const   { return wins; }
int Player::getDefeats()    const   { return defeats; }

void Player::swapPokemons(int index1, int index2) {
    std::swap(pokemons[index1], pokemons[index2]);
}

// GymLeader definition

GymLeader::GymLeader(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons,
                    const std::string& gym_name, const std::string& badge, int badges_condition)
    : Trainer(name, pokemons)
    , gym_name { gym_name }
    , badge { badge }
    , badges_condition { badges_condition } {}

const std::string GymLeader::toString() const
{
    std::string s {};
    s += "Gym leader " + name + " - ";
    s += gym_name + " - Badge: " + badge;
    return s;
}

const std::string& GymLeader::getGymName() const { return gym_name; }
int GymLeader::getBadgesCondition() const { return badges_condition; }
bool GymLeader::isDefeated() const { return defeated; }

// Master definition

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

                std::shared_ptr<Pokemon> pokemon { pokemon_map[pokemon_token] };

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