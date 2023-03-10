#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"
#include "../hdr/Player.h"

void GameCharacter::interact(Map &map, sf::Vector2<float> target, const float &dt)
{

	auto actor = map.get<GameCharacter>(map.posFloatToInt(target));
	setHealth(health - actor->getWeapon().getAttack());
	actor->setEnergy(actor->getEnergy() - actor->getWeapon().getCost());

	if(health <= 0 && map.posFloatToInt(getPos()) != map.posFloatToInt(map.get<Player>()->getPos()))
		map.remove(map.posFloatToInt(getPos()));	
}

void GameCharacter::move(Map &map, sf::Vector2<float> target, const float &dt)
{
	sf::Vector2<float> pos = getPos();
	sf::Vector2<float> direction = target - pos;
	if(!map.get<GameCharacter>(map.posFloatToInt(target)).get())
	{
		energy -= movementStrategy->getMovementCost();
        map.move(map.posFloatToInt(pos),map.posFloatToInt(target));
	}
	pos += {Utils::Math::normalize(direction)*dt*speed};
	setPos(pos);
}

