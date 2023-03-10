#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
    virtual ~Tile() = default;

    void serialize(Archive& fs) override { Entity::serialize(fs); };
};

