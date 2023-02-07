#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Filter.h"
#include "Bar.h"
#include "DialogueManager.h"

class Hud 
{
public:
	Hud(Map& map);

	void update(const float& dt);

	void render(sf::RenderWindow& window);
private:
	Filter filter;
	std::weak_ptr<Player> player;
	Bar playerHealth;
	Bar playerEnergy;
	DialogueManager dialogueManager;
};