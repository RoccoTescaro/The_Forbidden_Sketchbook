#include "../hdr/TurnSystem.h"

TurnSystem::TurnSystem (Map& map): map(map){

};
//inti

std::shared_ptr<GameCharacter> TurnSystem::getActor(){

    while (turnQueue.top().expired())
        turnQueue.pop();
    if(turnQueue.empty()){
        updateQueue();
    }
    std::weak_ptr<GameCharacter> actor = turnQueue.top();
    turnQueue.pop();
    return actor.lock();

}

bool TurnSystem::isPlayerTurn(){

    return turnQueue.size()==1;
}

void TurnSystem::updateQueue(){

    auto gameCharacters = map.getGameCharacters();
    for(auto &gcs:gameCharacters){
        auto gc=gcs.second;
        if(Config::maxActivationDistance>Utils::Math::distance(map.getPlayer().get()->getPos(),gc.get()->getPos())){
            turnQueue.push(gc);
            //gc.roundReset();
        }
    }
}

void TurnSystem::initQueue(){

    turnQueue.push(map.getPlayer());
}

