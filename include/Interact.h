#ifndef INTERACT_H
#define INTERACT_H

#include <string>

// Pure abstract class (interface)

class Interact
{
public:
    virtual ~Interact() = default;
    virtual const std::string interactWith() const = 0;
};

#endif