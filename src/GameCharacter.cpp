#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

void GameCharacter::update(Map &map, const float &dt)
{
	if(!stepQueue.empty() && energy>0){
		bool apSpent=false;

			if(Utils::Math::distance(stepQueue.front(), stepQueue.back()) < getRange() && energy>2 && map.getGameCharacter(map.posFloatToInt(stepQueue.back()))){
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
							
			}else{
				apSpent=true;
				sf::Vector2<float> pos =getPos();
				sf::Vector2<float> &nextStep =stepQueue[1];
				sf::Vector2<float> direction=nextStep-pos;
				direction={direction.x/map.getCellDim().x,direction.y/map.getCellDim().y};
				pos+={direction.x*dt*speed,direction.y*dt*speed};
				//if(weapon)
				//	weapon->setPosition(pos);
				if(Utils::Math::distance(sprite.getPosition(),nextStep)<3){
					sprite.setPosition(nextStep);
					energy-=1;
					map.move(map.posFloatToInt(stepQueue.front()),map.posFloatToInt(nextStep));
					stepQueue.pop_front();
				}else{
					sprite.setPosition(pos);
				}
				//if(weapon)
				//	weapon->setPosition(newPos);
				if(stepQueue.size()==1)
                    {   stepQueue.clear();  } 
			
		}

		if(!apSpent){
			energy=0;
		}
	}
}



void GameCharacter::execute(GameCharacter &gameCharacter, Map &map)
{

}


void GameCharacter::updateStepQueue( Map &map, const sf::Vector2<float> target)
{
	if(stepQueue.empty()){
		stepQueue=movementStrategy->findPath(map, getPos(), target, isSolid());
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
		texture->loadFromFile(Config::meleeTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,128.f/textureRect.height);


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
		texture->loadFromFile(Config::batTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 115,0,1910,1070 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(104.f/textureRect.width,48.f/textureRect.height);


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
		texture->loadFromFile(Config::rangedTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,	128.f/textureRect.height);


    movementStrategy = std::unique_ptr<PathAlgorithm>(new DigletMovement());

}
