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
    : Trainer(name, pokemons) 
{}

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
void Player::setNbPotions(int new_nb_potions) { nb_potions = new_nb_potions; }
void Player::setWins(int new_wins) { wins = new_wins; }
void Player::setDefeats(int new_defeats) { defeats = new_defeats; }
void Player::setBadges(int new_badges) { badges = new_badges; }

int Player::getBadges()     const   { return badges; }
int Player::getWins()       const   { return wins; }
int Player::getDefeats()    const   { return defeats; }
int Player::getNbPotions()  const   { return nb_potions; }

void Player::swapPokemons(int index1, int index2) {
    std::swap(pokemons.at(index1), pokemons.at(index2));
}

void Player::Defeated() { setDefeats(getDefeats() + 1); }

const std::string Player::interactWith(const std::shared_ptr<Interact>& target) {
    return target->interactWith();
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
void GymLeader::Defeated() { defeated = true; }

const std::string GymLeader::interactWith() const {
    return name + ": Félicitations ! Tu as prouvé ta force et ta détermination. Continue comme ça et tu deviendras un véritable maître Pokemon !";
}

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

bool Master::isDefeated() const { return defeated; }
void Master::Defeated() { defeated = true; }
