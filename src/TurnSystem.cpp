#include "../hdr/TurnSystem.h"

std::weak_ptr<GameCharacter> TurnSystem::getActor()
{
    if(!turnQueue.empty()){
        while (turnQueue.top().expired())
            turnQueue.pop();
    }
    else
        newRound();
    actor = turnQueue.top();
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

void TurnSystem::turnBuild(sf::Vector2<float> target)
{
    if(!actionQueue.empty())
        return;

    auto actorShr = actor.lock();
    uint8_t range = actorShr->getRange();
    uint8_t energy = actorShr->getEnergy();
    uint8_t dmg = 5;//actorShr->getWeapon().getDmg()  
    uint8_t targetHp; 
    if(map->getGameCharacter(map->posFloatToInt(target)))
        targetHp = map->getGameCharacter(map->posFloatToInt(target))->getHealth();
    bool inRange = (Utils::Math::distance(actorShr->getPos(),target)<=range && map->getGameCharacter(map->posFloatToInt(target)));
    sf::Vector2<float> newPos = actorShr->getPos();

    std::deque<sf::Vector2<float>> stepQueue = actorShr->getMovementStrategy()->findPath(*map, actorShr->getPos(), target, actorShr->isSolid());
    while (!stepQueue.empty() && energy && !inRange)
    {
        newPos = stepQueue.front();
        inRange = (Utils::Math::distance(newPos,target)<=range && map->getGameCharacter(map->posFloatToInt(target)));    //if you can attack u must do it
        energy-=actorShr->getMovementStrategy()->getMovementCost();
        std::unique_ptr<Action> action {new Move(newPos)};
        actionQueue.push(std::move(action));
        stepQueue.pop_front();

        LOG("action type: move \n action target:{1},{2} \n energy left: {3} \n", newPos.x, newPos.y, int(energy) );
        
    }
    LOG(" movement built ");
    while(inRange && energy>1 && targetHp)
    {
        //energy-=actorShr->getWeapon()->getCost();
        energy-=2;
        targetHp-=dmg;
        std::unique_ptr<Action> action {new Attack(target)};
        actionQueue.push(std::move(action));
        LOG("action type: attack \n action target:{1},{2} \n energy left: {3} \n\n", target.x, target.y, int(energy) );

    }
    LOG(" attack built ");


}

void TurnSystem::update(const float &dt)
{

    if(actionQueue.empty())
        return;

    actionQueue.front()->update(*map, actor, dt);
    if(actionQueue.front()->isAnimationEnded(actor))
        actionQueue.pop();

}