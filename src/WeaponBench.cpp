#include "../hdr/WeaponBench.h"
#include "../hdr/Map.h"
#include "../hdr/Player.h"
#include "../hdr/Weapon.h"

WeaponBench::WeaponBench()
{
	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile(Config::weaponBenchTexturePath);
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 200,0,900,1300 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f / textureRect.width, 96.f / textureRect.height);
	sprite.setOrigin(0.f, 867.f);
}

void WeaponBench::interact(Map &map, sf::Vector2<float> target, const float &dt) 
{
	
	auto playerShr = map.get<Player>();
	
	Weapon meleeWeapon {5,2,1,false};
	Weapon rangedWeapon {3,3,4,false};

	if(playerShr->getWeapon().isHidden())
		playerShr->setWeapon(meleeWeapon);
	else
		if(playerShr->getWeapon().getRange() == 1)
			playerShr->setWeapon(rangedWeapon);
		else
			playerShr->setWeapon(meleeWeapon);
	
	playerShr->getWeapon().setPos(playerShr->getPos());
}