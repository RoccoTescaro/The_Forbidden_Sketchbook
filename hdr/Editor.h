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

	std::vector<std::function<Serializable* ()>> entitiesFactories;
	std::vector<std::function<Serializable* ()>>::iterator factory;
	std::unique_ptr<Entity> placeHolderEntity;
	std::unique_ptr<Entity> previewEntity;
	sf::Texture placeHolderBackgroundTexture;
	sf::Sprite placeHolderBackground;

	//the only reason to have a shr pointer to map instead of an object on the stack is becouse turnSystem need a shr pointer 
	//otherwise when turnSystem get destroyed before map, which is likely and certain for editor the program gonna crash
	//dealocating an object on the stack when still used.
	std::shared_ptr<Map> map;
	Camera cam;
	sf::View gui;

};