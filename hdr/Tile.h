#pragma once

#include "Entity.h"

class Tile : public Entity
{
public:

    void update(Map &map, const float &dt) override{};
    void execute(GameCharacter &gameCharacter, Map &map) override{};
    void serialize(Archive& fs) override { Entity::serialize(fs); };

};

class Wall : public Tile{

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
    Wall() {};

    void setTexture(uint8_t newType);

    bool isSolid() const override;

    void serialize(Archive& fs) override 
    {
        Tile::serialize(fs);
        fs.serialize(type);
    }
private:
    uint8_t type;
    REGIST(Wall);
};

class Hole : public Tile{

public:
    Hole();

    bool isSolid() const override;
private:
    REGIST(Hole);
};

class ColorPedestral : public Tile{

public:
    ColorPedestral();

    bool isSolid() const override;

    void execute(GameCharacter &gameCharacter, Map &map) override;//todo

private:
	sf::Color color; 
};