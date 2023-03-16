#pragma once
#include "Tile.h"
#include "Trigger.h"

class ColorPedestral : public Tile
{
public:
    ColorPedestral();

    virtual ~ColorPedestral() {};

    void render(sf::RenderWindow& window) override;

    inline bool isSolid() const override { return true; };

    void interact(Map& map, sf::Vector2<float> target) override;

    void serialize(Archive& fs)
    {
        Tile::serialize(fs);
        fs.serialize(color);
    };

    static Serializable* create() { return new ColorPedestral; };
private:
    static Register registration;

    sf::Color color;
    const sf::Color nullColor{ 0,0,0,0 };

    sf::CircleShape colorSprite{ 8 };
    static Trigger trigger;
};
