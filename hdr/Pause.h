#pragma once
#include "State.h"
#include "Button.h"

class Pause :	public State
{
public:
    Pause();

    void update() override;
    void render() override;
private:
    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::Font buttonFont;
    Button save;
    Button backToGame;
    Button backToMenu;
};

