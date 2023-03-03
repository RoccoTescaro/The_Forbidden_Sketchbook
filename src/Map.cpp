#include "../hdr/Map.h"
#include "../hdr/Entity.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

void Map::render(sf::RenderWindow& window)
{
	for (auto& tile : tiles)
		tile.second->render(); //TODO make indipendent from application 
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

std::shared_ptr<GameCharacter> Map::getPlayer()
{
	auto player = gameCharacters[playerPos];
	if (!player) ERROR("Map has no player in it");
	
	return player;
}

Map& Map::add(const sf::Vector2<int>& pos, Entity* entity)
{
	Tile* ptrTile = dynamic_cast<Tile*>(entity);
	GameCharacter* ptrGameCharacter = dynamic_cast<GameCharacter*>(entity);
	if (ptrTile) addTile(pos, ptrTile);
	else if (ptrGameCharacter) addGameCharacter(pos, ptrGameCharacter);
	else ERROR("no suitable conversion for entity");
	return *this;
}

Map& Map::addTile(const sf::Vector2<int>& pos, Tile* tile)
{
	tile->setPos(posIntToFloat(pos));
	if (tiles.count(pos))
		removeTile(pos);
	tiles[pos] = static_cast<std::shared_ptr<Tile>>(tile);		
	return *this;
}

Map& Map::addGameCharacter(const sf::Vector2<int>& pos, GameCharacter* gameCharacter)
{
	gameCharacter->setPos(posIntToFloat(pos));
	if (gameCharacters.count(pos))
		removeGameCharacter(pos);
	gameCharacters[pos] = static_cast<std::shared_ptr<GameCharacter>>(gameCharacter);
	
	Player* ptrPlayer = dynamic_cast<Player*>(gameCharacter);
	if (ptrPlayer) 
		playerPos = pos;
	return *this;
}

Map& Map::remove(const sf::Vector2<int>& pos)
{
	removeTile(pos);
	removeGameCharacter(pos);
	return *this;
}

Map& Map::removeTile(const sf::Vector2<int>& pos)
{
	tiles.erase(pos);
	return *this;
}

Map& Map::removeGameCharacter(const sf::Vector2<int>& pos)
{
	gameCharacters.erase(pos);
	return *this;
}

void Map::move(const sf::Vector2<int>& start, const sf::Vector2<int>& end)
{
	auto gameCharacter = gameCharacters.extract(start);
	if (gameCharacter) gameCharacters[end] = gameCharacter.mapped();
	else ERROR("no gameCharacter at start position");

	if (start == playerPos) playerPos = end;
}

bool Map::isOccupied(const sf::Vector2<int>& pos, bool solid)
{
	return getTile(pos) && (getTile(pos).get()->isSolid() || solid); 
}

sf::Vector2<float> Map::posIntToFloat(const sf::Vector2<int>&pos)
{
	return sf::Vector2<float>(pos.x * cellDim.x, pos.y * cellDim.y);
}

sf::Vector2<int> Map::posFloatToInt(const sf::Vector2<float>&pos)
{
	return sf::Vector2<int>(std::floor(pos.x / cellDim.x), std::floor(pos.y / cellDim.y));
}

void Map::serialize(Archive& fs)
{
	//can't easily serialize the tiles and gamecharacters maps becoude of compare function
	//but we can save each entity one by one
	if (fs.getMode() == Archive::Save) 
	{
		uint32_t sizeTiles = tiles.size();
		fs.serialize(sizeTiles);
		for (auto& tile : tiles)
			fs.serialize(tile.second);
		uint32_t sizeGameCharacters = gameCharacters.size();
		fs.serialize(sizeGameCharacters);
		for (auto& gameCharacter : gameCharacters)
			fs.serialize(gameCharacter.second);
	}
	else 
	{
		tiles.clear();
		gameCharacters.clear();

		uint32_t sizeTiles;
		fs.serialize(sizeTiles);
		for (uint32_t i = 0; i < sizeTiles; i++)
		{
			Tile* tile;
			fs.serialize(tile);
			addTile(posFloatToInt(tile->getPos()), tile);
		}
		uint32_t sizeGameCharacters;
		fs.serialize(sizeGameCharacters);
		for (uint32_t i = 0; i < sizeGameCharacters; i++)
		{
			GameCharacter* gameCharacter;
			fs.serialize(gameCharacter);
			addGameCharacter(posFloatToInt(gameCharacter->getPos()), gameCharacter);
		}
	}
	
}