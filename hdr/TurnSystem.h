#pragma once

#include <memory>
#include "GameCharacter.h"
#include "Map.h"
#include "Utils.h"


class TurnSystem
{
public:

    TurnSystem(Map &map);

    std::shared_ptr<GameCharacter> getActor();
    bool isPlayerTurn();

private:
    struct PriorityCompare
    {
        bool operator()(const std::weak_ptr<GameCharacter> &p1, const std::weak_ptr<GameCharacter> &p2) const
        {
            return (p1.lock()->getPriority() <= p2.lock()->getPriority());
        }
    };

    Map &map;
    std::priority_queue<std::weak_ptr<GameCharacter>,std::vector<std::weak_ptr<GameCharacter>>,PriorityCompare> turnQueue;

    void newRound();

};