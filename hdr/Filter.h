#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"

class Filter 
{
public:
    Filter() 
    {
        shader.loadFromFile(Config::filterShaderPath, sf::Shader::Fragment);
        state.shader = &shader;
        state.blendMode = sf::BlendMultiply;
        shader.setUniform("previousColor", sf::Glsl::Vec4(previousColor));
        shader.setUniform("targetColor", sf::Glsl::Vec4(targetColor));
    };

    inline void update(const float& dt) 
    {
        threshold += dt * 0.1f;
        shader.setUniform("threshold", threshold);
    };

    void render(sf::RenderWindow& window) 
    {
        texture.create(window.getSize().x, window.getSize().y); 
        sprite.setTexture(texture.getTexture());
        window.draw(sprite, state);
    };

    void setTargetColor(const sf::Color& color)
    {
        if (threshold <= 1.f || targetColor == color) //last animation not ended yet
            return;

        previousColor = targetColor;
        targetColor = color;
        shader.setUniform("previousColor", sf::Glsl::Vec4(previousColor));
        shader.setUniform("targetColor", sf::Glsl::Vec4(targetColor));
        threshold = 0.f;
    };

private:
    sf::RenderTexture texture;
    sf::RenderStates state;
    sf::Sprite  sprite;
    sf::Shader  shader;
    float threshold = 0.f;
    sf::Color previousColor{0.f,0.f,0.f,255.f};
    sf::Color targetColor;
};