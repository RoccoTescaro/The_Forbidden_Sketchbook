#pragma once 

#include <SFML/Graphics.hpp>

class Map;
class GameCharacter;

class Entity
{
public:
	virtual ~Entity() = default;


	virtual void render(); // gets window from application (singleton)
	virtual void update(Map &map) = 0;
	virtual void execute(GameCharacter &gameCharacter, Map &map) = 0;

    void setPos(const sf::Vector2<float> &pos){sprite.setPosition(pos);}

    const sf::Vector2<float> getPos() const {return sprite.getPosition();}
    const sf::Vector2<float> getCenter() const {return (getPos()+getSize()*0.5f);}


protected:

	sf::Sprite sprite;

	const sf::Vector2<float> getSize() const {return {sprite.getTexture()->getSize().x * sprite.getScale().x, sprite.getTexture()->getSize().y * sprite.getScale().y};};




};