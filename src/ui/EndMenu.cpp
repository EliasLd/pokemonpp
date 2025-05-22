#include "EndMenu.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

GameState EndMenu(ScreenInteractive& screen, const bool victory)
{
    GameState current_state { GameState::StartMenu };

    Component victory_text = Renderer([] {
        return vbox ({
            text(R"(  _   ___     __               )") | color(Color::Green1) | center,
            text(R"( | | / (_)___/ /____  ______ __)") | color(Color::Green1) | center,
            text(R"( | |/ / / __/ __/ _ \/ __/ // /)") | color(Color::Green1) | center,
            text(R"( |___/_/\__/\__/\___/_/  \_, / )") | color(Color::Green1) | center,
            text(R"(                        /___/  )") | color(Color::Green1) | center,
            separatorEmpty(),
            text("You won the game! You can play again or exit.") | center | bold | color(Color::SkyBlue1),
        });
    });

    Component game_over_text = Renderer([] {
        return vbox ({
            text(R"(   _____                 ____              )") | color(Color::Red1) | center,
            text(R"(  / ___/__ ___ _  ___   / __ \_  _____ ____)") | color(Color::Red1) | center,
            text(R"( / (_ / _ `/  ' \/ -_) / /_/ / |/ / -_) __/)") | color(Color::Red1) | center,
            text(R"( \___/\_,_/_/_/_/\__/  \____/|___/\__/_/   )") | color(Color::Red1) | center,
            separatorEmpty(),
            text("Game is over! You can play again or exit.") | center | bold | color(Color::SkyBlue1),
        });
    });

    auto style = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1,
                                        Color::Default, Color::RGB(255, 255, 255));

    Component selection = Container::Horizontal({
        Button("Play again", [&] {current_state = GameState::SelectionMenu; screen.ExitLoopClosure()();}, style) | center,
        Renderer([] { return separatorDouble(); }),
        Button("Exit", [&] {current_state = GameState::Exit; screen.ExitLoopClosure()();}, style) | center,
    });

    auto start_menu = Container::Vertical({
        victory ? victory_text : game_over_text,
        selection | center,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(start_menu);

    return current_state;
}
