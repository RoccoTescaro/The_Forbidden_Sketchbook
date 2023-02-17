#pragma once

#include <memory>
#include "GameCharacter.h"
#include "Map.h"
#include "Utils.h"
#include "Serializable.h"


class TurnSystem : public Serializable
{
private:

    struct PriorityCompare
    {
        bool operator()(const std::weak_ptr<GameCharacter>& p1, const std::weak_ptr<GameCharacter>& p2) const
        {
            return (p1.lock().get()->getPriority() <= p2.lock().get()->getPriority());
        }
    };

public:

    TurnSystem(const std::shared_ptr<Map>& map);
    TurnSystem() : map(nullptr) {};

    std::shared_ptr<GameCharacter> getActor();
    bool isPlayerTurn();
    void newRound();

    void serialize(Archive& fs) override;

private:
    static Serializable* create() { return new TurnSystem; };
    static Register registration;

    std::shared_ptr<Map> map;
    std::priority_queue<std::weak_ptr<GameCharacter>,std::vector<std::weak_ptr<GameCharacter>>,PriorityCompare> turnQueue;
};