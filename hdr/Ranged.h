#pragma once
#include "GameCharacter.h"

class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy)
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
		weapon.reset(new Weapon{ 2, 3, 5 });
	}

    virtual ~Ranged() {};

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }

    static Serializable* create() { return new Ranged{ 5,15 }; };
private:
    static Register registration;

    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
