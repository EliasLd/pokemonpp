#include "Game.h"
#include "Trainer.h"
#include "MainMenu.h"

bool isOpponentMaster(Trainer& opponent) {
    return dynamic_cast<Master*>(&opponent) != nullptr;
}

void updatePokemonIndex(int& index, std::vector<std::shared_ptr<Pokemon>> pokemon_list) {

    int list_lenght { static_cast<int>(pokemon_list.size()) };
    while(index < list_lenght - 1 && pokemon_list[index]->getCurrentHp() <= 0) {
        index++;
    }
}

bool allPokemonsKO(const std::vector<std::shared_ptr<Pokemon>> pokemon_list) {

    for(const auto& p: pokemon_list) {
        if(p->getCurrentHp() > 0)
            return false;
    }

    return true;
}

void resetPokemonHp(const std::vector<std::shared_ptr<Pokemon>>& pokemon_list) {
    for(auto& p: pokemon_list)
        p->heal();
}

float getDamagesMultiplicator(std::shared_ptr<Pokemon>& src, std::shared_ptr<Pokemon>& target) {

    std::string type1 { src->getType1() };
    std::string type2 { src->getType2() };

    for(const auto& weakness: target->getWeaknesses()){
        if(type1 == weakness || type2 == weakness)
            return 2.0f;
    }
    for(const auto& resistance: target->getResistances()){
        if(type1 == resistance || type2 == resistance)
            return 0.5f;
    }

    return 1.0f;

}

bool defeatedAllGym(std::vector<GymLeader>& trainers) {
    for(const auto& trainer: trainers) {
        if(!trainer.isDefeated())
            return false;
    }

    return true;
}
