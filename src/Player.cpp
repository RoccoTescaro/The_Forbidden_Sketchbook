#include "../hdr/Player.h"


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
	sprite.setScale(64.f / textureRect.width, 128.f / textureRect.height);
	sprite.setOrigin(0.f, 1470.f);
	//FILTER
	filterColor = sf::Color(filterColorR, filterColorG, filterColorB, 255);

	movementStrategy.reset(new AStar);
	weapon.reset(new Weapon{ 2,1,1 });
}

void Player::serialize(Archive& fs)
{
	GameCharacter::serialize(fs);
	fs.serialize(filterColor);
	uint8_t attack = weapon->getAttack();
	uint8_t cost = weapon->getCost();
	uint8_t range = weapon->getRange();
	bool hidden = weapon->isHidden();
	fs.serialize(attack);
	fs.serialize(cost);
	fs.serialize(range);
	fs.serialize(hidden);
	weapon.reset(new Weapon{ attack, cost, range, hidden });
}

