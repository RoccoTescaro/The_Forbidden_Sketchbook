#pragma once
#include <SFML/System.hpp>
#include "Serializable.h"
#include "Archive.h"
#include "Entity.h"
#include "Tile.h"
#include "GameCharacter.h"
#include <unordered_map>
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
	Map() : cellDim({ 64,32 }) {};

	void render(sf::RenderWindow& window);

	template<class Type>
	std::shared_ptr<Type> get(const sf::Vector2<int>& pos) 
	{
		const char* id = typeid(Type).name();

		if constexpr (std::is_base_of<Tile, Type>::value)
			if(tiles.find(id) != tiles.end() && tiles.at(id).find(pos) != tiles.at(id).end())
				return std::static_pointer_cast<Type>(tiles.at(id).at(pos));
		else if constexpr (std::is_base_of<GameCharacter, Type>::value)
			if (gameCharacters.find(id) != gameCharacters.end() && gameCharacters.at(id).find(pos) != gameCharacters.at(id).end())
				return std::static_pointer_cast<Type>(gameCharacters.at(id).at(pos));

		return std::shared_ptr<Type>(nullptr);;
	}

	template<>
	std::shared_ptr<Entity> get<Entity>(const sf::Vector2<int>& pos) 
	{
		std::shared_ptr<Tile> tile = get<Tile>(pos);
		std::shared_ptr<GameCharacter> gameCharacter = get<GameCharacter>(pos);

		if (tile.get()) return tile;
		else if (gameCharacter.get()) return gameCharacter;
		return nullptr;
	}

	template<>
	std::shared_ptr<Tile> get<Tile>(const sf::Vector2<int>& pos) 
	{
		for (auto& tile : tiles)
			if (tile.second.find(pos) != tile.second.end())
				return tile.second.at(pos);
		return nullptr;
	}

	template<>
	std::shared_ptr<GameCharacter> get<GameCharacter>(const sf::Vector2<int>& pos) 
	{
		for (auto& gameCharacter : gameCharacters)
			if (gameCharacter.second.find(pos) != gameCharacter.second.end())
				return gameCharacter.second.at(pos);
		return nullptr;
	}

	template<class Player>
	std::shared_ptr<Player> get() 
	{
		auto& players = gameCharacters.at(typeid(Player).name());
		for (auto& player : players)
			return std::dynamic_pointer_cast<Player>(player.second);
	}

	template<class Type>
	Map& append(const sf::Vector2<int>& pos, Type* entity) 
	{
		const char* id = typeid(*entity).name();
		Tile* tile = dynamic_cast<Tile*>(entity);
		GameCharacter* gameCharacter = dynamic_cast<GameCharacter*>(entity);

		bool occupied = isOccupied(pos, entity->isSolid()) || (gameCharacter && get<GameCharacter>(pos).get()) || (tile && get<Tile>(pos).get());

		if (!occupied)
		{
			if (tile)
			{
				tiles[id][pos] = static_cast<std::shared_ptr<Tile>>(tile);
				tile->setPos(posIntToFloat(pos));
				//LOG("allocated instance of {3} at pos {{1},{2}}", pos.x, pos.y, id);
			}
			else if (gameCharacter)
			{
				gameCharacters[id][pos] = static_cast<std::shared_ptr<GameCharacter>>(gameCharacter);
				gameCharacter->setPos(posIntToFloat(pos));
				//LOG("allocated instance of {3} at pos {{1},{2}}", pos.x, pos.y, id);
			}
			else
			{
				WARNING(!tile && !gameCharacter, "attempt to allocate a non valid entity");
				delete entity;
			}
		}
		else
			delete entity;

		return *this;
	}

	template<class Type>
	Map& remove(const sf::Vector2<int>& pos) 
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
	Map& remove<Entity>(const sf::Vector2<int>& pos) 
	{
		remove<Tile>(pos);
		remove<GameCharacter>(pos);
		return *this;
	}

	template<>
	Map& remove<Tile>(const sf::Vector2<int>& pos) 
	{
		for (auto& tile : tiles) 
		{
			if (tile.second.erase(pos))
			{
				//LOG("a Tile removed at pos {{1},{2}}", pos.x, pos.y);
				return *this;
			}
			else;
				//LOG("no Tile at pos {{1},{2}}", pos.x, pos.y);
		}
		return *this;
	}

	template<>
	Map& remove<GameCharacter>(const sf::Vector2<int>& pos)
	{
		for (auto& gameCharacter : gameCharacters) 
		{
			if (gameCharacter.second.erase(pos)) {
				//LOG("a GameCharacter removed at pos {{1},{2}}", pos.x, pos.y);
				return *this;
			}
			else;
				//LOG("no GameCharacter at pos {{1},{2}}", pos.x, pos.y);
		}
		return *this;
	}


	inline const Tiles& getTiles() const { return tiles; };
	inline const GameCharacters& getGameCharacters() const { return gameCharacters; };
	
	void move(const sf::Vector2<int>& start, const sf::Vector2<int>& end);

	bool isOccupied(const sf::Vector2<int>& pos, bool solid = false);

	inline const sf::Vector2<int>& getCellDim() const { return cellDim; };

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

