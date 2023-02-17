#pragma once
#include "State.h"
#include "Map.h"
#include "TurnSystem.h"
#include "GameCharacter.h"
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

	std::shared_ptr<Map> map;
	TurnSystem turnSystem;
	std::shared_ptr<GameCharacter> actor;
	Camera cam;
	Hud hud;
};

