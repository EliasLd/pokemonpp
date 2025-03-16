#include "MainMenu.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

Component exitButton(ScreenInteractive& screen) {
    return Button(" Exit game ", [&] {
        screen.ExitLoopClosure()();
    }, ButtonOption::Animated(Color::Red));
}

Component PlayerStats(const Player& player) {
    // Display player statistics
    Component stats = Renderer([&] {
        return hbox ({
            text(player.getName() + " "),
            separatorDouble(),
            text(" " + std::to_string(player.getBadges()) + " badges ") | color(Color::BlueLight),
            separatorDouble(),
            text(" " + std::to_string(player.getWins()) + " win(s) ") | color(Color::Green),
            separatorDouble(),
            text(" " + std::to_string(player.getDefeats()) + " defeat(s) ") | color(Color::Red) | size(WIDTH, EQUAL, 12),
        }) | center | border;
    });

    return stats;
}

Component leaderEntry(ScreenInteractive& screen, const GymLeader& leader, const Player& player) {
    /* 
        Display informations about a gym leader
        and a button to fight them
    */
    Component button = Button("Fight", [&] {
        screen.ExitLoopClosure()();
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

void mainMenu(ScreenInteractive& screen, GameState& state, Player& player, 
    std::vector<GymLeader>& leaders, 
    std::vector<Master>& masters)
{
    Component header            { PlayerStats(player) };
    Component title             { Title(player, leaders) };
    Component leaders_display   { Container::Vertical({}) };
    Component exit_button       { exitButton(screen) };

    leaders_display->Add(title);

    for(const auto& leader: leaders) {
        // Display infos and fight button for each unlocked gym leader
        if(player.getBadges() >= leader.getBadgesCondition()) {
            Component leader_entry = leaderEntry(screen, leader, player);
            leaders_display->Add(leader_entry);
        }
    } 

    Component render = Container::Vertical({
        header,
        leaders_display | border,
        exit_button | align_right,
    }) | center | borderDouble | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(render);
}