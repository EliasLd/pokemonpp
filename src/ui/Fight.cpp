#include "Game.h"
#include "Trainer.h"
#include "FightUtils.h"
#include "MainMenu.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

void opponentTurn(bool& is_player_turn, std::vector<std::shared_ptr<Pokemon>>& player_pokemons, 
                  int& player_index, std::shared_ptr<Pokemon>& opponent_pokemon, 
                  Player& player, Trainer& opponent, 
                  std::vector<Element>& logs, ScreenInteractive& screen) {

    if (!is_player_turn) {
        if(opponent_pokemon->getCurrentHp() > 0) {
            std::shared_ptr<Pokemon> target {};
            if(player_index < static_cast<int>(player_pokemons.size())){
                target = player_pokemons[player_index];
            }
            if(target->getCurrentHp() > 0) {
                float multiplicator { getDamagesMultiplicator(opponent_pokemon, target) };
                int damage { static_cast<int>(multiplicator * opponent_pokemon->getAttackDamage()) };
                logs.push_back(text(opponent_pokemon->getName() + " does " + std::to_string(damage) + " damages to " + target->getName()));
                target->takeDamage(damage);
            }

            updatePokemonIndex(player_index, player_pokemons);

            if(allPokemonsKO(player_pokemons)) {
                player.setDefeats(player.getDefeats() + 1);
                resetPokemonHp(opponent.getPokemons());
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


void Fight(ftxui::ScreenInteractive& screen, Player& player, Trainer& opponent) {

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
    
    int log_scroll_index {};

    Component logs = Container::Vertical({
        Renderer([&] {
            int log_size = fight_logs.size();
            int display_count = 6; 
            int start_index = std::max(0, log_size - display_count - log_scroll_index);
            
            std::vector<Element> visible_logs;
            for (int i = start_index; i < log_size; ++i) {
                visible_logs.push_back(fight_logs[i]);
            }
    
            return vbox(visible_logs) | flex | size(HEIGHT, LESS_THAN, 8);
        })
    });
    
    Component logs_container = Renderer([&] {
        return vbox({
            text("Fight logs") | center,
            separatorDouble(),
            logs->Render(),
        });
    });

    Component fight_header = fightHeader(player, opponent);

    Component attack_button = Button("Attack", [&] {

        if(is_player_turn) {

            std::shared_ptr<Pokemon> target {};

            if(opponent_index < static_cast<int>(opponent_pokemons.size())){
                target = opponent_pokemons[opponent_index];
            }

            if(target->getCurrentHp() > 0) {
                float multiplicator { getDamagesMultiplicator(player_pokemons[player_index], target) };
                // Pokemon masters deal +25% damages 
                if(is_master) { multiplicator *= 1.25; }
                int damage { static_cast<int>(multiplicator * player_pokemons[player_index]->getAttackDamage()) };
                target->takeDamage(damage);
                fight_logs.push_back(text(player_pokemons[player_index]->getName() + " does " + std::to_string(damage) + " damages to " + target->getName()));
                
                screen.PostEvent(Event::Custom);

                if(allPokemonsKO(opponent_pokemons)) {
                    opponent.Defeated();
                    player.setNbPotions(player.getNbPotions() + 1);
                    if(!is_master) { player.setBadges(player.getBadges() + 1); }
                    player.setWins(player.getWins() + 1);
                    screen.ExitLoopClosure()();
                }

                // Update opponent pokemon if K.O
                updatePokemonIndex(opponent_index, opponent_pokemons);
            }
            
            is_player_turn = false;

            opponentTurn(is_player_turn, player_pokemons, player_index, opponent_pokemons[opponent_index], player, opponent, fight_logs, screen);
        }
    }, ButtonOption::Animated());

    Component heal_button = healButton(player_index, player);

    Component player_interface = Container::Vertical({
        Renderer([&] {
            return vbox({
                text(player.getName() + " - " + std::to_string(player.getNbPotions()) + " potion(s)"),
                separatorDouble(),
                player_pokemons[player_index]->getSprite()->Render(),
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
        Container::Horizontal({
            attack_button,
            heal_button
        }) | center,
    }) | center | border;

    Component opponent_interface = Container::Vertical ({
        Renderer([&] {
            return vbox ({
                text((is_master ? "Master " : "Gym leader ") + opponent.getName()),
                separatorDouble(),
                opponent_pokemons[opponent_index]->getSprite()->Render(),
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

    if(allPokemonsKO(player_pokemons))
        screen.ExitLoopClosure()();
            
    Component renderer = Container::Vertical({
        fight_header | center | border,
        Container::Horizontal({
            player_interface | center,
            logs_container | center | flex | border,
            opponent_interface | center,
        }),
    }) | center | border | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(renderer);
}