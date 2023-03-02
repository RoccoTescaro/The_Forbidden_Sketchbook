#include "../hdr/Action.h"

void Move::update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) 
{

    auto actorShr = actor.lock();
	sf::Vector2<float> pos = actorShr->getPos();
	sf::Vector2<float> direction=target-pos;
	pos+={Utils::Math::normalize(direction)*dt*speed};//todo if targethas negative coords it will get a wrong starting pos
	//if(weapon)
	//	weapon->setPosition(pos);
	if(!map.getGameCharacter(map.posFloatToInt(target))){
		actorShr->subEnergy(actorShr->getMovementStrategy()->getMovementCost());
		LOG("from:{1},{2} to:{3},{4}\n", pos.x,pos.y,target.x,target.y );
        map.move(map.posFloatToInt(pos),map.posFloatToInt(target));
	}
	actorShr->setPos(pos);
    if(Utils::Math::distance(pos,target)<10)
        actorShr->setPos(target);

	
	//if(weapon)
	//	weapon->setPosition(newPos);
}

void Attack::update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) 
{
    
}

void Interact::update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) 
{
    
}