#include "SelectionMenu.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

#include <unordered_map>

using namespace ftxui;

// Interactive pokemon selection menu
std::vector<std::shared_ptr<Pokemon>> SelectionMenu(
    ScreenInteractive& screen, 
    const std::unordered_map<std::string, std::shared_ptr<Pokemon>>& pokemon_map)
{
    if(pokemon_map.empty())
        throw std::runtime_error("Error: no pokemon loaded.");

    std::vector<std::shared_ptr<Pokemon>> pokemon_list;
    for (const auto& pair : pokemon_map) {
        pokemon_list.push_back(pair.second);
    }
    
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

    size_t length { pokemon_list.size() };
    bool* states = new bool[length];
    for (size_t i {0} ; i < length ; ++i) {
        states[i] = false;
        checkbox_container->Add(Checkbox(pokemon_list.at(i)->toString(), &states[i]));
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

        long nb_selected = std::count(states, states + length, true);
    
        if (nb_selected < 6) {
            return vbox({ 
                text("Too few Pokemons selected (" + std::to_string(nb_selected) + ")") | center | bgcolor(Color::OrangeRed1),
                separator(),
            });
        } 
        else if (nb_selected > 6) {
            return vbox({ 
                text("Too many Pokemons selected (" + std::to_string(nb_selected) + ")") | center | bgcolor(Color::Red1),
                separator(),
            });
        } 
        else {
            return vbox({ 
                text("Good") | center | bgcolor(Color::Green1),
                separator(),
            }) ;
        }
    });

    auto validate_button = Button("Validate Choices", [&] {

        long nb_selected = std::count(states, states + length, true);

        if (nb_selected == 6) {

            selected_pokemons.clear();
            // Get the chosen pokemons
            for(size_t i {0} ; i < length ; ++i) {
                if(states[i]) 
                    selected_pokemons.push_back(pokemon_list.at(i));
            }
            
            screen.ExitLoopClosure()();
        }
    }, ButtonOption::Animated());

    auto sprite = Renderer([&] {
        int i = checkbox_container->ActiveChild()->Index();
        if (0 <= i && i < length) {
            return pokemon_list.at(i)->getSprite()->Render() | center;
        }
        return vbox({ text("index out of range") });
    });

    // Wrapping container, displays all the menu
    Component container = Container::Vertical({
        title,
        scrollable_checkbox,
        sprite,
        warning_message,
        validate_button,
    }) | border | center | bgcolor(Color::RGB(0, 0, 0));

    screen.Loop(container);

    // Free allocated memory before exiting
    delete[] states;

    return selected_pokemons;
}
