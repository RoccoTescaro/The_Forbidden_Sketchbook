#include "../hdr/Map.h"
#include "../hdr/Entity.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

template<>
std::shared_ptr<Tile> Map::get<Tile>(const sf::Vector2<int>& pos) 
{
	for (auto& tile : tiles)
		if (tile.second.find(pos) != tile.second.end())
			return tile.second.at(pos);
	return nullptr;
}

template<>
std::shared_ptr<GameCharacter> Map::get<GameCharacter>(const sf::Vector2<int>& pos) 
{
	for (auto& gameCharacter : gameCharacters)
		if (gameCharacter.second.find(pos) != gameCharacter.second.end())
			return gameCharacter.second.at(pos);
	return nullptr;
}

template<>
std::shared_ptr<Entity> Map::get<Entity>(const sf::Vector2<int>& pos) 
{
	std::shared_ptr<Tile> tile = get<Tile>(pos);
	std::shared_ptr<GameCharacter> gameCharacter = get<GameCharacter>(pos);
	if (tile.get()) return tile;
	else if (gameCharacter.get()) return gameCharacter;
	return nullptr;
}

template<>
Map& Map::remove<Tile>(const sf::Vector2<int>& pos) 
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
Map& Map::remove<GameCharacter>(const sf::Vector2<int>& pos)
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

template<>
Map& Map::remove<Entity>(const sf::Vector2<int>& pos) 
{
	remove<Tile>(pos);
	remove<GameCharacter>(pos);
	
	return *this;
}


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
		for (auto& tileType : tiles)
			for (auto& tile : tileType.second)
				tile.second->render(window);

		for (auto& gameCharacterType : gameCharacters)
			for (auto& gameCharacter : gameCharacterType.second)
				gameCharacter.second->render(window);
	}
}

bool Map::isOccupied(const sf::Vector2<int>& pos, bool solid)
{
	return get<Tile>(pos).get() && (get<Tile>(pos)->isSolid() || solid);
}

void Map::move(const sf::Vector2<int>& start, const sf::Vector2<int>& end) //#TODO test
{
	for (auto& gameCharacter : gameCharacters)
		if (gameCharacter.second.find(start) != gameCharacter.second.end())
		{
			auto entity = gameCharacter.second.extract(start);

			if (entity)
			{
				entity.key() = end;
				gameCharacter.second.insert(std::move(entity));
			}

		}
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
	//can't easily serialize the tiles and gamecharacters maps becoude of compare function
	//but we can save each entity one by one
	if (fs.getMode() == Archive::Save) 
	{
		uint32_t sizeTiles = tiles.size();
		fs.serialize(sizeTiles);
		for (auto& tileType : tiles) 
		{
			uint32_t sizeType = tileType.second.size();
			fs.serialize(sizeType);
			for (auto& tile : tileType.second)
				fs.serialize(tile.second);
		}
		uint32_t sizeGameCharacters = gameCharacters.size();
		fs.serialize(sizeGameCharacters);
		for (auto& gameCharactersType : gameCharacters)
		{
			uint32_t sizeType = gameCharactersType.second.size();
			fs.serialize(sizeType);
			for (auto& gameCharacter : gameCharactersType.second)
				fs.serialize(gameCharacter.second);
		}
	}
	else 
	{
		tiles.clear();
		gameCharacters.clear();

		uint32_t sizeTiles;
		fs.serialize(sizeTiles);
		for (uint32_t i = 0; i < sizeTiles; i++)
		{
			uint32_t sizeType;
			fs.serialize(sizeType);
			for (uint32_t j = 0; j < sizeType; j++)
			{
				Tile* tile;
				fs.serialize(tile);
				append<Tile>(posFloatToInt(tile->getPos()), tile);
			}
		}
		uint32_t sizeGameCharacters;
		fs.serialize(sizeGameCharacters);
		for (uint32_t i = 0; i < sizeGameCharacters; i++)
		{
			uint32_t sizeType;
			fs.serialize(sizeType);
			for (uint32_t j = 0; j < sizeType; j++)
			{
				GameCharacter* gameCharacter;
				fs.serialize(gameCharacter);
				append<GameCharacter>(posFloatToInt(gameCharacter->getPos()), gameCharacter);
			}
		}
	}
	
}