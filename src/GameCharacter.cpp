#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

void GameCharacter::update(Map &map, const float &dt)
{
	if(gcBrain.ready()){
		bool apSpent=false;

		if(gcBrain.canExecute()){
			if(gcBrain.canAttack(map, getRange())){
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
				sf::Vector2<float>newPos = gcBrain.move(map, sprite, getPos(), dt);
				//if(weapon)
				//	weapon->setPosition(newPos);
				gcBrain.queueClearCheck();
			}
		}

		if(!apSpent){
			gcBrain.setEnergy(0);
		}
	}
}



void GameCharacter::execute(GameCharacter &gameCharacter, Map &map)
{

}


void GameCharacter::updateStepQueue( Map &map, const sf::Vector2<float> target)
{
	gcBrain.updateStepQueue(map,getPos(),target,isSolid());
}

//PLAYER

Player::Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB)
	: GameCharacter(100,health,20,energy,0,GcBrain::ASTAR)
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

}


//MELEE

Melee::Melee(uint8_t health, uint8_t energy)
	: GameCharacter(30, health, 10, energy, 2,GcBrain::ASTAR)
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
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);

}

//BAT

Bat::Bat(uint8_t health, uint8_t energy)
	: GameCharacter(10, health, 30, energy, 3,GcBrain::ASTAR)
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
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);

}

//RANGED

Ranged::Ranged(uint8_t health, uint8_t energy)
	: GameCharacter(5, health, 50, energy,1,GcBrain::DIGLETMOVEMENT), animationDuration(1), animationTime(0)
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
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);

}

void Ranged::update(Map &map, const float &dt)
{

}
