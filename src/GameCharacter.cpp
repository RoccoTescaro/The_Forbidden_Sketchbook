#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

//todo remove subEnergy
void GameCharacter::interact(Map &map, sf::Vector2<float> target, const float &dt)
{
	weapon.update(dt,target);
	if(weapon.isAnimationEnded()){
		map.getGameCharacter(map.posFloatToInt(target))->subHealth(weapon.getAttack());
		subEnergy(weapon.getCost());
	}

}

void GameCharacter::move(Map &map, sf::Vector2<float> target, const float &dt)
{

	sf::Vector2<float> pos = getPos();
	sf::Vector2<float> direction=target-pos;
	if(!map.getGameCharacter(map.posFloatToInt(target))){
		subEnergy(getMovementStrategy()->getMovementCost());
        map.move(map.posFloatToInt(pos),map.posFloatToInt(target));
	}
	pos+={Utils::Math::normalize(direction)*dt*speed};//todo if targethas negative coords it will get a wrong starting pos
	setPos(pos);
    if(Utils::Math::distance(pos,target)<5)
        setPos(target);
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
	: GameCharacter(30, health, 5, energy, 2)
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
	: GameCharacter(10, health, 8, energy, 3)
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
