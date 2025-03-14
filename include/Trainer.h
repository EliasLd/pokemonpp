#ifndef TRAINER_H
#define TRAINER_H

#include "Pokemon.h"

#include <unordered_map>

class Trainer
{
protected:
    std::string name {};
    std::vector<std::shared_ptr<Pokemon>> pokemons {};

public:
    Trainer() = default;
    Trainer(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons);

    virtual ~Trainer() = default;
    // pure virtual function
    virtual const std::string toString() const = 0;

    const std::vector<std::shared_ptr<Pokemon>> getPokemons() const;
};

class Player : public Trainer
{
private:
    int badges {};
    int wins {};
    int defeats {};

public:
    Player(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons);
    Player() = default;

    const std::string toString() const override;

    void setName(const std::string& new_name);
    void setPokemons(const std::vector<std::shared_ptr<Pokemon>>& new_pokemons);
};

class GymLeader : public Trainer
{
private:
    std::string gym_name {};
    std::string badge {};

public: 
    GymLeader(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons,
              const std::string& gym_name, const std::string& badge);
    const std::string toString() const override;
};

class Master : public Trainer
{
public:
    Master(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons);
    const std::string toString() const override;
};

std::vector<GymLeader> readGymLeadersFromCSV(
    const std::string& filename,
    std::unordered_map<std::string, std::shared_ptr<Pokemon>> pokemon_map);

#endif