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
    int range = actorShr->getWeapon().getRange();
    int energy = actorShr->getEnergy();
    int dmg = actorShr->getWeapon().getAttack();  
    int targetHp; 
    if(map->getGameCharacter(map->posFloatToInt(target)))
        targetHp = map->getGameCharacter(map->posFloatToInt(target))->getHealth();
    bool inRange = (Utils::Math::distance(map->posFloatToInt(actorShr->getPos()),map->posFloatToInt(target))<=range && map->getGameCharacter(map->posFloatToInt(target)));
    std::deque<sf::Vector2<float>> stepQueue = actorShr->getMovementStrategy()->findPath(*map, actorShr->getPos(), target, actorShr->isSolid());
    sf::Vector2<float> newPos = stepQueue.front();
    while (!stepQueue.empty() && energy && !inRange)
    {
        if(map->getGameCharacter(map->posFloatToInt(newPos)))
            break;
        energy-=actorShr->getMovementStrategy()->getMovementCost();
        actionQueue.emplace(0,newPos);
        stepQueue.pop_front();
        inRange = (Utils::Math::distance(map->posFloatToInt(newPos),map->posFloatToInt(target))<=range && map->getGameCharacter(map->posFloatToInt(target)));    //if you can attack u must do it        
        newPos = stepQueue.front();

    }
    while(inRange && energy>=actorShr->getWeapon().getCost() && targetHp)
    {
        //energy-=actorShr->getWeapon()->getCost();
        energy -=actorShr->getWeapon().getCost();
        targetHp=(  (targetHp-dmg)<actorShr->getMaxHealth() ? (targetHp-dmg):0);
        actionQueue.emplace(1,target);

    }
}

void TurnSystem::update(const float &dt)
{
    if(actionQueue.empty())
        return;
   
    auto action = actionQueue.front();
    auto actorShr = actor.lock();
        

    if(!action.actionType){
        actorShr->move(*map, action.target, dt);
        if(Utils::Math::distance(actorShr->getPos(),action.target)<5){
            actionQueue.pop();
            }
    }
    else{
        actorShr->interact(*map, action.target, dt);
        if(actorShr->getWeapon().isAnimationEnded()){
            actionQueue.pop();
            }
        
    }

    if(actionQueue.empty() && !dynamic_cast<Player*>(actorShr.get())) 
        actorShr->setEnergy(0);

}