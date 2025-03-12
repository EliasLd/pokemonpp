#include "Pokemon.h"
#include "PokemonReader.h"

Pokemon::Pokemon(const std::string& name, const std::string& type1, const std::string& type2, int hp, const std::string& attack_name, int attack_damage)
    : name {name}
    , type1 {type1}
    , type2 {type2}
    , hp {hp}
    , attack_name {attack_name}
    , attack_damage {attack_damage} {}

void Pokemon::display() const {
    std::cout   << "Pokemon: " << name 
                << " [" << type1 << (type2.empty() ? "]" : " | " + type2 + "]")
                << " - " << hp << " hp" << std::endl;
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

int Pokemon::getHp() const {
    return hp;
}

const std::string& Pokemon::getAttackName() const {
    return attack_name;
}

int Pokemon::getAtackDamage() const {
    return attack_damage;
}

void Pokemon::attack(Pokemon& target) {
    std::cout << name << " attaque " << target.getName() << " avec " << attack_name 
              << " et inflige " << attack_damage << " dégâts !" << std::endl;
    target.takeDamage(attack_damage);
}

void Pokemon::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

const std::vector<std::string>& Pokemon::getWeaknesses() const {
    return weaknesses;
}

const std::vector<std::string>& Pokemon::getResistances() const {
    return resistances;
}

PokemonFeu::PokemonFeu(const std::string& name, const std::string& type1, const std::string& type2, int hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, hp, attack_name, attack_damage) 
    {
        weaknesses = { "Eau", "Roche", "Sol" };
        resistances = { "Plante", "Glace", "Insecte", "Acier", "Fée" };
    }

PokemonEau::PokemonEau(const std::string& name, const std::string& type1, const std::string& type2, int hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, hp, attack_name, attack_damage) 
    {
        weaknesses = { "Plante", "Électrik" };
        resistances = { "Feu", "Eau", "Glace", "Acier" };
    }

PokemonPlante::PokemonPlante(const std::string& name, const std::string& type1, const std::string& type2, int hp, const std::string& attack_name, int attack_damage)
    : Pokemon(name, type1, type2, hp, attack_name, attack_damage) 
    {
        weaknesses = { "Feu", "Glace", "Poison", "Vol", "Insecte" };
        resistances = { "Eau", "Sol", "Roche" };
    }

    std::shared_ptr<Pokemon> createPokemon(
        const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int hp, 
        const std::string& attackName, 
        int attackDamage
    ) {
        HandledTypes r_type1 { getHandledType(type1) };
        HandledTypes r_type2 { getHandledType(type2) };
    
        // Check if the type is handled
        if (r_type1 == HandledTypes::Feu || r_type2 == HandledTypes::Feu)
            return std::make_shared<PokemonFeu>(name, type1, type2, hp, attackName, attackDamage);
        if (r_type1 == HandledTypes::Eau || r_type2 == HandledTypes::Eau) 
            return std::make_shared<PokemonEau>(name, type1, type2, hp, attackName, attackDamage);
        if (r_type1 == HandledTypes::Plante || r_type2 == HandledTypes::Plante)
            return std::make_shared<PokemonPlante>(name, type1, type2, hp, attackName, attackDamage);
    
        std::cerr << "The type of the pokemon " << name << " is not handled yet." << std::endl;

        return nullptr;
    }
    
