#include "../hdr/Tile.h"
#include "../hdr/Map.h"
#include <array>

//WALL

Wall::Wall(uint8_t type) : type(type)
{
    setTexture(type);
}

void Wall::setTexture(uint8_t newType){

    static std::array<sf::Texture*, Type::ENUM_SIZE> texture;
	for (int i = 0; i < Type::ENUM_SIZE; i++)
		if (!texture[i])
		{
			texture[i] = new sf::Texture;
			texture[i]->loadFromFile(Config::wallTexturePath[i]);
			texture[i]->generateMipmap();
		}
	type = newType;
	sprite.setTexture(*texture[type]);
	sf::Rect<int> textureRect{0,0,1300,1300};
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,96.f/textureRect.height);
	sprite.setOrigin(0.f,867.f);
}

//HOLE

Hole::Hole(){

	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile(Config::holeTexturePath);
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 0,0,1080,1080 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);
	sprite.setOrigin(0.f, 405.f);
}

//COLORPEDESTRAL


ColorPedestral::ColorPedestral()
	: color(255,125,125,255)
{
	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile(Config::colorPedestralTexturePath);
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 200,0,900,1300 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f / textureRect.width, 96.f / textureRect.height);
	sprite.setOrigin(0.f, 867.f);

	colorSprite.setFillColor(color);
	colorSprite.setOrigin(8.f,8.f);

}

void ColorPedestral::render(sf::RenderWindow &window) 
{
	colorSprite.setPosition(getPos()+sf::Vector2<float>{33,-27});
	window.draw(colorSprite);
	Tile::render(window);
};

void ColorPedestral::interact(Map &map, sf::Vector2<float> target, const float &dt) 
{
	auto playerShr = map.get<Player>();

	if(color != nullColor){
		playerShr->setFilterColor(color);
		color = nullColor;
		colorSprite.setFillColor(color);
	}
}

//WEAPONBENCH


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