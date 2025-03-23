#include "Game.h"
#include "Trainer.h"
#include "MainMenu.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

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

void opponentTurn(bool& is_player_turn, std::vector<std::shared_ptr<Pokemon>>& player_pokemons, 
                  int& player_index, std::shared_ptr<Pokemon>& opponent_pokemon, 
                  std::vector<Element>& logs, ScreenInteractive& screen, Player& player) {
    if (!is_player_turn) {
        if(opponent_pokemon->getCurrentHp() > 0) {
            std::shared_ptr<Pokemon> target = player_pokemons[player_index];
            if(target->getCurrentHp() > 0) {
                float multiplicator { getDamagesMultiplicator(opponent_pokemon, target) };
                int damage { static_cast<int>(multiplicator * opponent_pokemon->getAttackDamage()) };
                logs.push_back(text(opponent_pokemon->getName() + " does " + std::to_string(damage) + " damages to " + target->getName()));
                target->takeDamage(damage);
            }

            updatePokemonIndex(player_index, player_pokemons);

            if(allPokemonsKO(player_pokemons)) {
                player.setDefeats(player.getDefeats() + 1);
                screen.ExitLoopClosure()();
            }

            is_player_turn = true;
        }
    }
}

Component fightHeader(const Player& player, const Trainer& opponent) {
    return Renderer([&] {
        return hbox ({
            text("Pokemon trainer " + player.getName()),
            text(" VS ") | color(Color::Green),
            text(opponent.toString()),
        });
    });
}


void Fight(ftxui::ScreenInteractive& screen, Player& player, GymLeader& opponent) {

    screen.Clear();

    bool is_master { isOpponentMaster(opponent) };
    bool is_player_turn { true };

    std::vector<std::shared_ptr<Pokemon>> player_pokemons { player.getPokemons() };
    std::vector<std::shared_ptr<Pokemon>> opponent_pokemons { opponent.getPokemons() };

    int player_index {};
    int opponent_index{};

    updatePokemonIndex(player_index, player_pokemons);
    updatePokemonIndex(opponent_index, opponent_pokemons);

    std::vector<Element> fight_logs {};

    Component fight_header = fightHeader(player, opponent);

    Component exit_button = Button("Exit (tmp)", [&] {
        screen.ExitLoopClosure()();
    });

    Component player_interface = Container::Vertical({
        Renderer([&] {
            return vbox({
                text(player.getName() + " - " + std::to_string(player.getNbPotions()) + " potion(s)"),
                separatorDouble(),
                text (
                    "Current Pokemon: " + player_pokemons[player_index]->getName() + " - "
                    + std::to_string(player_pokemons[player_index]->getCurrentHp()) + "/" 
                    + std::to_string(player_pokemons[player_index]->getBaseHp()) + " HP"
                ),
                text (
                    "Type(s) : " + player_pokemons[player_index]->getType1()
                    + (!player_pokemons[player_index]->getType2().empty() ? ", " + player_pokemons[player_index]->getType2() : "")
                ),
                separatorDouble(),
            });
        }),
    }) | center | border;

    Component opponent_interface = Container::Vertical ({
        Renderer([&] {
            return vbox ({
                text((is_master ? "Master " : "Gym leader ") + opponent.getName()),
                separatorDouble(),
                text (
                    "Current Pokemon: " + opponent_pokemons[opponent_index]->getName() + " - "
                    + std::to_string(opponent_pokemons[opponent_index]->getCurrentHp()) + "/" 
                    + std::to_string(opponent_pokemons[opponent_index]->getBaseHp()) + " HP"
                ),
                text (
                    "Type(s) : " + opponent_pokemons[opponent_index]->getType1()
                    + (!opponent_pokemons[opponent_index]->getType2().empty() ? ", " + opponent_pokemons[opponent_index]->getType2() : "")
                ),
            });
        }),
    }) | center | border;

    Component attack_button = Button("Attack", [&] {
        if(is_player_turn) {
            std::shared_ptr<Pokemon> target = opponent_pokemons[opponent_index];
            if(target->getCurrentHp() > 0) {
                float multiplicator { getDamagesMultiplicator(player_pokemons[player_index], target) };
                int damage { static_cast<int>(multiplicator * player_pokemons[player_index]->getAttackDamage()) };
                target->takeDamage(damage);
                fight_logs.push_back(text(player_pokemons[player_index]->getName() + " does " + std::to_string(damage) + " damages to " + target->getName()));
                
                if(allPokemonsKO(opponent_pokemons)) {
                    opponent.Defeated();
                    player.setNbPotions(player.getNbPotions() + 5);
                    player.setBadges(player.getBadges() + 1);
                    player.setWins(player.getWins() + 1);
                    // Cela enlève juste l'affichage du combat
                    // Le pokemon adverse peut toujours attaquer.
                    screen.ExitLoopClosure()();
                }

                // Mise à jour du Pokémon adverse si K.O.
                updatePokemonIndex(opponent_index, opponent_pokemons);
            }
            
            is_player_turn = false;

            opponentTurn(is_player_turn, player_pokemons, player_index, opponent_pokemons[opponent_index], fight_logs, screen, player);

            screen.PostEvent(Event::Custom);
        }
    });

    Component heal_button = healButton(player_index, player);

    if(allPokemonsKO(player_pokemons))
        screen.ExitLoopClosure()();

    Component logs = Renderer([&]{
        return vbox(fight_logs) | flex;
    });
            
    Component renderer = Container::Vertical({
        fight_header,
        Container::Horizontal({
            player_interface | center,
            logs | center | flex | border,
            opponent_interface | center,
        }),
        exit_button | center,
        attack_button | center,
        heal_button | center,
    }) | center | border | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(renderer);
}