#include "Game.h"
#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

bool isOpponentMaster(Trainer& opponent) {
    return dynamic_cast<Master*>(&opponent) != nullptr;
}

void Fight(ftxui::ScreenInteractive& screen, Player& player, Trainer& opponent) {
    screen.Clear();

    Component fight_title = Renderer([&] {
        return hbox ({
            text("Pokemon trainer " + player.getName()),
            text(" VS ") | color(Color::Green),
            text(opponent.toString()), 
        });
    });

    Component exit_button = Button("Exit (tmp)", [&] {
        screen.ExitLoopClosure()();
    });

    Component renderer = Container::Vertical({
        fight_title | center,
        exit_button | center,
    }) | center | borderDouble;

    screen.Loop(renderer);
}
