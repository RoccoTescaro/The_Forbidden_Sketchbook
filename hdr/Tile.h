#pragma once

#include "Entity.h"

class Tile : public Entity{};

class Wall : public Tile{

public:
    Wall(uint8_t type);

    virtual void setTexture(uint8_t newType);

    const bool isSolid() const override;

    void update(Map &map) override{};
    void execute(GameCharacter &gameCharacter, Map &map) override{};

private:
    uint8_t type;
};

class Hole : public Tile{

public:
    Hole();

    const bool isSolid() const override;

    void update(Map &map) override{};
    void execute(GameCharacter &gameCharacter, Map &map) override{};
 
};
class ColorPedestral : public Tile{

public:
    ColorPedestral();

    const bool isSolid() const override;
    inline const sf::Color& getColor() const { return color; };


    void update(Map &map) override{};
    void execute(GameCharacter &gameCharacter, Map &map) override{};

private:
	sf::Color color; 
};