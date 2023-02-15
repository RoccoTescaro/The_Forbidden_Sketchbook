#include "../hdr/TurnSystem.h"

TurnSystem::TurnSystem (Map& map): map(map)
{
    //newRound();
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
    return turnQueue.size() == 0;
}

void TurnSystem::newRound()
{
    auto gameCharacters = map.getGameCharacters();

    for(auto &gcs : gameCharacters)
    {
        auto gc = gcs.second;

        if(Config::maxActivationDistance > Utils::Math::distance(map.getPlayer()->getPos(),gc->getPos()))
        {
            turnQueue.push(gc);
            gc->turnReset();
        }

    }
}

