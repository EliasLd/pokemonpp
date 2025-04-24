#include "MainMenu.h"
#include "Game.h"
#include "Fight.h"
#include "FightUtils.h"
#include "Random.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

Component exitButton(ScreenInteractive& screen, GameState& state) {
    return Button(" Exit game ", [&] {
        state = GameState::Exit;
        screen.ExitLoopClosure()();
    }, ButtonOption::Animated(Color::Red));
}

Component PlayerStats(const Player& player) {
    // Display player statistics
    Component stats = Renderer([&] {
        return hbox ({
            text(player.getName() + " "),
            separatorDouble(),
            text(" " + std::to_string(player.getBadges()) + " badge(s) ") | color(Color::BlueLight),
            separatorDouble(),
            text(" " + std::to_string(player.getWins()) + " win(s) ") | color(Color::Green),
            separatorDouble(),
            text(" " + std::to_string(player.getDefeats()) + " defeat(s) ") | color(Color::Red),
            separatorDouble(),
            text(" " + std::to_string(player.getNbPotions()) + " potion(s) ") | color(Color::Pink1),
        }) | center | border;
    });

    return stats;
}

Component leaderInteractButton(Player& player, GymLeader& leader, std::shared_ptr<Element> interaction_text) {
    return Button("Interact", [&] {
        if(leader.isDefeated()) {
            *interaction_text = text(player.interactWith(std::make_shared<GymLeader>(leader)));
        }
        else {
            *interaction_text = text(leader.getName() + " a le regard plein de détermination et n'attend qu'une chose: vous combattre !");
        }
    }, ButtonOption::Animated());
}

Component leaderEntry(ScreenInteractive& screen, GymLeader& leader, Player& player, GameState& state, std::shared_ptr<Element> interaction_text) {
    /* 
        Display informations about a gym leader
        and a button to fight them
    */
    Component button = Button("Fight", [&] {
        if(!leader.isDefeated()) {
            Fight(screen, player, leader);
            screen.ExitLoopClosure()();
        }
    }, ButtonOption::Animated());

    Component interaction_button = Button("Interact", [&] {
        if(leader.isDefeated()) {
            *interaction_text = text(player.interactWith(std::make_shared<GymLeader>(leader)));
        }
        else {
            *interaction_text = text(leader.getName() + " a le regard plein de détermination et n'attend qu'une chose: vous combattre !");
        }
    }, ButtonOption::Animated());

    Component leader_entry = Container::Horizontal({

        Renderer([&] {

            std::vector<Element> leader_elements = {};
            leader_elements.push_back(text(leader.getName() + " - " + leader.getGymName()) | vcenter | size(WIDTH, EQUAL, 40));
            leader_elements.push_back(separatorEmpty());
            int earned_badges { player.getBadges() };
            
            if(leader.isDefeated())
                leader_elements.push_back(text("  Defeated  ") | color(Color::Green3) | center);
            else    
                leader_elements.push_back(text("Not defeated") | color(Color::Red1) | center);
            
            leader_elements.push_back(separatorEmpty());
            leader_elements.push_back(separatorDouble());
    
            return hbox(leader_elements);
        }),
        button | center | size(HEIGHT, EQUAL, 3),
        interaction_button | center,
    }) | center;

    return leader_entry;
}

Component Title(const Player& player, const std::vector<GymLeader>& leaders) {
    // Display a message to keep player updated on what to do next
    std::string title_text {};
    int earned_badges { player.getBadges() };

    if (earned_badges == leaders.size())
        title_text = "Defeated all gym leaders! You can now fight a Pokemon Master !";
    else
        title_text = "Defeat " + leaders.at(earned_badges).getName() + " to unlock the next gym fight !";
    
    Component title = Renderer([=] {
        return vbox ({
            text(title_text) | center,
            separator(),
        });
    });

    return title;
}

void updatePokemonsEntries(std::vector<std::string>& values, std::vector<std::string>& entries, Player& player) {
    // Used to refresh pokemon displayed pokemon
    // details when moved
    values.clear();
    entries.clear();
    for (const auto& p : player.getPokemons())
        values.push_back(p->getName());
}

Component movePokemonContainer(std::vector<std::string>& values, std::vector<std::string>& entries, Player& player, int& selected) {
    // Swap pokemon position in pokemon list
    auto move_up_button = Button("↑", [&] {
        if (selected > 0) {
            player.swapPokemons(selected, selected - 1);
            selected--;
            updatePokemonsEntries(values, entries, player);
        }
    });
    auto move_down_button = Button("↓", [&] {
        if (selected < player.getPokemons().size() - 1) {
            player.swapPokemons(selected, selected + 1);
            selected++;
            updatePokemonsEntries(values, entries, player);
        }
    });

    return Container::Vertical ({
        move_up_button | center,
        move_down_button | center,
    });
}

