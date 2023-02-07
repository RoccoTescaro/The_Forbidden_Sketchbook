#pragma once
#include "State.h"
#include "Map.h"
#include "Camera.h"
#include "Hud.h"

class Game : public State
{
public:
	Game();

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
	//turnSystem
	//actor
	Camera cam;
	Hud hud;
};

