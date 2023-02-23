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
    this->map = static_cast<std::shared_ptr<Map>>(&map);//std::make_shared<Map>(map);
}

void TurnSystem::serialize(Archive& fs) //#TODO test with less entities
{
    if (fs.getMode() == Archive::Save)
    {
        uint32_t size = turnQueue.size();
        size++;
        fs.serialize(size);
        //LOG("onSave turnQueue size: {1}", size);
    }
    else 
    {
        while (!turnQueue.empty()) //clear the queue to load it properly
            turnQueue.pop();
        
        uint32_t size;
        fs.serialize(size);

        newRound();
        while (size != turnQueue.size())
            turnQueue.pop();

        //LOG("onLoad turnQueue size: {1}", size);
    }
}