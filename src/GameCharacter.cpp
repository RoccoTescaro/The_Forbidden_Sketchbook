#include "../hdr/GameCharacter.h"
#include "../hdr/Map.h"

//GAMECHARACTER

void GameCharacter::interact(Map &map, sf::Vector2<float> target)
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

//PLAYER

Player::Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB)
	: GameCharacter(50, health, 15, energy, 0)
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
	sprite.setScale(64.f/textureRect.width,128.f/textureRect.height); 
	sprite.setOrigin(0.f,1470.f);
	//FILTER
	filterColor = sf::Color(filterColorR, filterColorG, filterColorB, 255);

    movementStrategy.reset(new AStar);
	weapon.reset(new Weapon{2,1,1});
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
	sprite.setScale(64.f / textureRect.width, 128.f / textureRect.height);
	sprite.setOrigin(0.f, 1470.f);


	movementStrategy.reset(new AStar);
	weapon.reset(new Weapon{3, 2, 1});
}

//BAT

Bat::Bat(uint8_t health, uint8_t energy)
	: GameCharacter(10, health, 10, energy, 3)
{
	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile(Config::batTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 110,0,1800,1070 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(104.f/textureRect.width,48.f/textureRect.height);
	sprite.setOrigin(340.f, 1000.f);


    movementStrategy.reset(new AStar);
	weapon.reset(new Weapon{2,1,1});
}

//RANGED

Ranged::Ranged(uint8_t health, uint8_t energy)
	: GameCharacter(5, health, 15, energy, 1), animationDuration(1), animationTime(0)
{

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile(Config::rangedTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 150,96,780,928 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f / textureRect.width, 96.f / textureRect.height);
	sprite.setOrigin(0.f, 619.f);


	movementStrategy.reset(new DigletMovement);
	weapon.reset(new Weapon{2, 3, 5});
}
