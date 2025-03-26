#include "MainMenu.h"
#include "Game.h"
#include "Fight.h"
#include "FightUtils.h"

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

Component leaderEntry(ScreenInteractive& screen, GymLeader& leader, Player& player, GameState& state) {
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

    Component leader_entry = Container::Horizontal({

        Renderer([=] {

            std::vector<Element> leader_elements = {};
            leader_elements.push_back(text(leader.getName() + " - " + leader.getGymName()) | vcenter | size(WIDTH, EQUAL, 40));
            leader_elements.push_back(separatorEmpty());
            int earned_badges { player.getBadges() };
            
            if(leader.isDefeated())
                leader_elements.push_back(text("Defeated") | color(Color::Green3) | center);
            else
                leader_elements.push_back(text("Not defeated") | color(Color::Red1) | center);
            
            leader_elements.push_back(separatorEmpty());
            leader_elements.push_back(separatorDouble());
    
            return hbox(leader_elements);
        }),
        button | center | size(HEIGHT, EQUAL, 3),
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
            return vbox({
                text(p->getName()) | bold,
                separator(),
                text(std::to_string(p->getCurrentHp()) + "/" + std::to_string(p->getBaseHp()) + " HP"),
                text("Type(s): " + p->getType1() + (p->getType2().empty() ? "" : ", " + p->getType2())),
            }) | border | center;
        }),
    });
}

Component healButton(int& selected, Player& player) {
    return Button("Heal", [&] {
        auto& selected_pokemon = player.getPokemons()[selected];
        // Heal the pokemon only if needed and player has at least 1 potion
        if(selected_pokemon->getCurrentHp() < selected_pokemon->getBaseHp() 
        && player.getNbPotions() > 0) 
        {
            selected_pokemon->heal();
            player.setNbPotions(player.getNbPotions() - 1);
        }
    }, ButtonOption::Animated(Color::Pink1));
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

    leaders_display->Add(title);

    for(auto& leader: leaders) {
        // Display infos and fight button for each unlocked gym leader
        if(player.getBadges() >= leader.getBadgesCondition()) {
            Component leader_entry = leaderEntry(screen, leader, player, state);
            leaders_display->Add(leader_entry);
        }
    } 

    Component masters_container = Container::Vertical({});

    if(defeatedAllGym(leaders)){
        masters_container->Add(Button("Fight a random Pokemon Master", [&] {
            Fight(screen, player, masters[0]);
            screen.ExitLoopClosure()();
        }, ButtonOption::Animated(Color::Yellow1)));
    }

    Component leaders_container = Container::Vertical({
        header,
        leaders_display | border,
        exit_button | align_right,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    updatePokemonsEntries(tab_values, tab_entries, player);
    // display player's pokemons
    auto tab_toggle { Radiobox(&tab_values, &tab_selected) };
    // display details of selected pokemon
    Component tab_content = Renderer([&] {
        return hbox({
            separatorDouble(),
            PokemonDetails(player.getPokemons()[tab_selected])->Render(),
            separatorDouble(),
        }); 
    });

    Component heal_button = healButton(tab_selected, player);
    Component separator_container = Renderer([&] { return vbox(separatorDouble());});
    Component move_container = movePokemonContainer(tab_values, tab_entries, player, tab_selected);
    
    Component pokemon_container = Container::Horizontal({
        tab_toggle,
        tab_content,
        Container::Vertical ({
            move_container | hcenter,
            separator_container,
            heal_button,
        }),
    }) | border | size(HEIGHT, EQUAL, 10);

    // Renderer, wrap all containers.
    Component render = Container::Vertical({
        Container::Horizontal({
            leaders_container,
            pokemon_container,
        }),
        masters_container | center,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(render);
}
