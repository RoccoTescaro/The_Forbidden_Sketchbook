#include "../hdr/Map.h"
#include "../hdr/Entity.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

#define MAX_AREA_PROPERLY_RENDERED 1500 //50*30
#define RENDER_OVERSHOOT 3 

void Map::render(sf::RenderWindow& window)
{
	auto& view = window.getView();
	float top = view.getCenter().y - view.getSize().y * 0.5f - cellDim.y * RENDER_OVERSHOOT;
	float bottom = view.getCenter().y + view.getSize().y * 0.5 + cellDim.y * RENDER_OVERSHOOT;
	float left = view.getCenter().x - view.getSize().x * 0.5f - cellDim.x * RENDER_OVERSHOOT;
	float right = view.getCenter().x + view.getSize().x * 0.5f + cellDim.x * RENDER_OVERSHOOT;

	uint32_t nCells = ((bottom - top) / cellDim.y) * ((right - left) / cellDim.x);

	if (nCells < MAX_AREA_PROPERLY_RENDERED)
		for (float j = top; j < bottom; j += cellDim.y)
			for (float i = left; i < right; i += cellDim.x) 
			{
				auto tile = get<Tile>(posFloatToInt({ i,j }));
				auto gameCharater = get<GameCharacter>(posFloatToInt({ i,j }));
				if (tile) tile->render(window);
				else if (gameCharater) gameCharater->render(window);
			}
	else 
	{
		for (auto& tile : tiles)
			tile.second->render(window);

		for (auto& gameCharacter : gameCharacters)
			gameCharacter.second->render(window);
	}
}

Map& Map::append(const sf::Vector2<int>& pos, Entity* entity)
{
	auto tile = dynamic_cast<Tile*>(entity);
	auto gameCharacter = dynamic_cast<GameCharacter*>(entity);

	entity->setPos(posIntToFloat(pos));

	if (tile)
		tiles[pos] = std::shared_ptr<Tile>(tile);
	else if (gameCharacter)
	{
		auto player = dynamic_cast<Player*>(entity);
		if (player) playerPos = pos;
		gameCharacters[pos] = std::shared_ptr<GameCharacter>(gameCharacter);
	}
	else
	{
		ERROR("entity is not a tile or a gameCharacter");
		delete entity;
	}

	return *this;
}

Map& Map::remove(const sf::Vector2<int>& pos)
{
	if (tiles.count(pos)) tiles.erase(pos);
	if (gameCharacters.count(pos)) gameCharacters.erase(pos);
	return *this;
}

bool Map::isOccupied(const sf::Vector2<int>& pos, bool solid)
{
	return get<Tile>(pos).get() && (get<Tile>(pos)->isSolid() || solid);
}

void Map::move(const sf::Vector2<int>& start, const sf::Vector2<int>& end) 
{
	auto gameCharacter = gameCharacters.extract(start);
	if (gameCharacter) gameCharacters[end] = gameCharacter.mapped();
	else ERROR("no gameCharacter at start position");

	if (start == playerPos) playerPos = end;
}

sf::Vector2<float> Map::posIntToFloat(const sf::Vector2<int>&pos) const 
{
	return sf::Vector2<float>(pos.x * cellDim.x, pos.y * cellDim.y);
}

sf::Vector2<int> Map::posFloatToInt(const sf::Vector2<float>&pos) const
{
	return sf::Vector2<int>(std::floor(pos.x / cellDim.x), std::floor(pos.y / cellDim.y));
}

void Map::serialize(Archive& fs)
{
	//can't easily serialize the tiles and gamecharacters maps becouse of compare function
	//but we can save each entity one by one
	if (fs.getMode() == Archive::Save) 
	{
		uint32_t size = tiles.size();
		fs.serialize(size);
		for (auto& tile : tiles) 
				fs.serialize(tile.second);
		size = gameCharacters.size();
		fs.serialize(size);
		for (auto& gameCharacter : gameCharacters)
			fs.serialize(gameCharacter.second);
	}
	else 
	{
		tiles.clear();
		gameCharacters.clear();

		uint32_t size;
		fs.serialize(size);
		for (uint32_t i = 0; i < size; i++)
		{
			Tile* tile;
			fs.serialize(tile);
			append(posFloatToInt(tile->getPos()), tile);
		}
		fs.serialize(size);
		for (uint32_t i = 0; i < size; i++)
		{
			GameCharacter* gameCharacter;
			fs.serialize(gameCharacter);
			append(posFloatToInt(gameCharacter->getPos()), gameCharacter);
		}
	}
	
}