#include "Game.h"
#include "Trainer.h"
#include "MainMenu.h"

bool isOpponentMaster(Trainer& opponent) {
    return dynamic_cast<Master*>(&opponent) != nullptr;
}

void updatePokemonIndex(int& index, std::vector<std::shared_ptr<Pokemon>>& pokemon_list) {

    int list_lenght { static_cast<int>(pokemon_list.size()) };
    while(index < list_lenght - 1 && pokemon_list[index]->getCurrentHp() <= 0) {
        index++;
    }
    if(index >= list_lenght) {
        index = list_lenght - 1;
    }
}

bool allPokemonsKO(const std::vector<std::shared_ptr<Pokemon>>& pokemon_list) {

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

bool defeatedAllGym(const std::vector<GymLeader>& leaders) {
    for(const auto& leader: leaders) {
        if(!leader.isDefeated())
            return false;
    }

    return true;
}

bool defeatedAllMasters(const std::vector<Master>& masters) {
    for(const auto& master: masters) {
        if(!master.isDefeated())
            return false;
    }

    return true;
}
