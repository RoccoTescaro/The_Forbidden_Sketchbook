#pragma once

#include "Entity.h"

class Tile : public Entity{};

class Wall : public Tile{
public:
    Wall(uint8_t type);

    virtual void setTexture(uint8_t newType);

    const bool isSolid() const override;
};
class Hole : public Tile{};
class ColorPedestral : public Tile{};