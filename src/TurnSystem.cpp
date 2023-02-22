#include "../hdr/TurnSystem.h"

std::weak_ptr<GameCharacter> TurnSystem::getActor()
{
    while (!turnQueue.empty() && turnQueue.top().expired())
        turnQueue.pop();

    if (turnQueue.empty())
        newRound();

    std::weak_ptr<GameCharacter> actor = turnQueue.top();
    turnQueue.pop();

    return actor;
}

bool TurnSystem::isPlayerTurn()
{
    return turnQueue.empty();
}

void TurnSystem::newRound()
{
    auto& gameCharacters = map->getGameCharacters();

    for(auto &gcs : gameCharacters)
    {
        auto gc = gcs.second;

        if(Config::maxActivationDistance > Utils::Math::distance(map->getPlayer()->getPos(), gc->getPos()))
        {
            turnQueue.emplace(gc);
            gc->turnReset();
        }

    }
}

void TurnSystem::init(Map& map) 
{
    this->map = std::make_shared<Map>(map);
    newRound();
}

void TurnSystem::serialize(Archive& fs) 
{
    uint32_t size = turnQueue.size();
    fs.serialize(size);
    newRound();
    while (size != turnQueue.size())
        turnQueue.pop();
}