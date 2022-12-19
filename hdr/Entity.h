#pragma once 
#include <SFML/Graphics.hpp>
#include "Serializable.h"


class Map;
class GameCharacter;

class Entity 
{
public:
	virtual ~Entity() = default;

	inline virtual void render()
	{
   		//sf::RenderWindow &window = Application::getWindow();
   		//window.draw(sprite;)
	};

	virtual void update(Map &map) = 0;
	virtual void execute(GameCharacter &gameCharacter, Map &map) = 0;

	inline void setPos(const sf::Vector2<float>& pos) { sprite.setPosition(pos); };
	inline sf::Vector2<float> getPos() const { return sprite.getPosition(); };
	inline sf::Vector2<float> getCenter() const { return (getPos() + getSize() * 0.5f); };
	inline virtual const bool isSolid() const =0;
S
protected:

	inline sf::Vector2<float> getSize() const 
	{ 
		return { sprite.getTexture()->getSize().x * sprite.getScale().x, sprite.getTexture()->getSize().y * sprite.getScale().y }; 
	};

	sf::Sprite sprite;
};