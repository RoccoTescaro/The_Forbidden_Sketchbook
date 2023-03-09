#include "../hdr/TurnSystem.h"

void TurnSystem::init(std::shared_ptr<Map> map)
{
    this->map = map;
    while (!turnQueue.empty())
        turnQueue.pop();
    while (!actionQueue.empty())
		actionQueue.pop();
}

void TurnSystem::update(const float& dt)
{
    auto actorShr = actor.lock();
    auto mapShr = map.lock();
    if (!actorShr.get() || actorShr->getEnergy() == 0 ) newTurn();
    if ( actionQueue.empty() ) return;

    auto action = actionQueue.front();

    switch (action.type)
    {
    case Action::Type::Move:
        actorShr->move(*mapShr, action.target, dt);
        if (Utils::Math::distance(actorShr->getPos(), action.target) < Config::epsDistance) actionQueue.pop(); 
        break;
    case Action::Type::Interact:
        auto targetEntity = mapShr->get<GameCharacter>(mapShr->posFloatToInt(action.target));
        if (targetEntity)
        {
            actorShr->getWeapon().update(action.target, dt);
            if (actorShr->getWeapon().isAnimationEnded())
            {
                targetEntity->interact(*mapShr, actorShr->getPos(), dt);
                actionQueue.pop();
            }
        }
        else;
            //mapShr->get<Tile>(mapShr->posFloatToInt(action.target))->interact();
        break;
    }

    if (actionQueue.empty() && !isPlayerTurn())
        actorShr->setEnergy(0);

}

void TurnSystem::turnBuild(sf::Vector2<float> target)
{
    if (!actionQueue.empty()) return;

    //ACTOR
    auto actorShr = actor.lock();
    uint8_t range = actorShr->getWeapon().getRange();
    int energy = actorShr->getEnergy(); //casted to int to allow underflow
    uint8_t dmg = actorShr->getWeapon().getAttack();

    //MAP
    auto mapShr = map.lock();

    //TARGET
    sf::Vector2<int> targetPos = mapShr->posFloatToInt(target);
    GameCharacter* targetEntity = mapShr->get<GameCharacter>(targetPos).get();
    int targetHp = 0; //casted to int to allow underflow
    if (targetEntity)
        targetHp = targetEntity->getHealth();

    //MOVEMENT
    bool inRange = (Utils::Math::distance(mapShr->posFloatToInt(actorShr->getPos()), targetPos) <= range && targetEntity);

    std::deque<sf::Vector2<float>> stepQueue = actorShr->getMovementStrategy()->findPath(*mapShr, actorShr->getPos(), target, actorShr->isSolid());
    sf::Vector2<float> newPos = stepQueue.front();

    while (!stepQueue.empty() && energy && !inRange)
    {
        if (mapShr->get<GameCharacter>(mapShr->posFloatToInt(newPos)).get()) break; //check if there is a character in the way

        actionQueue.emplace(Action::Type::Move, newPos);
        stepQueue.pop_front();

        energy -= actorShr->getMovementStrategy()->getMovementCost();
        inRange = (Utils::Math::distance(mapShr->posFloatToInt(newPos), targetPos) <= range && targetEntity); //if you can attack u must do it        
        if (stepQueue.size() >= 1) newPos = stepQueue.front();
    }

    //INTERACT
    while (inRange && energy >= actorShr->getWeapon().getCost() && targetHp > 0)
    {
        energy -= actorShr->getWeapon().getCost();
        targetHp = targetHp - dmg;
        actionQueue.emplace(Action::Type::Interact, target);
    }
}

void TurnSystem::newRound()
{
    auto& gameCharacters = map.lock()->getGameCharacters();

    for (auto& gameCharacter : gameCharacters)
    {
        if (Config::maxActivationDistance > Utils::Math::distance(map.lock()->get<Player>()->getPos(), gameCharacter.second->getPos()))
        {
            turnQueue.emplace(gameCharacter.second);
            gameCharacter.second->turnReset();
        }
    }
}

void TurnSystem::newTurn()
{
    if(!turnQueue.empty()){
        while (turnQueue.top().expired())
            turnQueue.pop();
    }
    else
        newRound();

    actor = turnQueue.top();
    turnQueue.pop();
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

