#include "TypeStats.h"

// Here are defined all weaknesses and resistances per type

namespace TypeStats 
{
    const std::unordered_map<std::string, std::vector<std::string>> type_weaknesses = {
        { "Feu", { "Eau", "Roche", "Sol" } },
        { "Eau", { "Plante", "Électrik" } },
        { "Plante", { "Feu", "Glace", "Poison", "Vol", "Insecte" } },
        { "Électrik", { "Sol" } },
        { "Glace", { "Feu", "Combat", "Roche", "Acier" } },
        { "Combat", { "Vol", "Psy", "Fée" } },
        { "Poison", { "Sol", "Psy" } },
        { "Sol", { "Eau", "Plante", "Glace" } },
        { "Vol", { "Électrik", "Glace", "Roche" } },
        { "Psy", { "Insecte", "Spectre", "Ténèbres" } },
        { "Insecte", { "Feu", "Vol", "Roche" } },
        { "Roche", { "Eau", "Plante", "Combat", "Sol", "Acier" } },
        { "Spectre", { "Spectre", "Ténèbres" } },
        { "Dragon", { "Glace", "Dragon", "Fée" } },
        { "Ténèbres", { "Combat", "Insecte", "Fée" } },
        { "Acier", { "Feu", "Combat", "Sol" } },
        { "Fée", { "Poison", "Acier" } }
    };

    const std::unordered_map<std::string, std::vector<std::string>> type_resistances = {
        { "Feu", { "Plante", "Glace", "Insecte", "Acier", "Fée" } },
        { "Eau", { "Feu", "Eau", "Glace", "Acier" } },
        { "Plante", { "Eau", "Sol", "Roche" } },
        { "Électrik", { "Vol", "Acier", "Électrik" } },
        { "Glace", { "Glace" } },
        { "Combat", { "Roche", "Insecte", "Ténèbres" } },
        { "Poison", { "Plante", "Fée", "Combat", "Poison", "Insecte" } },
        { "Sol", { "Poison", "Roche" } },
        { "Vol", { "Plante", "Combat", "Insecte" } },
        { "Psy", { "Combat", "Psy" } },
        { "Insecte", { "Plante", "Combat", "Sol" } },
        { "Roche", { "Feu", "Vol", "Poison", "Normal" } },
        { "Spectre", { "Poison", "Insecte" } },
        { "Dragon", { "Feu", "Eau", "Électrik", "Plante" } },
        { "Ténèbres", { "Spectre", "Psy", "Ténèbres" } },
        { "Acier", { "Normal", "Plante", "Glace", "Vol", "Psy", "Insecte", "Roche", "Dragon", "Acier", "Fée" } },
        { "Fée", { "Combat", "Insecte", "Ténèbres", "Dragon" } }
    };
}
