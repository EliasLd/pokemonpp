#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Pokemon
{
protected:
    std::string name;
    std::string type1;
    std::string type2;
    int hp;
    std::string attack_name;
    int attack_damage;
    std::vector<std::string> weaknesses {};
    std::vector<std::string> resistances {};

public:
    Pokemon (const std::string& name, 
            const std::string& type1, 
            const std::string& type2,
            int hp, 
            const std::string& attack_name, 
            int attack_damage);
    
    virtual void attack(Pokemon& p);
    virtual ~Pokemon() = default;
    
    // Getters
    const std::string& getName() const;
    const std::string& getType1() const;
    const std::string& getType2() const;
    int getHp() const;
    const std::string& getAttackName() const;
    int getAtackDamage() const;
    const std::vector<std::string>& getWeaknesses() const;
    const std::vector<std::string>& getResistances() const;

    void takeDamage(int damage);
    void display() const;
};

class PokemonFeu : public Pokemon 
{
public:
    PokemonFeu(const std::string& name, 
                const std::string& type1, 
                const std::string& type2, 
                int hp, const std::string& attack_name, int attack_damage);
};

class PokemonEau : public Pokemon 
{
public:
    PokemonEau(const std::string& name,
                const std::string& type1,
                const std::string& type2, 
                int hp, const std::string& attack_name, int attack_damage);
};

class PokemonPlante : public Pokemon 
{
public:
    PokemonPlante(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int hp, const std::string& attack_name, int attack_damage);
};

std::shared_ptr<Pokemon> createPokemon(
    const std::string& name, 
    const std::string& type1, 
    const std::string& type2, 
    int hp,
    const std::string& attackName, 
    int attackDamage
);

#endif