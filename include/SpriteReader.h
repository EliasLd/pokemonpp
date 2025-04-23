#ifndef SPRITEREADER_H
#define SPRITEREADER_H

#include "ftxui/component/component.hpp"

#include <string_view>

inline constexpr std::string_view SPRITE_DIR = "../data/sprites/small/regular/";

ftxui::Component getSpriteForPokemon(const std::string& english_name);

#endif // SPRITEREADER_H