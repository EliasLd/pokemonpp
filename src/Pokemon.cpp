#include "Pokemon.h"
#include "DataReader.h"
#include "TypeStats.h"

#include <iomanip>
#include <unordered_set>

Pokemon::Pokemon(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : name {name}
    , type1 {type1}
    , type2 {type2}
    , base_hp {base_hp}
    , attack_name {attack_name}
    , attack_damage {attack_damage} 
{
    current_hp = base_hp;
    assignWeaknessesAndResistances();
}

void addVectToVect(std::vector<std::string>& dest, const std::vector<std::string>& src) {
    // Create a set to check if an element is already in the dest vector
    // Without increasing complexity
    std::unordered_set<std::string> set(dest.begin(), dest.end());
    for(const auto& elt: src)
        if(set.find(elt) == set.end())
            dest.push_back(elt);
}

void Pokemon::assignWeaknessesAndResistances() {
    // Weaknesses
    if(TypeStats::type_weaknesses.count(type1))
        addVectToVect(weaknesses, TypeStats::type_weaknesses.at(type1));
    if(TypeStats::type_weaknesses.count(type2))
        addVectToVect(weaknesses, TypeStats::type_weaknesses.at(type2));
    // Resistances
    if(TypeStats::type_resistances.count(type1))
        addVectToVect(resistances, TypeStats::type_resistances.at(type1));
    if(TypeStats::type_resistances.count(type2))
        addVectToVect(resistances, TypeStats::type_resistances.at(type2));
}

void Pokemon::heal() {
    // Full heal
    current_hp = base_hp;
}

const std::string Pokemon::toString() const {
    std::ostringstream s;

    s << std::left << std::setw(20) << name;  
    s << std::setw(25) << ("[" + type1 + (type2.empty() ? "]" : " | " + type2 + "]")); 
    s << std::setw(10) << " - " + std::to_string(base_hp) + " hp";  

    return s.str();
}

const std::string& Pokemon::getName() const {
    return name;
}

const std::string& Pokemon::getType1() const {
    return type1;
}

const std::string& Pokemon::getType2() const {
    return type2;
}

int Pokemon::getBaseHp() const {
    return base_hp;
}

int Pokemon::getCurrentHp() const {
    return current_hp;
}

const std::string& Pokemon::getAttackName() const {
    return attack_name;
}

int Pokemon::getAttackDamage() const {
    return attack_damage;
}

void Pokemon::attack(std::shared_ptr<Pokemon> target, int damages) {
    target->takeDamage(damages);
}

void Pokemon::takeDamage(int damage) {
    current_hp -= damage;
    if (current_hp < 0) current_hp = 0;
}

const std::vector<std::string>& Pokemon::getWeaknesses() const {
    return weaknesses;
}

const std::vector<std::string>& Pokemon::getResistances() const {
    return resistances;
}

PokemonFeu::PokemonFeu(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonFeu::clone() const {
    return std::make_shared<PokemonFeu>(*this);
}

PokemonEau::PokemonEau(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonEau::clone() const {
    return std::make_shared<PokemonEau>(*this);
}

PokemonPlante::PokemonPlante(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPlante::clone() const {
    return std::make_shared<PokemonPlante>(*this);
}

PokemonSol::PokemonSol(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonSol::clone() const {
    return std::make_shared<PokemonSol>(*this);
}

PokemonElectrik::PokemonElectrik(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonElectrik::clone() const {
    return std::make_shared<PokemonElectrik>(*this);
}

PokemonPoison::PokemonPoison(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPoison::clone() const {
    return std::make_shared<PokemonPoison>(*this);
}

PokemonPsy::PokemonPsy(const std::string& name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPsy::clone() const {
    return std::make_shared<PokemonPsy>(*this);
}

    
std::shared_ptr<Pokemon> createPokemon(
    const std::string& name, 
    const std::string& type1, 
    const std::string& type2, 
    int base_hp, 
    const std::string& attackName, 
    int attackDamage
) {
    HandledTypes r_type1 { getHandledType(type1) };
    HandledTypes r_type2 { getHandledType(type2) };

    // Check if the type is handled
    if (r_type1 == HandledTypes::Feu || r_type2 == HandledTypes::Feu)
        return std::make_shared<PokemonFeu>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Eau || r_type2 == HandledTypes::Eau) 
        return std::make_shared<PokemonEau>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Plante || r_type2 == HandledTypes::Plante)
        return std::make_shared<PokemonPlante>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Sol || r_type2 == HandledTypes::Sol)
        return std::make_shared<PokemonSol>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Electrik || r_type2 == HandledTypes::Electrik)
        return std::make_shared<PokemonElectrik>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Poison || r_type2 == HandledTypes::Poison)
        return std::make_shared<PokemonPoison>(name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Psy || r_type2 == HandledTypes::Psy)
        return std::make_shared<PokemonPsy>(name, type1, type2, base_hp, attackName, attackDamage);

    std::cerr << "The type of the pokemon " << name << " is not handled yet." << std::endl;
    return nullptr;
}
    
