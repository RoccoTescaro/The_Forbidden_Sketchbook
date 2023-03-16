#pragma once
#include "GameCharacter.h"

class Melee : public GameCharacter
{
public:
    Melee(uint8_t health, uint8_t energy)
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
		weapon.reset(new Weapon{ 3, 2, 1 });
	}

    inline virtual ~Melee() override {};

    static Serializable* create() { return new Melee{ 30,5 }; };
private:
    static Register registration;
};
