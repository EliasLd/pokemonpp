#include "MainMenu.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

void mainMenu(ftxui::ScreenInteractive& screen, GameState& state, Player& player, 
    std::vector<GymLeader>& leaders, 
    std::vector<Master>& masters)
{
    Component exit_button = Button (" Exit game ", [&] {
        screen.ExitLoopClosure()();
    }, ButtonOption::Animated(Color::Red));

    Component header = Renderer([&] {
        return hbox ({
            text(player.getName() + " "),
            separatorDouble(),
            text(" " + std::to_string(player.getBadges()) + " badges ") | color(Color::BlueLight),
            separatorDouble(),
            text(" " + std::to_string(player.getWins()) + " win(s) ") | color(Color::Green),
            separatorDouble(),
            text(" " + std::to_string(player.getDefeats()) + " defeat(s) ") | color(Color::Red),
        }) | center | border;
    });

    Component leaders_display = Container::Vertical({});

    for(const auto& leader: leaders) {
        Component button = Button("Fight", [&] {
            screen.ExitLoopClosure()();
        }, ButtonOption::Animated());

        Component leader_entry = Container::Horizontal ({
            Renderer([=] { 
                return hbox ({
                    text(" " + leader.getName() + " - " + leader.getGymName()) 
                    | vcenter | size(WIDTH, EQUAL, 40),
                    separatorDouble(),
                });
            }),
            button | center | size(HEIGHT, EQUAL, 3),
        }) | center;

        leaders_display->Add(leader_entry);
    } 

    Component render = Container::Vertical({
        header,
        leaders_display | borderDouble,
        exit_button | align_right,
    }) | center | borderDouble | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(render);
}