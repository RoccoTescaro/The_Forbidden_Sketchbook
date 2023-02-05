#pragma once
#include "State.h"
#include "Button.h"
#include "Map.h"
#include "Camera.h"

class Editor : public State
{
public:
    Editor();

    void update() override;
    void render() override;
private:
    sf::Texture backgroundTexture;
    sf::Sprite background;

    Map map;
    Camera cam;
    sf::View gui;

    sf::Vector2<int> mouseGriddedPos;
    sf::RectangleShape mouseIndicator;
    sf::Font mouseFont;
    sf::Text mousePosText;

};