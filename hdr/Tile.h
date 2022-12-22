#pragma once

#include "Entity.h"

class Tile : public Entity{
public:

    void update(Map &map, const float &dt) override{};
    void execute(GameCharacter &gameCharacter, Map &map) override{};

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

    void setTexture(uint8_t newType);

    const bool isSolid() const override;

private:
    uint8_t type;
};

class Hole : public Tile{

public:
    Hole();

    const bool isSolid() const override;
};
class ColorPedestral : public Tile{

public:
    ColorPedestral();

    const bool isSolid() const override;

    void execute(GameCharacter &gameCharacter, Map &map) override;//todo

private:
	sf::Color color; 
};