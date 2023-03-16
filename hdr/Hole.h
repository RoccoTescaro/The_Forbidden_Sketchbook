#pragma once
#include "Tile.h"

class Hole : public Tile
{
public:
    Hole() 
	{
		static sf::Texture* texture;
		if (!texture) {
			texture = new sf::Texture;
			texture->loadFromFile(Config::holeTexturePath);
			texture->generateMipmap();
		}
		sprite.setTexture(*texture);
		sf::Rect<int> textureRect{ 0,0,1080,1080 };
		sprite.setTextureRect(textureRect);
		sprite.setScale(64.f / textureRect.width, 64.f / textureRect.height);
		sprite.setOrigin(0.f, 405.f);
	}

    virtual ~Hole() {};

    inline bool isSolid() const override { return false; };

    void interact(Map& map, sf::Vector2<float> target) override {};

    static Serializable* create() { return new Hole; };
private:
    static Register registration;
};