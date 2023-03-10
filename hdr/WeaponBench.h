#pragma once
#include "Tile.h"

class WeaponBench : public Tile
{
public:
    WeaponBench();
    virtual ~WeaponBench() {};

    inline bool isSolid() const override { return true; };

    void interact(Map& map, sf::Vector2<float> target, const float& dt) override;

    static Serializable* create() { return new WeaponBench; };
private:
    static Register registration;
};