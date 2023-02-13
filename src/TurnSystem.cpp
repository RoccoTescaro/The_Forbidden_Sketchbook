#include "../hdr/TurnSystem.h"

TurnSystem::TurnSystem (Map& map): map(map){
newRound();
};
//inti

std::shared_ptr<GameCharacter> TurnSystem::getActor(){

    while (turnQueue.top().expired())
        turnQueue.pop();
    if(turnQueue.empty()){
        newRound();
    }
    std::weak_ptr<GameCharacter> actor = turnQueue.top();
    turnQueue.pop();
    std::cout<<actor.lock().get()->getPos().x<<"-"<<actor.lock().get()->getPos().y<<std::endl;
    return actor.lock();

}

bool TurnSystem::isPlayerTurn(){

    return turnQueue.size()==0;
}

void TurnSystem::newRound(){

    auto gameCharacters = map.getGameCharacters();
    for(auto &gcs:gameCharacters){
        auto gc=gcs.second;
        if(Config::maxActivationDistance>Utils::Math::distance(map.getPlayer().get()->getPos(),gc.get()->getPos())){
            turnQueue.push(gc);
            gc.get()->turnReset();
        }
    }
}

