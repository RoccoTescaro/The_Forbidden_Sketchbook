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
    auto& gameCharacters = map.lock()->getGameCharacters();

    for(auto &gameCharactersType : gameCharacters)
    {
        for (auto& gameCharacterPair : gameCharactersType.second) 
        {
            auto& gameCharacter = gameCharacterPair.second;
            if(Config::maxActivationDistance > Utils::Math::distance(map.lock()->get<Player>()->getPos(), gameCharacter->getPos()))
            {
                turnQueue.emplace(gameCharacter);
                gameCharacter->turnReset();
            }
        }

    }
}

void TurnSystem::init(std::shared_ptr<Map> map)
{
    this->map = map;
}

void TurnSystem::serialize(Archive& fs) 
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