Component PokemonDetails(std::shared_ptr<Pokemon> p) {
    // Display pokemon details
    return Container::Vertical({
        Renderer([&] {
            Element hp_display = text(std::to_string(p->getCurrentHp()) + "/" + std::to_string(p->getBaseHp()) + " HP");
            return vbox({
                text(p->getName()) | bold | center,
                separator(),
                ( p->getCurrentHp() == 0 )
                ? ( hp_display | color(Color::Red) )
                : ( hp_display ),
                text("Type(s): " + p->getType1() + (p->getType2().empty() ? "" : ", " + p->getType2())),
            }) | border | center;
        }),
    });
}

Component healButton(int& selected, Player& player) {
    return Button("Heal", [&] {
        auto& selected_pokemon = player.getPokemons().at(selected);
        // Heal the pokemon only if needed and player has at least 1 potion
        if(selected_pokemon->getCurrentHp() < selected_pokemon->getBaseHp() 
        && player.getNbPotions() > 0) 
        {
            selected_pokemon->heal();
            player.setNbPotions(player.getNbPotions() - 1);
        }
    }, ButtonOption::Animated(Color::Pink1));
}

Component interactionBox(std::shared_ptr<Element> interaction_text) {
    return Container::Vertical({
        Renderer([&] {
            return vbox({
                text("Interaction box") | bold | color(Color::Green) | center,
                separatorDouble(),
                *interaction_text
            }) | border | center;
        })
    });
}

Component pokemonInteractButton(int& selected, Player& player, std::shared_ptr<Element> interaction_text) {
    return Button("Interact", [&] {
        auto& selected_pokemon = player.getPokemons().at(selected);
        *interaction_text = text(player.interactWith(selected_pokemon));
    }, ButtonOption::Animated(Color::Orange4)) | center;
}

void mainMenu(ScreenInteractive& screen, GameState& state, Player& player, 
    std::vector<GymLeader>& leaders, 
    std::vector<Master>& masters)
{
    Component header            { PlayerStats(player) };
    Component title             { Title(player, leaders) };
    Component leaders_display   { Container::Vertical({}) };
    Component exit_button       { exitButton(screen, state) };

    std::vector<std::string> tab_values {};
    std::vector<std::string> tab_entries {};
    int tab_selected {};

    auto interaction_text = std::make_shared<Element>(text(""));

    leaders_display->Add(title);

    for(auto& leader: leaders) {
        // Display infos and fight button for each unlocked gym leader
        if(player.getBadges() >= leader.getBadgesCondition()) {
            Component leader_entry = leaderEntry(screen, leader, player, state, interaction_text);
            leaders_display->Add(leader_entry);
        }
    } 

    Component masters_container = Container::Vertical({});

    if(defeatedAllGym(leaders)){
        masters_container->Add(Button("Fight a random Pokemon Master", [&] {
            int random_index {};

            do {
                random_index = Random::get(0, static_cast<int>(masters.size()) - 1);
            } while (masters.at(random_index).isDefeated() && !defeatedAllMasters(masters));

            if(!defeatedAllMasters(masters)) {
                Fight(screen, player, masters.at(random_index));
                screen.ExitLoopClosure()();
            }
            
        }, ButtonOption::Animated(Color::Yellow1)));
    }

    Component leaders_container = Container::Vertical({
        header,
        leaders_display | border,
        exit_button | xflex | size(WIDTH, EQUAL, 11),
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    updatePokemonsEntries(tab_values, tab_entries, player);
    // display player's pokemons

    Component tab_toggle = Container::Vertical({
        Radiobox(&tab_values, &tab_selected),
    });

    Component sprite = Renderer([&] {
        Component color_drawing = player.getPokemons().at(tab_selected)->getSprite();
        return color_drawing->Render();
    });

    // display details of selected pokemon
    Component tab_content = Renderer([&] {
        return hbox({
            vbox({
                PokemonDetails(player.getPokemons().at(tab_selected))->Render(),
                sprite->Render(),
            }),
        }); 
    });

    Component heal_button = healButton(tab_selected, player);
    Component move_container = movePokemonContainer(tab_values, tab_entries, player, tab_selected);
    Component pokemon_interact_button = pokemonInteractButton(tab_selected, player, interaction_text);
    Component interaction_box = interactionBox(interaction_text);
    
    Component pokemon_container = Container::Horizontal({
        Container::Vertical({
            Container::Horizontal({
                tab_toggle | border,
                move_container | center,
            }),
            pokemon_interact_button,
            heal_button,
        }),
        Renderer([] { return separatorDouble(); }),
        tab_content,
    }) | border;

    // Renderer, wrap all containers.
    Component render {};

    if (defeatedAllGym(leaders)) {
        render = Container::Vertical({
            interaction_box,
            Container::Horizontal({
                leaders_container,
                Container::Vertical({
                    pokemon_container,
                    masters_container | center,
                })
            }),
        }) | center | bgcolor(Color::RGB(0, 0, 0));
    } else {
        render = Container::Vertical({
            interaction_box,
            Container::Horizontal({
                leaders_container,
                pokemon_container,
            }),
        }) | center | bgcolor(Color::RGB(0, 0, 0));
    }

    screen.Loop(render);
}
