#pragma once
#include "GameCharacter.h"

class Bat : public GameCharacter
{
public:
    Bat(uint8_t health, uint8_t energy) 
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
		sprite.setScale(104.f / textureRect.width, 48.f / textureRect.height);
		sprite.setOrigin(340.f, 1000.f);


		movementStrategy.reset(new AStar);
		weapon.reset(new Weapon{ 2,1,1 });
	}

    virtual ~Bat() {};

    inline bool isSolid() const override { return false; };

    static Serializable* create() { return new Bat{ 10,10 }; };
private:
    static Register registration;
};