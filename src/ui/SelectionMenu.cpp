#include "SelectionMenu.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

// Interactive pokemon selection menu
std::vector<std::shared_ptr<Pokemon>> SelectionMenu(ScreenInteractive& screen, std::vector<std::shared_ptr<Pokemon>>& pokemon_list)
{
    if(pokemon_list.empty())
        throw std::runtime_error("Error: no pokemon loaded.");

    // Collects all the entries
    std::vector<std::string> pokemon_choices {};
    for (const auto& p: pokemon_list)
        pokemon_choices.push_back(p->toString());
    
    std::vector<std::shared_ptr<Pokemon>> selected_pokemons {};

    Component title = Renderer([&] {
        return vbox ({
            text("Select 6 Pokemons") | bold | center,
            separator(),
        });
    });

    /*
        Creates a checkbox containing all
        possible entries.
    */

    auto checkbox_container = Container::Vertical({});

    bool* states = new bool[pokemon_choices.size()];
    int length { static_cast<int>(pokemon_choices.size())};
    for (size_t i {0} ; i < length ; ++i) {
        states[i] = false;
        checkbox_container->Add(Checkbox(pokemon_choices[i], &states[i]));
    }

    // Makes the checkbox scrollable with max 10 entries displayed
    Component scrollable_checkbox = Renderer(checkbox_container, [&] {
        return vbox ({
            checkbox_container->Render()
            | vscroll_indicator
            | frame
            | size(HEIGHT, LESS_THAN, 10),
            separator(),
        });
    });

    // Informs the user on how many pokemons they have to choose.
    // By default, they have to choose 6 pokemons
    Component warning_message = Renderer([&] {

        long nb_selected = std::count(states, states + pokemon_choices.size(), true);
    
        if (nb_selected < 6) {
            return vbox({ 
                text("Too few Pokemons selected (" + std::to_string(nb_selected) + ")") | center | bgcolor(Color::OrangeRed1),
                separator(),
            });
        } 
        else if (nb_selected > 6) {
            return vbox({ 
                text("Too many Pokemons selected (" + std::to_string(nb_selected) + ")") | center | bgcolor(Color::Red),
                separator(),
            });
        } 
        else {
            return vbox({ 
                text("Good") | center | bgcolor(Color::Green),
                separator(),
            }) ;
        }
    });

    auto validate_button = Button("Validate Choices", [&] {

        long nb_selected = std::count(states, states + pokemon_choices.size(), true);

        if (nb_selected == 6) {

            selected_pokemons.clear();
            // Get the chosen pokemons
            for(size_t i {0} ; i < length ; ++i) {
                if(states[i]) 
                    selected_pokemons.push_back(pokemon_list[i]);
            }
            
            // Free allocated memory and exit menu
            delete[] states;
            screen.ExitLoopClosure()();
        }
    });

    // Wrapping container, displays all the menu
    Component container = Container::Vertical({
        title,
        scrollable_checkbox,
        warning_message,
        validate_button,
    }) | border | center | bgcolor(Color::Black);

    screen.Loop(container);

    return selected_pokemons;
}
