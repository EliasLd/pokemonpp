#include "SpriteReader.h"

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

} // namespace
