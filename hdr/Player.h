#pragma once
#include "GameCharacter.h"

class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    virtual ~Player() {};

    inline const sf::Color& getFilterColor() const { return filterColor; };
    inline void setFilterColor(const sf::Color& color) { filterColor = color; };

    void serialize(Archive& fs) override;

    static Serializable* create() { return new Player{ 50,15,190,190,190 }; };
private:
    static Register registration;
    sf::Color filterColor;
};