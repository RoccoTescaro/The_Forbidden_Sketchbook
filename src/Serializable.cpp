#include "../hdr/Serializable.h"
#include "../hdr/Map.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"
#include "../hdr/Player.h"
#include "../hdr/Ranged.h"
#include "../hdr/Melee.h"
#include "../hdr/Bat.h"
#include "../hdr/Wall.h"
#include "../hdr/Hole.h"
#include "../hdr/ColorPedestral.h"
#include "../hdr/WeaponBench.h"
#include "../hdr/TurnSystem.h"


//there was an easy way to automaticaly register all classes by instasiating a static template object in each class
//but working with singleton can offen couse a lot of problems so we just decided to do it manually

Register Wall::registration(static_cast<std::string>(typeid(Wall).name()), Wall::create);
Register Hole::registration(static_cast<std::string>(typeid(Hole).name()), Hole::create);
Register ColorPedestral::registration(static_cast<std::string>(typeid(ColorPedestral).name()), ColorPedestral::create);
Register WeaponBench::registration(static_cast<std::string>(typeid(WeaponBench).name()), WeaponBench::create);
Register Player::registration(static_cast<std::string>(typeid(Player).name()), Player::create);
Register Melee::registration(static_cast<std::string>(typeid(Melee).name()), Melee::create);
Register Ranged::registration(static_cast<std::string>(typeid(Ranged).name()), Ranged::create);
Register Bat::registration(static_cast<std::string>(typeid(Bat).name()), Bat::create);
Register Map::registration(static_cast<std::string>(typeid(Map).name()), Map::create);
Register TurnSystem::registration(static_cast<std::string>(typeid(TurnSystem).name()), TurnSystem::create);
