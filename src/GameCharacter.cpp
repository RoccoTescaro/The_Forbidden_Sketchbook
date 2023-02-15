#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

void GameCharacter::update(Map &map, const float &dt)
{
	if(!stepQueue.empty()&&this->getEnergy()>0){
		bool apSpent=false;
			if(stepQueue.size()>1){
				if(isInRange(map)){
					if(getEnergy()>=2){

						apSpent=true;
						/*if(weapon){
							if(weapon->animation(dt,stepQueue.back(), map.getCellDim())){
								this->ap-=2;
								map.get<GameCharacter>(map.worldCoordToPos(stepQueue.back()))->execute(weapon,map,turnSystem);
								if(!map.get<GameCharacter>(map.worldCoordToPos(stepQueue.back())))
									stepQueue.clear();
							}
						}else{
							this->energy-=2;
							map.getGameCharacter(map.worldCoordToPos(stepQueue.back()))->execute(nullptr,map,turnSystem);
							if(!map.get<GameCharacter>(map.worldCoordToPos(stepQueue.back())))
									stepQueue.clear();
						}*/
						
					}
				}else{
					apSpent=true;
					sf::Vector2<float> &nextStep =stepQueue[1];
					sf::Vector2<float> pos = this->getPos();
					sf::Vector2<float> direction=nextStep-pos;
					if(!(Utils::Math::distance(pos,nextStep)>64)){
						direction={direction.x/map.getCellDim().x,direction.y/map.getCellDim().y};
						pos+={direction.x*dt*300,direction.y*dt*300};
						sprite.setPosition(pos);
						//if(weapon)
						//	weapon->setPosition(pos);
						if(Utils::Math::distance(sprite.getPosition(),nextStep)<3){
							sprite.setPosition(nextStep);
							this->energy-=1;
							map.move(map.posFloatToInt(stepQueue.front()),map.posFloatToInt(nextStep));
							stepQueue.pop_front();
						}
					}else{
            			stepQueue.pop_front();
					}
					if(stepQueue.size()==1){
						stepQueue.clear();}
				}
			}
		if(!apSpent){
			this->energy=0;}
	}
							std::cout<<int(energy)<<std::endl;

						std::cout<<"\n"<<std::endl;

}

void GameCharacter::execute(GameCharacter &gameCharacter, Map &map)
{

}

bool GameCharacter::isInRange(Map &map) const
{
	return false;
}

void GameCharacter::updateStepQueue( Map &map, const sf::Vector2<float> target)
{
	if(stepQueue.empty()){
		stepQueue=movementStrategy->findPath(map, getPos(), target, isSolid());
	for(auto &n:stepQueue){
		std::cout<<n.x<<"-"<<n.y<<std::endl;
	}
	}
}

//PLAYER

Player::Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB)
	: GameCharacter(100,health,20,energy,0) 
{

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile(Config::playerTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

	//FILTER
	filterColor = sf::Color(filterColorR, filterColorG, filterColorB, 255);
	
    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}


//MELEE

Melee::Melee(uint8_t health, uint8_t energy)
	: GameCharacter(30, health, 10, energy, 2)
{
	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../img/Melee.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
   	movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}

//BAT

Bat::Bat(uint8_t health, uint8_t energy)
	: GameCharacter(10, health, 30, energy, 3)
{
	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../img/Bat.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}

//RANGED

Ranged::Ranged(uint8_t health, uint8_t energy)
	: GameCharacter(5, health, 50, energy,1), animationDuration(1), animationTime(0)
{

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../img/Ranged.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new DigletMovement());
}

void Ranged::update(Map &map, const float &dt)
{

}
