#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
    void serialize(Archive& fs) override { Entity::serialize(fs); };
};

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

    Wall(uint8_t type);

    void setTexture(uint8_t newType) ;

    bool isSolid() const override;

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override {}; 

   
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

class Hole : public Tile
{
public:
    Hole();

    bool isSolid() const override;

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override {}; 

    static Serializable* create() { return new Hole; };
private:
    static Register registration;
};

class ColorPedestral : public Tile
{
public:
    ColorPedestral();

    bool isSolid() const override;

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override {}; 

    void serialize(Archive& fs) 
    { 
        Tile::serialize(fs);
        fs.serialize(color); 
    };

    static Serializable* create() { return new ColorPedestral; };
private:
    static Register registration;

	sf::Color color; 
};