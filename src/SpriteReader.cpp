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

} // namespace
