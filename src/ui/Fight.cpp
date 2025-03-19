#include "Game.h"
#include "Trainer.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

bool isOpponentMaster(Trainer& opponent) {
    return dynamic_cast<Master*>(&opponent) != nullptr;
}

