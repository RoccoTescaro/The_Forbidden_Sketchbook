#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Application.h"

class Button
{
public:
    Button(const sf::Vector2<float>& pos, const sf::Vector2<int> dim) : pos(pos), dim(dim) {};

    //STATE
    inline void onMouseOver(const sf::Vector2<float>& mousePos, void* (*function)())
    {
        if(isMouseOver(mousePos))
            function();
    }

    inline void setActive(bool active)
    {
        this->active = active;
    };

    inline bool isActive() const
    {
        return active;
    };

    inline bool isMouseOver(const sf::Vector2<float>& mousePos) const
    {
        if (!active) return false; //if button is not active it maintains its properties

        sf::FloatRect bound{ pos,sf::Vector2<float>(dim) };
        return bound.contains(mousePos);
    };

    inline void setClicked(bool clicked)
    {
        this->clicked = clicked;
    };

    inline void setClicked()
    {
        clicked = !clicked; //toggle without parameters
    };

    inline bool isClicked() const
    {
        return clicked;
    };

    //TEXTURE/SPRITE
    inline void setTexture(const sf::Texture& texture)
    {
        sprite.setTexture(texture);
    };

    inline void setTextureRect(const sf::IntRect& rectangle)
    {
        sprite.setTextureRect(rectangle);
    };

    //TEXT
    inline void setText(const sf::Text& text)
    {
        this->text = text;
    };

    inline sf::Text& getText()
    {
        return text;
    };

    inline const sf::Text& getText() const
    {
        return text;
    };

    //RENDER
    inline void render()
    {
        Application::getWindow().draw(sprite);
        Application::getWindow().draw(text);
    };

private:
    //STATE
    bool active = true;
    bool clicked = false;

    //POSITION
    sf::Vector2<float> pos;
    sf::Vector2<int> dim;

    //RENDER
    sf::Sprite sprite;
    sf::Text text;
};