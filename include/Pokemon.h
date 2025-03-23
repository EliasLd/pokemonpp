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
    int base_hp;
    int current_hp;
    std::string attack_name;
    int attack_damage;
    std::vector<std::string> weaknesses {};
    std::vector<std::string> resistances {};

public:
    Pokemon (const std::string& name, 
            const std::string& type1, 
            const std::string& type2,
            int base_hp, 
            const std::string& attack_name, 
            int attack_damage);
    
    virtual void attack(std::shared_ptr<Pokemon> target, int damages);
    virtual ~Pokemon() = default;
    virtual std::shared_ptr<Pokemon> clone() const = 0;
    
    // Getters
    const std::string& getName() const;
    const std::string& getType1() const;
    const std::string& getType2() const;
    int getBaseHp() const;
    int getCurrentHp() const;
    const std::string& getAttackName() const;
    int getAttackDamage() const;
    const std::vector<std::string>& getWeaknesses() const;
    const std::vector<std::string>& getResistances() const;

    // Pokemon methods
    void takeDamage(int damage);
    const std::string toString() const;
    void assignWeaknessesAndResistances();
    void heal();
};

class PokemonFeu : public Pokemon 
{
public:
    PokemonFeu(const std::string& name, 
                const std::string& type1, 
                const std::string& type2, 
                int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonEau : public Pokemon 
{
public:
    PokemonEau(const std::string& name,
                const std::string& type1,
                const std::string& type2, 
                int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonPlante : public Pokemon 
{
public:
    PokemonPlante(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonSol : public Pokemon 
{
public:
    PokemonSol(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonElectrik : public Pokemon 
{
public:
    PokemonElectrik(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonPoison : public Pokemon 
{
public:
    PokemonPoison(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

class PokemonPsy : public Pokemon 
{
public:
    PokemonPsy(const std::string& name, 
        const std::string& type1, 
        const std::string& type2, 
        int base_hp, const std::string& attack_name, int attack_damage);

    std::shared_ptr<Pokemon> clone() const override;
};

std::shared_ptr<Pokemon> createPokemon(
    const std::string& name, 
    const std::string& type1, 
    const std::string& type2, 
    int base_hp,
    const std::string& attackName, 
    int attackDamage
);

void addVectToVect(std::vector<std::string>& dest, const std::vector<std::string>& src);

#endif