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
            text("/_/   \\___/_/\\_\\\\__/_/_/_/\\_ __/_//_/_/  /_/     ") | color(Color::Blue1) | center,
        });
    });

    Component subhead = Renderer([&]{
        return vbox ({
            separatorEmpty(),
            text("Welcome to Pokemonpp, a C++ Pokemon battle simulation") | center | bold | color(Color::BlueLight),
            separatorEmpty(),
        });
    });

    auto style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                        Color::Default, Color::White);

    Component selection = Container::Horizontal({
        Button("Start", [&] {current_state = GameState::Introduction; screen.ExitLoopClosure()();}, style) | center,
        Renderer([] { return separatorDouble(); }),
        Button("Exit", [&] {current_state = GameState::Exit; screen.ExitLoopClosure()();}, style) | center,
    });

    auto start_menu = Container::Vertical({
        title,
        subhead,
        selection  | center,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(start_menu);

    return current_state;
}