#pragma once
#include "Tile.h"

class Wall : public Tile
{
public:
    enum Type
    {
        RL,
        UD,
        RU,
        LU,
        RD,
        LD,
        ENUM_SIZE //must be mainteined as last element of the enum, indicates the number of elements in the list
    };

    Wall(uint8_t type) : type(type) { setTexture(type); }

    virtual ~Wall() {};

    void setTexture(uint8_t newType) 
    {
        static sf::Texture* texture[Type::ENUM_SIZE]{};
        for (int i = 0; i < Type::ENUM_SIZE; i++)
            if (!texture[i])
            {
                texture[i] = new sf::Texture;
                texture[i]->loadFromFile(Config::wallTexturePath[i]);
                texture[i]->generateMipmap();
            }
        type = newType;
        sprite.setTexture(*texture[type]);
        sf::Rect<int> textureRect{ 0,0,1300,1300 };
        sprite.setTextureRect(textureRect);
        sprite.setScale(64.f / textureRect.width, 96.f / textureRect.height);
        sprite.setOrigin(0.f, 867.f);
    };

    inline bool isSolid() const override { return true; };

    void interact(Map& map, sf::Vector2<float> target) override {};

    void serialize(Archive& fs) override
    {
        Tile::serialize(fs);
        fs.serialize(type);
        setTexture(type);
    }

    static Serializable* create() { return new Wall{ RL }; };

private:
    static Register registration;
    uint8_t type = 0;
};
