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
    });

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

    Component render = Container::Vertical({
        header,
        exit_button | align_right,
    }) | center | borderDouble | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(render);
}