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

	//the only reason to have a shr pointer to map instead of an object on the stack is becouse turnSystem need a shr pointer 
	//otherwise when turnSystem get destroyed before map, which is likely and certain for editor the program gonna crash
	//dealocating an object on the stack when still used.
	std::shared_ptr<Map> map; 
	TurnSystem turnSystem;
	std::weak_ptr<GameCharacter> actor;
	Camera cam;
	Hud hud;
};

