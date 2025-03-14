#include "Introduction.h"
#include "Game.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

std::string IntroductionMenu(ScreenInteractive& screen, GameState& current_state)
{   
    Component title = Renderer([&] {
        return vbox({
            text(R"( _       __     __                        )")  | color(Color::Blue1) | center,
            text(R"(| |     / /__  / /________  ____ ___  ___ )")  | color(Color::Blue1) | center,
            text(R"(| | /| / / _ \/ / ___/ __ \/ __ `__ \/ _ \ )") | color(Color::Blue1) | center,
            text(R"(| |/ |/ /  __/ / /__/ /_/ / / / / / /  __/)")  | color(Color::Blue1) | center,    
            text(R"(|__/|__/\___/_/\___/\____/_/ /_/ /_/\___/ )")   | color(Color::Blue1) | center,
        });
    });

    Component intro_text = Renderer([&] {
        return vbox ({
            separatorEmpty(),
            text("Welcome to the incredible world of Pokemons !") | bold | center | color(Color::BlueLight),
            separatorEmpty(),
            text("You will soon get started in the Kanto region") | center,
            separatorDouble(),
        });
    });

    /*
    *   This part is about creating an input field for the 
    *   user to enter their name.
    */

    std::string trainer_name {};

    Component input_trainer_name = Input(&trainer_name, "Your name here...");

    Component input_trainer_container = Container::Vertical({
        input_trainer_name,
    });

    Component name_text = Renderer(input_trainer_container,[&] {
        return vbox({
            hbox(text("Enter your name:"), 
            separatorEmpty(), 
            input_trainer_name->Render()) | bold, 
            separatorEmpty(),
        });
    });

    // Displays a log message depending on the 
    // length of the input
    Component warning_message = Renderer([&] {
        int length { static_cast<int>(trainer_name.size())  };

        if (length < 3) {
            return vbox({ 
                text("Name must be at least 3 characters long") | center | color(Color::OrangeRed1),
                separatorEmpty(),
            }) | center;
        } 
        else if (length > 15) {
            return vbox({
                text("Name is too long (max 15 characters)") | center | color(Color::Red),
                separatorEmpty(),
            }) | center;
        }
        else {
            return vbox({
                text("Seems like a valid name :)") | center | color(Color::Green),
                separatorEmpty(),
            }) | center;
        }
    });

    Component validate_button = Button("Enter the world of Pokemon", [&] {
        int length { static_cast<int>(trainer_name.size())  };
        // Exit the menu only when the input is correct
        if(length >= 3 && length <= 15) {
            current_state = GameState::SelectionMenu;
            screen.ExitLoopClosure()();
        }
    }, ButtonOption::Animated(Color::BlueLight));

    Component resume = Renderer([&] {
        return hbox ({
            separatorEmpty(),
            text("Name: " + trainer_name),
            separatorDouble(),
            text("Badges: 0"),
            separatorDouble(),
            text("Win(s): 0") | color(Color::Green),
            separatorDouble(),
            text("Defeat(s): 0") | color(Color::Red),
        });
    });

    Component name_container = Container::Vertical({
        name_text,
        warning_message,
        validate_button | center,
    }) | size(WIDTH, EQUAL, 40);
    
    // Wrapper container. 
    // Contains all component to be rendered.
    Component render = Container::Vertical({
        title,
        intro_text,
        name_container,
    }) | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(render);

    return trainer_name;
}