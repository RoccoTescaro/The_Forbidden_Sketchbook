#include "../hdr/GcBrain.h"
#include "../hdr/Map.h"

GcBrain::GcBrain(uint8_t maxEnergy, uint8_t energy, MovementType type)
    : maxEnergy(maxEnergy), energy(energy)
{
    if(type==MovementType::ASTAR)
        movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
    else
        movementStrategy = std::unique_ptr<PathAlgorithm>(new DigletMovement());
}

bool GcBrain::canAttack(Map &map, float range)
{
    return (Utils::Math::distance(stepQueue.front(), stepQueue.back()) < range && energy>2);
}


sf::Vector2<float> GcBrain::move(Map &map, sf::Sprite &sprite, sf::Vector2<float> pos, const float &dt)
{
    sf::Vector2<float> &nextStep =stepQueue[1];
	sf::Vector2<float> direction=nextStep-pos;
	direction={direction.x/map.getCellDim().x,direction.y/map.getCellDim().y};
	pos+={direction.x*dt*animationSpeed,direction.y*dt*animationSpeed};
	//if(weapon)
	//	weapon->setPosition(pos);
	if(Utils::Math::distance(sprite.getPosition(),nextStep)<3){
		sprite.setPosition(nextStep);
		energy-=1;
		map.move(map.posFloatToInt(stepQueue.front()),map.posFloatToInt(nextStep));
		stepQueue.pop_front();
        return nextStep;
	}else{
        sprite.setPosition(pos);
        return pos;
    }
}

void GcBrain::updateStepQueue( Map &map, const sf::Vector2<float> start, const sf::Vector2<float> target, bool solid)
{
    if(stepQueue.empty()){
		stepQueue=movementStrategy->findPath(map, start, target, solid);
	}
};
