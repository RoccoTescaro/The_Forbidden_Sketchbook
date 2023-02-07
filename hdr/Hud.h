#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Filter.h"
#include "Bar.h"
#include "DialogueManager.h"

class Hud 
{
public:
	Hud();

	inline void setView(const sf::View& view) { gui = view; };
	void setPlayer(const std::shared_ptr<Player>& player);

	void update(const float& dt);

	void render(sf::RenderWindow& window);
private:
	sf::View gui;
	Filter filter;
	std::weak_ptr<Player> player;
	Bar playerHealth;
	Bar playerEnergy;
	DialogueManager dialogueManager;
};