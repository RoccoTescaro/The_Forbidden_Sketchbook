#pragma once
#include "State.h"
#include "Button.h"

class Menu : public State
{
public:
    Menu();

    void update() override;
    void render() override;
private:
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    sf::Font titleFont;
    sf::Text title;

    Button start;
    Button load;
    Button editor;
    Button exit;
};