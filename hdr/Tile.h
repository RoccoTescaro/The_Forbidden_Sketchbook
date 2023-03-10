#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
    virtual ~Tile() = default;

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

    virtual ~Wall() {};

    void setTexture(uint8_t newType) ;

    inline bool isSolid() const override { return true; };

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
    
    virtual ~Hole() {};

    inline bool isSolid() const override { return false; };

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override {}; 

    static Serializable* create() { return new Hole; };
private:
    static Register registration;
};

class ColorPedestral : public Tile
{
public:
    ColorPedestral();

    virtual ~ColorPedestral() {};

    void render(sf::RenderWindow &window) override;

    inline bool isSolid() const override { return true; };

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override; 

    void serialize(Archive& fs) 
    { 
        Tile::serialize(fs);
        fs.serialize(color); 
    };

    static Serializable* create() { return new ColorPedestral; };
private:
    static Register registration;

	sf::Color color; 
    const sf::Color nullColor{0,0,0,0};

    sf::CircleShape colorSprite{8};
};


class WeaponBench : public Tile
{
public:
    WeaponBench();

    virtual ~WeaponBench() {};

    inline bool isSolid() const override { return true; };

    void interact(Map &map, sf::Vector2<float> target, const float &dt) override; 

    static Serializable* create() { return new WeaponBench; };
private:
    static Register registration;
};