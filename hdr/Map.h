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

	using Tiles = std::unordered_map<const char*, std::map<sf::Vector2<int>, std::shared_ptr<Tile>, Compare>>;
	using GameCharacters = std::unordered_map<const char*, std::map<sf::Vector2<int>, std::shared_ptr<GameCharacter>, Compare>>;

public:
	Map() : cellDim({ 64,32 }) {};// if (!get<Player>().get()) append({ 0,0 }, new Player); };

	void render(sf::RenderWindow& window);

	template<class Type>
	Map& append(const sf::Vector2<int>& pos, Type* entity);

	template<class Type>
	Map& remove(const sf::Vector2<int>& pos);

	template<class Type>
	std::shared_ptr<Type> get(const sf::Vector2<int>& pos) const;
	template<class Player>
	std::shared_ptr<Player> get() const;

	inline const Tiles& getTiles() const { return tiles; };
	inline const GameCharacters& getGameCharacters() const { return gameCharacters; };
	
	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end);

	bool isOccupied(const sf::Vector2<int>& pos, bool solid = false) const;
	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

	/*inline Tiles& getTiles() { return tiles; };
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

	bool isOccupied(const sf::Vector2<int>& pos, bool solid);*/

	sf::Vector2<float> posIntToFloat(const sf::Vector2<int>& pos) const;
	sf::Vector2<int> posFloatToInt(const sf::Vector2<float>& pos) const; //convert world coord to grided one

	void serialize(Archive& fs) override; 
private:
	static Serializable* create() { return new Map; };
	static Register registration;

	Tiles tiles;
	GameCharacters gameCharacters;
	
	sf::Vector2<int> cellDim{ 64,32 };
};

template<class Type>
Map& Map::append(const sf::Vector2<int>& pos, Type* entity) 
{
	const char* id = typeid(Type).name();  		 
	Tile* tile = dynamic_cast<Tile*>(entity);
	GameCharacter* gameCharacter = dynamic_cast<GameCharacter*>(entity);

	bool occupied = isOccupied(pos, entity->isSolid()); //TODO fix

	if (!occupied) 
	{
		if (tile) 
		{
			tiles[id][pos] = static_cast<std::shared_ptr<Tile>>(tile);
			tile->setPos(posIntToFloat(pos));
			LOG("allocated instance of {3} at pos {{1},{2}}", pos.x, pos.y, id);
		}
		else if (gameCharacter) 
		{
			gameCharacters[id][pos] = static_cast<std::shared_ptr<GameCharacter>>(gameCharacter);
			gameCharacter->setPos(posIntToFloat(pos));
			LOG("allocated instance of {3} at pos {{1},{2}}", pos.x, pos.y, id);
		}
		else
		{
			WARNING(!tile&&!gameCharacter,"attempt to allocate a non valid entity");
			delete entity;
		}
	}
	else
		delete entity;

	return *this;
}

template<class Type>
Map& Map::remove(const sf::Vector2<int>& pos)
{
	const char* id = typeid(Type).name();

	if (tiles.find(id) != tiles.end())
		tiles[id].erase(pos);
	else if (gameCharacters.find(id) != gameCharacters.end())
		gameCharacters[id].erase(pos);
	else
		ERROR("attempt to remove a non valid entity");

	return *this;
}


template<>
Map& Map::remove<Tile>(const sf::Vector2<int>& pos);

template<>
Map& Map::remove<GameCharacter>(const sf::Vector2<int>& pos);

template<>
Map& Map::remove<Entity>(const sf::Vector2<int>& pos);

template<class Type>
std::shared_ptr<Type> Map::get(const sf::Vector2<int>& pos) const
{
	const char* id = typeid(Type).name();

	if (tiles.find(id) != tiles.end() && tiles.at(id).find(pos) != tiles.at(id).end())
		return std::static_pointer_cast<Type>(tiles.at(id).at(pos).get());
	else if (gameCharacters.find(id) != gameCharacters.end() && gameCharacters.at(id).find(pos) != gameCharacters.at(id).end())
		return std::static_pointer_cast<Type>(gameCharacters.at(id).at(pos).get());
	else
		return nullptr;
}

template<>
std::shared_ptr<Entity> Map::get<Entity>(const sf::Vector2<int>& pos) const;

template<>
std::shared_ptr<Tile> Map::get<Tile>(const sf::Vector2<int>& pos) const;

template<>
std::shared_ptr<GameCharacter> Map::get<GameCharacter>(const sf::Vector2<int>& pos) const;

template<class Player>
std::shared_ptr<Player> Map::get() const
{
	auto& players = gameCharacters.at(typeid(Player).name());
	for (auto& player : players)
		return std::dynamic_pointer_cast<Player>(player.second);
}
