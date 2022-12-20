#include "../hdr/Map.h"

void Map::render()
{
	for (auto& tile : tiles)
		break;//tile.second->render(); 
	for (auto& gameCharacter : gameCharacters)
		break;//gameCharacter.second->render();
};

std::shared_ptr<Tile> Map::getTile(const sf::Vector2<int>& pos) 
{
	if (tiles.find(pos) != tiles.end())
		return tiles.at(pos);
	return nullptr;
}

std::shared_ptr<GameCharacter> Map::getGameCharacter(const sf::Vector2<int>& pos) 
{
	if (gameCharacters.find(pos) != gameCharacters.end())
		return gameCharacters.at(pos);
	return nullptr;
}

std::shared_ptr<Player> Map::getPlayer() 
{
	return player;
}