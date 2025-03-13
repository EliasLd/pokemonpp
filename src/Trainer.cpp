#include "Pokemon.h"
#include "Trainer.h"

Trainer::Trainer(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : name { name }
    , pokemons { std::move(pokemons) } {}

Player::Player(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : Trainer(name, pokemons) {}

const std::string& Player::toString() const 
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

const std::string& GymLeader::toString() const
{
    std::string s {};
    s += "Gym leader " + name + " - ";
    s += gym_name + " - Badge:" + badge;
    return s;
}

Master::Master(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons)
    : Trainer(name, pokemons) {}

const std::string& Master::toString() const 
{
    std::string s {};
    s += "Pokemon master " + name;
    s += " (All Pokemon deal +25% damages)";
    return s;
}