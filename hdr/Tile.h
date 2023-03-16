#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
    virtual ~Tile() = 0;

    void serialize(Archive& fs) override { Entity::serialize(fs); };
};

inline Tile::~Tile() {}
