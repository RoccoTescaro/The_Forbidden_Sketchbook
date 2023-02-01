#pragma once
#include <SFML/System.hpp>
#include "Serializable.h"
#include "Archive.h"
#include <map>
#include <memory>

//#TODO remove
class Entity;
class Tile;
class GameCharacter;
class Player;

class Map //: public Serializable
{
	struct Compare
	{
		bool operator() (const sf::Vector2<int>& pos1, const sf::Vector2<int>& pos2) const
		{
			return pos1.y < pos2.y;
		}
	};

	using Tiles = std::map<sf::Vector2<int>, std::shared_ptr<Tile>, Compare>;
	using GameCharacters = std::map<sf::Vector2<int>, std::shared_ptr<GameCharacter>, Compare>;

public:
	void render();

	inline Tiles& getTiles() { return tiles; };
	inline const Tiles& getTiles() const { return tiles; };
	inline GameCharacters& getGameCharacters() { return gameCharacters; };
	inline const GameCharacters& getGameCharacters() const { return gameCharacters; };

	std::shared_ptr<Tile> getTile(const sf::Vector2<int>& pos); //Cannot return a reference in case there is no entity at given pos
	std::shared_ptr<GameCharacter> getGameCharacter(const sf::Vector2<int>& pos);
	std::shared_ptr<Player> getPlayer();

	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

	Map& add(const std::shared_ptr<Entity>& entity);
	Map& addTile(const std::shared_ptr<Tile>& tile);
	Map& addGameCharacter(const std::shared_ptr<GameCharacter>& gameCharacter);

	Map& remove(const sf::Vector2<int>& pos);
	Map& removeTile(const sf::Vector2<int>& pos);
	Map& removeGameCharacter(const sf::Vector2<int>& pos);

	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end);

	bool isOccupied(const sf::Vector2<int>& pos, bool solid);

	inline sf::Vector2<float> posIntToFloat(const sf::Vector2<int>& pos);
	inline sf::Vector2<int> posFloatToInt(const sf::Vector2<float>& pos); //convert world coord to grided one

	//void serialize(Archive& fs) override; //#TODO fix sf::Vector2 save
private:
	//REGIST(Map);

	Tiles tiles;
	GameCharacters gameCharacters;
	std::shared_ptr<Player> player;

	sf::Vector2<int> cellDim{ 32,32 };
};
