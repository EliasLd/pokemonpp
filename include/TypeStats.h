#ifndef TYPESTATS_H
#define TYPESTATS_H

#include <unordered_map>
#include <vector>
#include <string>

namespace TypeStats
{
    extern const std::unordered_map<std::string, std::vector<std::string>> type_weaknesses;
    extern const std::unordered_map<std::string, std::vector<std::string>> type_resistances;
}

#endif