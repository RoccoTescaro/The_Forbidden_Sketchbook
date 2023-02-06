#include "../hdr/Serializable.h"
#include "../hdr/Map.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

Register Map::registration(static_cast<std::string>(typeid(Map).name()), Map::create);
Register Wall::registration(static_cast<std::string>(typeid(Wall).name()), Wall::create);
Register Hole::registration(static_cast<std::string>(typeid(Hole).name()), Hole::create);
Register Player::registration(static_cast<std::string>(typeid(Player).name()), Player::create);
