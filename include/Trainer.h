#ifndef TRAINER_H
#define TRAINER_H

#include "Pokemon.h"
#include "Interact.h"

#include <unordered_map>

inline constexpr int default_potions=5;

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
    virtual void Defeated() = 0;

    const std::vector<std::shared_ptr<Pokemon>>& getPokemons() const;
    const std::string& getName() const;
};

class Player : public Trainer
{
private:
    int badges {};
    int wins {};
    int defeats {};
    int nb_potions {default_potions};

public:
    Player(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons);
    Player() = default;

    const std::string toString() const override;

    void setName(const std::string& new_name);
    void setPokemons(const std::vector<std::shared_ptr<Pokemon>>& new_pokemons);
    void setNbPotions(int new_nb_potions);
    void setWins(int new_wins);
    void setDefeats(int new_defeats);
    void setBadges(int new_badges);

    int getBadges() const;
    int getWins() const;
    int getDefeats() const;
    int getNbPotions() const;

    void swapPokemons(int index1, int index2);
    void Defeated() override;

    const std::string interactWith(const std::shared_ptr<Interact>& target);
};

class GymLeader : public Trainer, public Interact
{
private:
    std::string gym_name {};
    std::string badge {};
    int badges_condition{};
    bool defeated { false };

public: 
    GymLeader(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons,
              const std::string& gym_name, const std::string& badge, int badges_condition);

    const std::string toString() const override;
    const std::string& getGymName() const;
    int getBadgesCondition() const;
    bool isDefeated() const;
    void Defeated() override;
    const std::string interactWith() const override;
};

class Master : public Trainer
{
private:
    bool defeated { false };

public:
    Master(const std::string& name, std::vector<std::shared_ptr<Pokemon>> pokemons);
    const std::string toString() const override;
    bool isDefeated() const;
    void Defeated() override;
};

#endif
