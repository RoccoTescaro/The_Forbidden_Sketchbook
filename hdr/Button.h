#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>
#include "Input.h"

class Button 
{
public:
    enum TextAlignment
    {
        LEFT,
        RIGHT,
        CENTER,
        TOP = LEFT,
        BOTTOM = RIGHT,
    };

    Button(const sf::Vector2<float>& pos, const sf::Vector2<int> dim) : pos(pos), dim(dim) {};
    Button(float x, float y, int width, int height) : pos(x, y), dim(width, height) {};

    //TEXT
    inline void setFont(const std::string& path) { font.loadFromFile(path); text.setFont(font); };
    void setTextAlignment(TextAlignment hAlignment, TextAlignment vAlignment);
    inline void setTextAlignment(TextAlignment alignment) { setTextAlignment(alignment, alignment); };
    void setText(const std::string& text, int charSize, TextAlignment hAlignment, TextAlignment vAlignment);
    void setText(const std::string& text, int charSize = 24, TextAlignment alignment = TextAlignment::CENTER);
    inline void setText(const sf::Text& text) { this->text = text; };
    inline const sf::Text& getText() const { return text; };
    inline sf::Text& getText() { return text; };
    
    //SPRITE
    void setSprite(sf::Texture& texture);
    inline void setSprite(const std::string& path) { texture.loadFromFile(path); setSprite(texture); };
    
    //STATE
    bool isMouseOver(const Input& input) const;
    inline void setActive(bool active) { this->active = active; };
    inline void setActive() { active = !active; }; //toggle without parameters
    inline bool isActive() const { return active; };
    inline void setClicked(bool clicked) { this->clicked = clicked; };
    inline void setClicked() { clicked = !clicked; }; //toggle without parameters
    inline bool isClicked() const { return clicked; };

    //UPDATE
    inline void setOnClick(std::function<void()> onClick) { this->onClick = onClick; };
    inline void setOnMouseOver(std::function<void()> onMouseOver) { this->onMouseOver = onMouseOver; };
    void update(const Input& input);
    void render(sf::RenderWindow& window);

private:
    //STATE
    bool active = true;
    bool clicked = false;
    std::function<void()> onClick;
    std::function<void()> onMouseOver;

    //POSITION
    sf::Vector2<float> pos;
    sf::Vector2<int> dim;

    //RENDER
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    sf::Text text;
};