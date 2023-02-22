#pragma once
#include <SFML/System.hpp>
#include "Serializable.h"
#include "Archive.h"
#include "Entity.h"
#include "Tile.h"
#include "GameCharacter.h"
#include <map>
#include <memory>
#include "GameCharacter.h"
#include "Tile.h"

class Map : public Serializable
{
	struct Compare
	{
		bool operator() (const sf::Vector2<int>& pos1, const sf::Vector2<int>& pos2) const
		{
			return (pos1.y < pos2.y||(pos1.y == pos2.y && pos1.x <pos2.x ));
		}
	};

	using Tiles = std::map<sf::Vector2<int>, std::shared_ptr<Tile>, Compare>;
	using GameCharacters = std::map<sf::Vector2<int>, std::shared_ptr<GameCharacter>, Compare>;

public:
	Map() : playerPos({0,0}), cellDim({64,32}) {};

	void render(sf::RenderWindow& window);

	inline Tiles& getTiles() { return tiles; };
	inline const Tiles& getTiles() const { return tiles; };
	inline GameCharacters& getGameCharacters() { return gameCharacters; };
	inline const GameCharacters& getGameCharacters() const { return gameCharacters; };

	std::shared_ptr<Tile> getTile(const sf::Vector2<int>& pos); //Cannot return a reference in case there is no entity at given pos
	std::shared_ptr<GameCharacter> getGameCharacter(const sf::Vector2<int>& pos);
	std::shared_ptr<GameCharacter> getPlayer();

	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

	Map& add(const sf::Vector2<int>& pos, Entity* entity);
	Map& addTile(const sf::Vector2<int>& pos, Tile* tile);
	Map& addGameCharacter(const sf::Vector2<int>& pos, GameCharacter* gameCharacter);

	Map& remove(const sf::Vector2<int>& pos);
	Map& removeTile(const sf::Vector2<int>& pos);
	Map& removeGameCharacter(const sf::Vector2<int>& pos);

	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end);

	bool isOccupied(const sf::Vector2<int>& pos, bool solid);

	sf::Vector2<float> posIntToFloat(const sf::Vector2<int>& pos);
	sf::Vector2<int> posFloatToInt(const sf::Vector2<float>& pos); //convert world coord to grided one

	void serialize(Archive& fs) override; 
private:
	static Serializable* create() { return new Map; };
	static Register registration;

	Tiles tiles;
	GameCharacters gameCharacters;
	sf::Vector2<int> playerPos;

	sf::Vector2<int> cellDim{ 64,32 };
};
