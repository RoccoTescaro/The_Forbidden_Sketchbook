#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>

class Input
{
public:

    enum Key
    {
        W,
        A,
        S,
        D,
        MouseR,
        MouseL,
        Enter,
        Space,
        Esc,
        Shift,
        ENUM_SIZE
    };


    void update(const sf::View* view = nullptr);

    const sf::Vector2<float> &getMousePos() const;
    const float &getWheelDelta() const;
    
    bool isKeyPressed(Key key) const;
    bool isKeyDown(Key key) const;
    bool isKeyReleased(Key key) const;

private:

    sf::Event event;
    sf::Vector2<float> mousePos;
    float WheelDelta;
    std::bitset<Key::ENUM_SIZE> oldKeys;   
    std::bitset<Key::ENUM_SIZE> newKeys;       

};
