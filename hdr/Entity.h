#pragma once 
#include <SFML/Graphics.hpp>
#include "Serializable.h"
#include "Application.h"
#include "Archive.h"

class Map;
class GameCharacter;

class Entity : public Serializable
{
public:
	virtual ~Entity() = default;

	inline virtual void render() { Application::getWindow().draw(sprite); };

	virtual void update(Map &map, const float &dt) = 0;
	virtual void execute(GameCharacter &gameCharacter, Map &map) = 0;

	inline void setPos(const sf::Vector2<float>& pos) { sprite.setPosition(pos); };
	
	inline sf::Vector2<float> getPos() const { return sprite.getPosition(); };
	inline sf::Vector2<float> getCenter() const { return (getPos() + getSize() * 0.5f); };
	virtual bool isSolid() const = 0;

	void serialize(Archive& fs) override { }//fs.serialize(sprite.getPosition()); };
protected:

	inline sf::Vector2<float> getSize() const 
	{ 
		return { 20,20 };
		//return { sprite.getTexture()->getSize().x * sprite.getScale().x, sprite.getTexture()->getSize().y * sprite.getScale().y }; 
	};

	sf::Sprite sprite;
};