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

	inline virtual void render(sf::RenderWindow& window) { window.draw(sprite); };
	inline virtual void setPos(const sf::Vector2<float>& pos) { sprite.setPosition(pos); };
	
	inline sf::Vector2<float> getPos() const { return sprite.getPosition(); };
	inline sf::Vector2<float> getSize() const { return { sprite.getTextureRect().width * sprite.getScale().x, sprite.getTextureRect().height * sprite.getScale().y}; };
	inline sf::Vector2<float> getCenter() const 
	{
		return sf::Vector2<float> {	sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x + getSize().x * 0.5f,
									sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().y + getSize().y * 0.5f };
	};

	inline sf::Sprite& getSprite() { return sprite; };

	virtual bool isSolid() const = 0; 
	
	void serialize(Archive& fs) override 
	{ 
		sf::Vector2<float> pos = sprite.getPosition();
		fs.serialize(pos); 
		sprite.setPosition(pos);
	};

protected:
	sf::Sprite sprite;
};