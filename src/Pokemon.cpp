#include "Pokemon.h"
#include "DataReader.h"
#include "TypeStats.h"

#include <iomanip>
#include <unordered_set>

Pokemon::Pokemon(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : name {name}
    , english_name {english_name}
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

const std::string& Pokemon::getEnglishName() const {
    return english_name;
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

PokemonFeu::PokemonFeu(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonFeu::clone() const {
    return std::make_shared<PokemonFeu>(*this);
}

const std::string PokemonFeu::interactWith() const {
    return name + " crépite d'énergie et fait jaillir quelques étincelles autour de lui !";
}

PokemonEau::PokemonEau(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonEau::clone() const {
    return std::make_shared<PokemonEau>(*this);
}

const std::string PokemonEau::interactWith() const {
    return name + " clapote joyeusement et projette quelques gouttes d'eau dans les airs !";
}

PokemonPlante::PokemonPlante(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPlante::clone() const {
    return std::make_shared<PokemonPlante>(*this);
}

const std::string PokemonPlante::interactWith() const {
    return name + " agite ses feuilles en captant la lumière du soleil avec enthousiasme !";
}

PokemonSol::PokemonSol(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonSol::clone() const {
    return std::make_shared<PokemonSol>(*this);
}

const std::string PokemonSol::interactWith() const {
    return name + " gratte le sol et soulève un petit nuage de poussière en signe d'excitation !";
}

PokemonElectrik::PokemonElectrik(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonElectrik::clone() const {
    return std::make_shared<PokemonElectrik>(*this);
}

const std::string PokemonElectrik::interactWith() const {
    return name + " émet de petits arcs électriques et fait crépiter l'air autour de lui !";
}

PokemonPoison::PokemonPoison(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPoison::clone() const {
    return std::make_shared<PokemonPoison>(*this);
}

const std::string PokemonPoison::interactWith() const {
    return name + " libère un léger nuage toxique et te fixe avec un regard malicieux !";
}

PokemonPsy::PokemonPsy(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonPsy::clone() const {
    return std::make_shared<PokemonPsy>(*this);
}

const std::string PokemonPsy::interactWith() const {
    return name + " ferme les yeux et semble lire dans tes pensées... Troublant !";
}

PokemonCombat::PokemonCombat(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonCombat::clone() const {
    return std::make_shared<PokemonCombat>(*this);
}

const std::string PokemonCombat::interactWith() const {
    return name + " s'échauffe en exécutant quelques coups rapides dans le vide !";
}

PokemonDragon::PokemonDragon(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonDragon::clone() const {
    return std::make_shared<PokemonDragon>(*this);
}

const std::string PokemonDragon::interactWith() const {
    return name + " rugit fièrement, faisant trembler l'air d'une puissance ancestrale !";
}

PokemonVol::PokemonVol(const std::string& name, const std::string& english_name, const std::string& type1, const std::string& type2, int base_hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, english_name, type1, type2, base_hp, attack_name, attack_damage) 
    {}

std::shared_ptr<Pokemon> PokemonVol::clone() const {
    return std::make_shared<PokemonVol>(*this);
}

const std::string PokemonVol::interactWith() const {
    return name + " bat des ailes avec grâce et tournoie dans les airs avant de se poser en douceur !";
}
    
std::shared_ptr<Pokemon> createPokemon(
    const std::string& name, 
    const std::string& english_name,
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
        return std::make_shared<PokemonFeu>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Eau || r_type2 == HandledTypes::Eau) 
        return std::make_shared<PokemonEau>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Plante || r_type2 == HandledTypes::Plante)
        return std::make_shared<PokemonPlante>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Sol || r_type2 == HandledTypes::Sol)
        return std::make_shared<PokemonSol>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Electrik || r_type2 == HandledTypes::Electrik)
        return std::make_shared<PokemonElectrik>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Poison || r_type2 == HandledTypes::Poison)
        return std::make_shared<PokemonPoison>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Psy || r_type2 == HandledTypes::Psy)
        return std::make_shared<PokemonPsy>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Combat || r_type2 == HandledTypes::Combat)
        return std::make_shared<PokemonCombat>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Dragon || r_type2 == HandledTypes::Dragon)
        return std::make_shared<PokemonDragon>(name, english_name, type1, type2, base_hp, attackName, attackDamage);
    if (r_type1 == HandledTypes::Vol || r_type2 == HandledTypes::Vol)
        return std::make_shared<PokemonVol>(name, english_name, type1, type2, base_hp, attackName, attackDamage);

    std::cerr << "The type of the pokemon " << name << " is not handled yet." << std::endl;
    return nullptr;
}
    
