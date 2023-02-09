#include "../hdr/TurnSystem.h"

TurnSystem::TurnSystem (Map& map): map(map){

};
//inti

std::weak_ptr<GameCharacter> TurnSystem::getActor(){

    std::weak_ptr<GameCharacter> actor = gameCharacterQueue.top();

    while (actor.expired())
    {
        gameCharacterQueue.pop();
        std::weak_ptr<GameCharacter> actor = gameCharacterQueue.top();
    }
    gameCharacterQueue.pop();
    
    if(gameCharacterQueue.empty()){
        updateQueue();
        std::weak_ptr<GameCharacter> actor = gameCharacterQueue.top();
    }

    return actor;

}

bool TurnSystem::isPlayerTurn(){

    return gameCharacterQueue.size()==1;
}

void TurnSystem::updateQueue(){

    auto gameCharacters = map.getGameCharacters();
    for(auto &gcs:gameCharacters){
        auto gc=gcs.second;
        if(Config::maxActivationDistance>Utils::Math::distance(map.getPlayer().get()->getPos(),gc.get()->getPos())){
            gameCharacterQueue.push(gc);
            //gc.roundReset();
        }
    }
}

void TurnSystem::initQueue(){

    gameCharacterQueue.push(map.getPlayer());
}

