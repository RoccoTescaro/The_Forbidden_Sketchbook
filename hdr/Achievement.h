#pragma once
#include <string>
#include "Hud.h"

class Achievement
{
public:
    Achievement(std::string name, std::string description, uint8_t goal = 1)
        : name(name), description(description), progress(0), goal(goal), hud(nullptr) {}

    Achievement() {};

    inline void attach(Hud* hud)
    {
        this->hud = hud;
    }

    inline void notify() const //in this case notify only one observer but it might be refactored to a list of observers
    {
        if (progress == goal)
            hud->showAchievement(name, description);
    }

    uint8_t progress;

private:
    std::string name;
    std::string description;
    uint8_t goal;
    Hud* hud;
};