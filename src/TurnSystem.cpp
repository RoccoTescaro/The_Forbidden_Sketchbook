#include "../hdr/TurnSystem.h"

TurnSystem::TurnSystem(const std::shared_ptr<Map>& map)
{
    this->map = map;
};

std::shared_ptr<GameCharacter> TurnSystem::getActor()
{

    while (!turnQueue.empty() && turnQueue.top().expired())
        turnQueue.pop();

    if(turnQueue.empty())
        newRound();
    
    std::weak_ptr<GameCharacter> actor = turnQueue.top();
    turnQueue.pop();

    return actor.lock();

}

bool TurnSystem::isPlayerTurn()
{
    return turnQueue.empty();
}

void TurnSystem::newRound()
{
    auto gameCharacters = map->getGameCharacters();

    for(auto &gcs : gameCharacters)
    {
        auto gc = gcs.second;

        if(Config::maxActivationDistance > Utils::Math::distance(map->getPlayer()->getPos(),gc->getPos()))
        {
            turnQueue.push(gc);
            gc->turnReset();
        }

    }
}

void TurnSystem::serialize(Archive& fs) 
{
    fs.serialize(map);
    uint32_t size = turnQueue.size();
    fs.serialize(size);
    newRound();
    while (turnQueue.size() != size) //might be size+1
        turnQueue.pop();
}
