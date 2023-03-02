#pragma once

#include <memory>
#include "GameCharacter.h"
#include "Map.h"
#include "Utils.h"
#include "Action.h"


class TurnSystem : public Serializable
{
private:

    struct PriorityCompare
    {
        bool operator()(const std::weak_ptr<GameCharacter>& p1, const std::weak_ptr<GameCharacter>& p2) const
        {
            return ( (p1.expired() || p2.expired()) || p1.lock().get()->getPriority() <= p2.lock().get()->getPriority());
        }
    };

    using ActionQueue = std::queue<std::unique_ptr<Action>>;
    using TurnQueue = std::priority_queue<std::weak_ptr<GameCharacter>, std::vector<std::weak_ptr<GameCharacter>>, PriorityCompare>;
public:

    void init(Map& map);
  
    std::weak_ptr<GameCharacter> getActor();
    
    bool isPlayerTurn();
    
    void newRound();

    void serialize(Archive& arc);

    void turnBuild(sf::Vector2<float> target);

    void update(const float &dt);

    inline bool isActionQueueEmpty(){   return actionQueue.empty(); };

private:
    static Serializable* create() { return new TurnSystem; };
    static Register registration;

    std::shared_ptr<Map> map;

    TurnQueue turnQueue;
    std::weak_ptr<GameCharacter> actor;

    ActionQueue actionQueue;
};