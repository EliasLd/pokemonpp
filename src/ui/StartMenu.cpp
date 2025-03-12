#include "StartMenu.h"
#include "GameState.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

GameState StartMenu(ScreenInteractive& screen)
{
    GameState current_state { GameState::StartMenu };

    Component title = Renderer([&] {
        return vbox ({
            text("   ___       __                       ___  ___  ") | color(Color::Blue1) | center ,
            text("  / _ \\___  / /_____ __ _  ___  ___  / _ \\/ _ \\ ") | color(Color::Blue1) | center,
            text(" / ___/ _ \\/  '_/ -_)  ' \\/ _ \\/ _ \\/ ___/ ___/ ") | color(Color::Blue1) | center,
            text("/_/   \\___/_/\\_\\\\__/_/_/_/\\___/_//_/_/  /_/     ") | color(Color::Blue1) | center,
        });
    });

    Component selection = Container::Horizontal({
        Button("Start", [&] {current_state = GameState::SelectionMenu; screen.ExitLoopClosure()();}),
        Button("Exit", [&] {current_state = GameState::Exit; screen.ExitLoopClosure()();})
    });

    auto start_menu = Container::Vertical({
        title,
        selection | border | center,
    }) | center;

    screen.Loop(start_menu);

    return current_state;
}