#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

void GameCharacter::update(Map &map, const float &dt)
{
	std::cout<<getPos().x<<"-"<<getPos().y<<std::endl;
	setEnergy(0);
}

void GameCharacter::execute(GameCharacter &gameCharacter, Map &map)
{

}

bool GameCharacter::isInRange(Map &map) const
{
	return true;
}

void GameCharacter::updateStepQueue(Map &map,const sf::Vector2<float> target)
{

	//if(stepQueue.empty())
		//stepQueue=movementStrategy->findPath(this->getPos(),target,isSolid())
		//;
}

//PLAYER

Player::Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB)
	: GameCharacter(100,health,100,energy,100) 
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
    //movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
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
   //movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
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
    //movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
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
    //movementStrategy = std::unique_ptr<PathAlgorithm>(new DiglettMovement());
}

void Ranged::update(Map &map, const float &dt)
{

}
