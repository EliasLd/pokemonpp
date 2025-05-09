#include "StartMenu.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

GameState StartMenu(ScreenInteractive& screen)
{
    GameState current_state { GameState::StartMenu };

    Component title = Renderer([&] {
        return vbox ({
            text(R"(   ___       __                       ___  ___ )") | color(Color::Blue1) | center,
            text(R"(  / _ \___  / /_____ __ _  ___  ___  / _ \/ _ \)") | color(Color::Blue1) | center,
            text(R"( / ___/ _ \/  '_/ -_)  ' \/ _ \/ _ \/ ___/ ___/)") | color(Color::Blue1) | center,
            text(R"(/_/   \___/_/\_\\__/_/_/_/\___/_//_/_/  /_/    )") | color(Color::Blue1) | center,
        });
    });

    Component subhead = Renderer([&]{
        return vbox ({
            separatorEmpty(),
            text("Welcome to Pokemonpp, a C++ Pokemon battle simulation") | center | bold | color(Color::SkyBlue1),
            separatorEmpty(),
        });
    });

    auto style = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1,
                                        Color::Default, Color::RGB(255, 255, 255));

    Component selection = Container::Horizontal({
        Button("Start", [&] {current_state = GameState::Introduction; screen.ExitLoopClosure()();}, style) | center,
        Renderer([] { return separatorDouble(); }),
        Button("Exit", [&] {current_state = GameState::Exit; screen.ExitLoopClosure()();}, style) | center,
    });

    Component warning_message = Renderer([&] {
        if (Terminal::ColorSupport() < Terminal::Color::TrueColor) {
            return vbox({
                text("Your terminal does not fully support colors."),
                text("This may affect the visual experience."),
                text("Please use a terminal that fully supports colors for the best experience."),
            }) | border;
        }
        return vbox({});
    });

    Component terminal_info = Renderer([&] {
        return vbox({
            Terminal::ColorSupport() >= Terminal::Color::Palette16
                ? text(" 16 color palette support : Yes")
                : text(" 16 color palette support : No "),
            Terminal::ColorSupport() >= Terminal::Color::Palette256
                ? text("256 color palette support : Yes")
                : text("256 color palette support : No "),
            Terminal::ColorSupport() >= Terminal::Color::TrueColor
                ? text("       True color support : Yes")
                : text("       True color support : No "),
        }) | border;
	});

    Component conditional_warning = Renderer([&] {
        if (Terminal::ColorSupport() < Terminal::Color::TrueColor) {
            return vbox({
                warning_message->Render(),
                separatorDouble(),
                terminal_info->Render(),
            }) | border;
        }
        return vbox({});
    });


    auto start_menu = Container::Vertical({
        title,
        subhead,
		conditional_warning,
        selection  | center,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(start_menu);

    return current_state;
}