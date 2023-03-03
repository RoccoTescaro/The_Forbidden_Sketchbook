#pragma once
#include "State.h"
#include "Button.h"
#include "Map.h"
#include "Camera.h"
#include <vector>

class Editor : public State
{
public:
    Editor();

    void update() override;
    void render() override;

	void save();
	void load();
private:
	sf::Texture backgroundTexture;
	sf::RectangleShape backgroundSprite;
	sf::Shader backgroundShader;

	sf::Vector2<int> mousePos; //in map coordinates;
	sf::Font mouseFont;
	sf::Text mousePosText;
	sf::RectangleShape mouseIndicator;

	Map map;
	Camera cam;
	sf::View gui;

	std::vector<std::function<Serializable* ()>> entitiesFactories;
	std::vector<std::function<Serializable* ()>>::iterator factory;
	std::unique_ptr<Entity> placeHolderEntity;
	sf::Texture placeHolderBackgroundTexture;
	sf::Sprite placeHolderBackground;
};