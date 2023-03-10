#pragma once
#include <SFML/System.hpp>
#include "Serializable.h"
#include "Archive.h"
#include "Entity.h"
#include "Tile.h"
#include "GameCharacter.h"
#include <map>
#include <memory>

class Map : public Serializable
{
	struct Compare
	{
		bool operator() (const sf::Vector2<int>& pos1, const sf::Vector2<int>& pos2) const
		{
			return (pos1.y < pos2.y||(pos1.y == pos2.y && pos1.x < pos2.x ));
		}
	};

	using Tiles = std::map<sf::Vector2<int>, std::shared_ptr<Tile>, Compare>;
	using GameCharacters = std::map<sf::Vector2<int>, std::shared_ptr<GameCharacter>, Compare>;

public:
	void render(sf::RenderWindow& window);
	
	template<class Type>
	std::shared_ptr<Type> get(const sf::Vector2<int>& pos) //doesn't work for get<Entity>
	{
		if constexpr (std::is_base_of<Tile, Type>::value || std::is_same<Tile, Type>::value)
			if(tiles.count(pos)) return std::static_pointer_cast<Type>(tiles.at(pos));
		
		if constexpr (std::is_base_of<GameCharacter, Type>::value || std::is_same<GameCharacter, Type>::value)
			if(gameCharacters.count(pos)) return std::static_pointer_cast<Type>(gameCharacters.at(pos));

		return std::shared_ptr<Type>(nullptr);
	}

	template<class Player>
	std::shared_ptr<Player> get()
	{
		if(gameCharacters.count(playerPos) && dynamic_cast<Player*>(gameCharacters[playerPos].get()))
			return std::static_pointer_cast<Player>(gameCharacters.at(playerPos));
		
		LOG("no player!!!");
		gameCharacters[playerPos] = std::shared_ptr<GameCharacter>(new Player{50,15,190,190,190});
		return std::static_pointer_cast<Player>(gameCharacters.at(playerPos));
	}
	
	Map& append(const sf::Vector2<int>& pos, Entity* entity);

	Map& remove(const sf::Vector2<int>& pos);

	inline const Tiles& getTiles() const { return tiles; };
	inline const GameCharacters& getGameCharacters() const { return gameCharacters; };
	
	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end); //can move only gameCharacters

	bool isOccupied(const sf::Vector2<int>& pos, bool solid = false); //check if tile is solid or occupied by a gameCharacter (for pathfinding)

	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

	sf::Vector2<float> posIntToFloat(const sf::Vector2<int>& pos) const;
	sf::Vector2<int> posFloatToInt(const sf::Vector2<float>& pos) const; //convert world coord to grided one

	void serialize(Archive& fs) override; 
private:
	static Serializable* create() { return new Map; };
	static Register registration;

	Tiles tiles;
	GameCharacters gameCharacters;

	sf::Vector2<int> playerPos{ 0,0 };
	
	sf::Vector2<int> cellDim{ 64,32 };
};
