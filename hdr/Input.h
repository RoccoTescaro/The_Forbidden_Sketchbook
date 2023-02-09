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
        ENUM_SIZE //must be mainteined as last element of the enum, indicates the number of elements in the list
    };


    void update();

    bool resizeEvent() const { return resized; }; //#TODO find better name

    sf::Vector2<float> getMousePos(const sf::View* view = nullptr) const;
    const float &getWheelDelta() const;
    
    bool isKeyPressed(Key key) const;
    bool isKeyDown(Key key) const;
    bool isKeyReleased(Key key) const;

private:

    sf::Event event{};
    bool resized = false;
    float wheelDelta = 0.f;
    std::bitset<Key::ENUM_SIZE> oldKeys{};
    std::bitset<Key::ENUM_SIZE> newKeys{};

};
