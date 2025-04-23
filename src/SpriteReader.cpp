#include "SpriteReader.h"

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

#include <memory>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

using namespace ftxui;

// Anonymous namespace to encapsulate private class, enum, and functions
namespace {

class RGB {
    public:
        uint8_t r, g, b;

        RGB(uint8_t r, uint8_t g, uint8_t b)
        : r { r }, g { g }, b { b } {}
};

enum class Item {
    Space,
    UpperSquare,
    LowerSquare,
    ColorReset,
    FgColor,
    BgColor,
};

int getDigit(const wchar_t c) {
    switch (c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    default: return -1;
    }
}

/// @brief Check if the character at index i in the string s is an ANSI escape. If
/// it is, increment i by 1 and return true.
/// @param s The string containing the characters to check.
/// @param i The index of the character to check.
/// @return true if the character at index i is an ANSI escape.
bool isAnsiEsc(const std::wstring& s, size_t& i)  {
    if (i < s.size() && s[i] == '\x1b')  {
        ++i;
        return true;
    }
    return false;
}

std::optional<std::pair<size_t, Item>> extractSpaceOrSquare(const std::wstring& s, size_t i) {
    Item item;
    const wchar_t& c { s[i] };

    if (c == ' ') {
        item = Item::Space;
        ++i;
    } else if (c == 9600) { // unicode upper square
        item = Item::UpperSquare;
        ++i;
    } else if (c == 9604) { // unicode lower square
        item = Item::LowerSquare;
        ++i;
    } else {
        // no match found
        return std::nullopt;
    }

    return std::make_pair(i, item);
}

std::optional<std::pair<size_t, Item>> extractColorReset(const std::wstring& s, size_t i) {
    if (isAnsiEsc(s, i) && i < s.size() && s.substr(i, 3) == L"[0m") {
        // we found what we were looking for
        return std::make_pair(i + 3, Item::ColorReset);
    } else {
        // not found
        return std::nullopt;
    }
}

std::optional<std::pair<size_t, RGB>> parseRGB(const std::wstring& s, size_t i) {
    const wchar_t SEP = L';';
    constexpr size_t N { 3 };
    int rgb[N];
    size_t j {};
    size_t start = i;
    size_t total_num_digits {};
    for (auto& c : rgb) {
        c = 0; // initialize at zero
        size_t num_digits {};
        for (size_t k {}; k < N && (start + k) < s.size(); ++k) {
            int digit = getDigit(s[start + k]);
            if (digit != -1) {
                ++num_digits;
                c *= 10; // base 10 numbers
                c += digit;
            } else if (j == N - 1 || s[start + k] == SEP) {
                // the last rgb component has ended or a seperator was reached
                break;
            } else {
                // invalid char
                return std::nullopt;
            }
        }
        if (num_digits == 0) {
            // no digits found before the SEP
            return std::nullopt;
        }
        if (c < 0 || 255 < c) {
            // invalid uint8 value
            return std::nullopt;
        }
        if (j != N - 1 && s[start + num_digits] != SEP) {
            // the separator is missing before the next value
            return std::nullopt;
        }
        // prepare the index for the next iteration of the loop
        total_num_digits += num_digits;
        start += num_digits + 1;
        ++j;
    }
    // all checks passed, we found a valid RGB color
    uint8_t r = (uint8_t)rgb[0];
    uint8_t g = (uint8_t)rgb[1];
    uint8_t b = (uint8_t)rgb[2];
    return std::make_pair(i + total_num_digits + N - 1, RGB(r, g, b));
}

std::optional<std::pair<std::pair<size_t, RGB>, Item>> extractColor(const std::wstring& s, size_t i) {
    const wchar_t SET_TEXT_ATTRIBUTES = L'm';
    if (!isAnsiEsc(s, i)) {
        // ansi esc missing
        return std::nullopt;
    } else if (s.substr(i, 6) == L"[38;2;") {
        // forground color
        auto parsed = parseRGB(s, i + 6);
        if (!parsed.has_value()) return std::nullopt;
        i = parsed.value().first;
        if (i >= s.size() || s[i] != SET_TEXT_ATTRIBUTES) return std::nullopt;
        return std::make_pair(std::make_pair(i + 1, parsed.value().second), Item::FgColor);
    } else if (s.substr(i, 6) == L"[48;2;") {
        // background color
        auto parsed = parseRGB(s, i + 6);
        if (!parsed.has_value()) return std::nullopt;
        i = parsed.value().first;
        if (i >= s.size() || s[i] != SET_TEXT_ATTRIBUTES) return std::nullopt;
        return std::make_pair(std::make_pair(i + 1, parsed.value().second), Item::BgColor);
    } else {
        // unknown content
        return std::nullopt;
    }
}

std::pair<std::vector<Item>, std::vector<RGB>> parseLine(const std::wstring& line) {
    std::vector<Item> itemStorage {};
    std::vector<RGB> colorStorage {};

    /*
        We parse the line piece by piece :
          - when we find something valid, we add it to one of the storage vectors
          - then we update the index to reflect the next position to be parsed
          - we continue until either reaching the end of the line or failing to parse something
    */

    size_t index {};
    while (index < line.size()) {
        {
            auto v = extractColorReset(line, index);
            if (v.has_value()) {
                itemStorage.emplace_back(v.value().second);
                index = v.value().first;
                continue;
            }
        }

        {
            auto v = extractSpaceOrSquare(line, index);
            if (v.has_value()) {
                itemStorage.emplace_back(v.value().second);
                index = v.value().first;
                continue;
            }
        }

        {
            auto v = extractColor(line, index);
            if (v.has_value()) {
                itemStorage.emplace_back(v.value().second);
                colorStorage.emplace_back(v.value().first.second);
                index = v.value().first.first;
                continue;
            }
        }

        // if we get here, it means we failed to parse something
        break;
    }

    return std::make_pair(itemStorage, colorStorage);
}

std::vector<std::pair<std::vector<Item>, std::vector<RGB>>> parseSpriteFile(const std::string& file_path) {
    std::wifstream wif(file_path);

    // set the appropriate locale to read utf-8
    const std::locale LOCALE = std::locale("C.UTF-8");
    wif.imbue(LOCALE);

    std::vector<std::pair<std::vector<Item>, std::vector<RGB>>> data {};

    if(!wif.is_open())  {
        std::cerr << "Error: could not open file " + file_path + " !" << std::endl;
        return data;
    }

    std::wstring line;
    while (std::getline(wif, line)) {
        auto processed_line = parseLine(line);
        data.emplace_back(processed_line);
    }

    wif.close();

    return data;
}

std::vector<std::vector<Color>> convertToColorGrid(const std::vector<std::pair<std::vector<Item>, std::vector<RGB>>>& parsed) {
    std::vector<std::vector<Color>> grid;
    grid.reserve(parsed.size() * 2UL);

    Color fg_color = Color::Black;
    Color bg_color = Color::Black;

    for (const auto& line : parsed) {
        size_t colorIndex {};
        auto items = line.first;
        auto colors = line.second;
        size_t approx_size = items.size() > colors.size() ? items.size() - colors.size() : items.size();
        std::vector<Color> upper_pixels;
        std::vector<Color> lower_pixels;
        upper_pixels.reserve(approx_size);
        lower_pixels.reserve(approx_size);
        for (const auto& elem : items) {
            switch (elem)
            {
            case Item::FgColor:
                fg_color = Color::RGB(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
                ++colorIndex;
                break;
            case Item::BgColor:
                bg_color = Color::RGB(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
                ++colorIndex;
                break;
            case Item::ColorReset:
                fg_color = Color::Black;
                bg_color = Color::Black;
                break;
            case Item::Space:
                upper_pixels.emplace_back(Color::Black);
                lower_pixels.emplace_back(Color::Black);
                break;
            case Item::UpperSquare:
                upper_pixels.emplace_back(fg_color);
                lower_pixels.emplace_back(bg_color);
                break;
            case Item::LowerSquare:
                upper_pixels.emplace_back(bg_color);
                lower_pixels.emplace_back(fg_color);
                break;
            
            default:
                throw std::runtime_error("unreachable");
                break;
            }
        }
        grid.emplace_back(std::move(upper_pixels));
        grid.emplace_back(std::move(lower_pixels));
    }

    return grid;
}

Component createSpriteComponent(const std::vector<std::vector<Color>>& grid) {
    Elements array;
    array.reserve(grid.size() / static_cast<size_t>(2));
    for (size_t y {}; (y + 1) < grid.size(); y += 2) {
        Elements line;
        line.reserve(
            grid[y].size() < grid[y + 1].size() ?
            grid[y].size() : grid[y + 1].size()
        );
        for (size_t x {}; x < grid[y].size() && x < grid[y + 1].size(); ++x) {
            line.push_back(
                text("▀")
                | color(grid[y][x])
                | bgcolor(grid[y + 1][x])
            );
        }
        array.emplace_back(hbox(std::move(line)));
    }

    Component sprite = Renderer([=] {
        return vbox(std::move(array));
    });

    return sprite;
}

std::string getFilePath(const std::string& english_name) {
    std::filesystem::path file_path(SPRITE_DIR);
    std::string file_name {};
    size_t i {};
    while (i < english_name.size()) {
        const char c = english_name[i];
        if (std::isalnum(c)) {
            file_name += std::tolower(c);
            ++i;
        } else if (english_name.substr(i, 4) == "♀") {
            file_name += "-f";
            i += 4;
        } else if (english_name.substr(i, 4) == "♂") {
            file_name += "-m";
            i += 4;
        } else if (english_name.substr(i, 2) == ". ") {
            file_name += '-';
            i += 2;
        } else {
            // skip the character
            ++i;
        }
    }
    file_path.append(file_name);
    return file_path;
}

} // namespace

Component getSpriteForPokemon(const std::string& english_name) {
    std::string file_path = getFilePath(english_name);
    const auto parsed = parseSpriteFile(file_path);
    const auto grid = convertToColorGrid(parsed);
    const Component sprite = createSpriteComponent(grid);
    return sprite;
}
