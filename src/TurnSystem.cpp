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
    if (!actorShr.get()) newTurn();

    auto mapShr = map.lock();
    if ( actionQueue.empty() ) return;

    auto action = actionQueue.front();

    switch (action.type)
    {
    case Action::Type::Move:
        actorShr->move(*mapShr, action.target, dt);
        if (Utils::Math::distance(actorShr->getPos(), action.target) < Config::epsDistance)
        {   
            actorShr->setPos(action.target);
            actionQueue.pop();
            if (actorShr->getEnergy() == 0 || (actionQueue.empty() && !isPlayerTurn())) newTurn();
        }
        break;
    case Action::Type::Interact:
        auto targetEntity = mapShr->get<GameCharacter>(mapShr->posFloatToInt(action.target));
        if (targetEntity.get())
        {
            actorShr->getWeapon().update(action.target, dt);
            if (actorShr->getWeapon().isAnimationEnded())
            {
                targetEntity->interact(*mapShr, actorShr->getPos());
                actionQueue.pop();
                if (actorShr->getEnergy() == 0 || (actionQueue.empty() && !isPlayerTurn())) newTurn();
            }
        }
        else
        {
            mapShr->get<Tile>(mapShr->posFloatToInt(action.target))->interact(*mapShr, action.target);
            actionQueue.pop();
        }
        break;
    }
}

void TurnSystem::turnBuild(sf::Vector2<float> target)
{
    if (!actionQueue.empty()) return;


    //ACTOR
    auto actorShr = actor.lock();
    uint8_t range = actorShr->getWeapon().getRange();
    int energy = actorShr->getEnergy(); 
    uint8_t dmg = actorShr->getWeapon().getAttack();

    //MAP
    auto mapShr = map.lock();

    //TARGET
    sf::Vector2<int> targetPos = mapShr->posFloatToInt(target);
    GameCharacter* targetGameCharacter = mapShr->get<GameCharacter>(targetPos).get();
    Tile* targetTile = mapShr->get<Tile>(targetPos).get();
    int targetHp = 0; 
    if (targetGameCharacter)
        targetHp = targetGameCharacter->getHealth();

    //player turn will be skipped if he clicks on himself
    if(targetPos == mapShr->posFloatToInt(actorShr->getPos())){newTurn(); return;} 

    //MOVEMENT
    bool inRange = (Utils::Math::distance(mapShr->posFloatToInt(actorShr->getPos()), targetPos) <= range && (targetGameCharacter||targetTile));

    std::deque<sf::Vector2<float>> stepQueue = actorShr->getMovementStrategy()->findPath(*mapShr, actorShr->getPos(), target, actorShr->isSolid());
    if(!stepQueue.empty())
    {
    
        sf::Vector2<float> newPos = stepQueue.front();

        while (!stepQueue.empty() && energy && !inRange)
        {
            if (mapShr->get<GameCharacter>(mapShr->posFloatToInt(newPos)).get()) break; //check if there is a character in the way
        
            actionQueue.emplace(Action::Type::Move, newPos);
            stepQueue.pop_front();

            energy -= actorShr->getMovementStrategy()->getMovementCost();
            inRange = (Utils::Math::distance(mapShr->posFloatToInt(newPos), targetPos) <= range && (targetGameCharacter||targetTile)); //if you can attack u must do it        
        
            LOG("Entity: {4}, move to target: {{1},{2}}, remaing energy {3}", newPos.x, newPos.y, energy, typeid(*actorShr.get()).name());

            if (stepQueue.size() >= 1) newPos = stepQueue.front();
        }
    }

    //INTERACT
    while (inRange && energy >= actorShr->getWeapon().getCost() && (targetHp > 0 || targetTile))
    {
        energy -= actorShr->getWeapon().getCost();
        targetHp -= dmg;
        actionQueue.emplace(Action::Type::Interact, target);

        LOG("Entity: {4}, interact with target: {{1},{2}}, remaing energy {3}", target.x, target.y, energy, typeid(*actorShr.get()).name());

        if (isPlayerTurn()) break; //if it's the player's turn, he can decide how to distribute damage between targets
    }
}

void TurnSystem::newRound()
{
    auto& gameCharacters = map.lock()->getGameCharacters();

    for (auto& gameCharacter : gameCharacters)
        if (Config::maxActivationDistance > Utils::Math::distance(map.lock()->get<Player>()->getPos(), gameCharacter.second->getPos()))
            turnQueue.emplace(gameCharacter.second);
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
    actor.lock()->turnReset();
    turnQueue.pop();
}

void TurnSystem::serialize(Archive& fs) 
{
    if (fs.getMode() == Archive::Save)
    {
        uint32_t size = turnQueue.size();
        fs.serialize(size);
        //the deserialization will regenerate the queue based on map but when newTurn will be called it will reset
        //the energy of the actor. So we need to save the energy of the actor before in order to re-reload it
        int8_t energy = actor.lock()->getEnergy();
        fs.serialize(energy);
    }
    else 
    {
        while (!turnQueue.empty()) //clear the queue to load it properly
            turnQueue.pop();

        while(!actionQueue.empty())
            actionQueue.pop();
        
        uint32_t size;
        fs.serialize(size);

        //its important to call newRound before newTurn because it might happens that size will be 0 (player turn)
        //this means that the while loop will never cycle, newTurn will never be called and actor never reloaded making crash
        newRound(); 
        while (size != turnQueue.size())
            newTurn();

        int8_t energy;
        fs.serialize(energy);
        actor.lock()->setEnergy(energy); 
    }
}

