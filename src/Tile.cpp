#include "../hdr/Tile.h"
#include <array>

//WALL

Wall::Wall(uint8_t type):type(type)
{
    setTexture(type);

}

void Wall::setTexture(uint8_t newType){

    static std::array<sf::Texture*, Type::ENUM_SIZE> texture;
	for (int i = 0; i < Type::ENUM_SIZE; i++)
		if (!texture[i])
		{
			texture[i] = new sf::Texture;
			texture[i]->loadFromFile("../img/Wall"+std::to_string(i)+".png");
			texture[i]->generateMipmap();
		}
	Wall::type=newType;
	sprite.setTexture(*texture[type]);
	sf::Rect<int> textureRect{0,0,1300,1350};
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);

}

bool Wall::isSolid() const{
	return true;
}

//HOLE

Hole::Hole(){

	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile("../img/hole.png");
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect({0,0,1080,1080});
	sprite.setScale(64.f/1080,64.f/1080);
}


bool Hole::isSolid() const {
	return false;
}


//COLORPEDESTRAL


ColorPedestral::ColorPedestral(){

	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile("../img/colorPedestral.png");
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect({0,0,1080,1080});
	sprite.setScale(64.f/1080,64.f/1080);
}

bool ColorPedestral::isSolid() const {
	return true;
}

void ColorPedestral::execute(GameCharacter &gameCharacter, Map &map){}