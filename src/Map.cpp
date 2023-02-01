#include "../hdr/Map.h"
#include "../hdr/Entity.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

void Map::render()
{
	for (auto& tile : tiles)
		tile.second->render();
	for (auto& gameCharacter : gameCharacters)
		gameCharacter.second->render();
};

std::shared_ptr<Tile> Map::getTile(const sf::Vector2<int>&pos)
{
	if (tiles.find(pos) != tiles.end())
		return tiles.at(pos);
	return nullptr;
}

std::shared_ptr<GameCharacter> Map::getGameCharacter(const sf::Vector2<int>&pos)
{
	if (gameCharacters.find(pos) != gameCharacters.end())
		return gameCharacters.at(pos);
	return nullptr;
}

std::shared_ptr<Player> Map::getPlayer()
{
	return player;
}

Map& Map::add(const std::shared_ptr<Entity>&entity)
{
	std::shared_ptr<Tile> ptrTile{ dynamic_cast<Tile*>(entity.get()) };
	std::shared_ptr<GameCharacter> ptrGameCharacter{ dynamic_cast<GameCharacter*>(entity.get()) };
	if (ptrTile) addTile(ptrTile);
	else if (ptrGameCharacter) addGameCharacter(ptrGameCharacter);
	else ERROR("no suitable conversion for entity");
	return *this;
}

Map& Map::addTile(const std::shared_ptr<Tile>&tile)
{
	sf::Vector2<int> pos = posFloatToInt(tile->getCenter());
	if (tiles.count(pos))
		removeTile(pos);
	tiles[pos] = static_cast<std::shared_ptr<Tile>>(tile);
	return *this;
}

Map& Map::addGameCharacter(const std::shared_ptr<GameCharacter>&gameCharacter)
{
	sf::Vector2<int> pos = posFloatToInt(gameCharacter->getCenter());
	if (gameCharacters.at(pos))
		removeGameCharacter(pos);

	gameCharacters[pos] = static_cast<std::shared_ptr<GameCharacter>>(gameCharacter);
	Player* ptrPlayer = dynamic_cast<Player*>(gameCharacter.get());
	if (ptrPlayer)
		player = static_cast<std::shared_ptr<Player>>(ptrPlayer);
	return *this;
}

Map& Map::remove(const sf::Vector2<int>&pos)
{
	removeTile(pos);
	removeGameCharacter(pos);
	return *this;
}

Map& Map::removeTile(const sf::Vector2<int>&pos)
{
	tiles.erase(pos);
	return *this;
}

Map& Map::removeGameCharacter(const sf::Vector2<int>&pos)
{
	gameCharacters.erase(pos);
	return *this;
}

void Map::move(const sf::Vector2<int>&start, const sf::Vector2<int>&end)
{
	auto gameCharacter = gameCharacters.extract(start);
	if (gameCharacter) gameCharacters[end] = gameCharacter.mapped();
	else ERROR("no gameCharacter at start position");
}

bool Map::isOccupied(const sf::Vector2<int>&pos, bool solid)
{
	return getTile(pos) && (getTile(pos).get()->isSolid() || solid);
}

inline sf::Vector2<float> Map::posIntToFloat(const sf::Vector2<int>&pos)
{
	return sf::Vector2<float>(pos.x * cellDim.x, pos.y * cellDim.y);
}

inline sf::Vector2<int> Map::posFloatToInt(const sf::Vector2<float>&pos)
{
	return sf::Vector2<int>(std::floor(pos.x / cellDim.x), std::floor(pos.y / cellDim.y));
}

/*void Map::serialize(Archive& fs)
{
	//can't easily serialize the tiles and gamecharacters maps becoude of compare function
	//but we can save each entity one by one
	switch (fs.getMode())
	{
	case Archive::Save:
	{
		uint32_t sizeTiles = tiles.size();
		fs.serialize(sizeTiles);
		for (auto& tile : tiles)
			fs.serialize(*tile.second.get());
		uint32_t sizeGameCharacters = gameCharacters.size();
		fs.serialize(sizeGameCharacters);
		for (auto& gameCharacter : gameCharacters)
			fs.serialize(gameCharacter.second);
	}
	case Archive::Load:
	{
		uint32_t sizeTiles;
		fs.serialize(sizeTiles);
		for (uint32_t i = 0; i < sizeTiles; i++)
		{
			std::shared_ptr<Tile> tile;
			fs.serialize(tile);
			addTile(tile);
		}
		uint32_t sizeGameCharacters;
		fs.serialize(sizeGameCharacters);
		for (uint32_t i = 0; i < sizeGameCharacters; i++)
		{
			std::shared_ptr<GameCharacter> gameCharacter;
			fs.serialize(gameCharacter);
			addGameCharacter(gameCharacter);
		}
	}
	}

}*/