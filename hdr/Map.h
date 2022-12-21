#pragma once
#include <SFML/System.hpp>
#include <map>
#include <memory>

//#TODO remove
class Tile; 
class GameCharacter;
class Player;

class Map
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
 
	std::shared_ptr<Tile> getTile(const sf::Vector2<int>& pos); //{ return *tiles.at(pos); };
	std::shared_ptr<GameCharacter> getGameCharacter(const sf::Vector2<int>& pos) ;
	std::shared_ptr<Player> getPlayer();

	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

	Map& addTile(Tile* tile);
	Map& addGameCharacter(GameCharacter* gameCharacter);

	Map& removeTile(const sf::Vector2<int>& pos);
	Map& removeGameCharacter(const sf::Vector2<int>& pos);

	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end);

	//inline bool isOccupied(const sf::Vector2<int>& pos);

	inline sf::Vector2<float> posIntToFloat();
	inline sf::Vector2<int> posFloatToInt(); //convert world coord to grided one

private:

	Tiles tiles;
	GameCharacters gameCharacters;
	std::shared_ptr<Player> player; 

	sf::Vector2<int> cellDim{ 32,32 };
};