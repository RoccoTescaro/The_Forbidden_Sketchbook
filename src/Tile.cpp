#include "../hdr/Tile.h"

//WALL

Wall::Wall(uint8_t type){
    setTexture(type);
}

void Wall::setTexture(uint8_t newType){
    
    static sf::Texture* texture[Type::ENUM_SIZE];
	for (int i = 0; i < Type::ENUM_SIZE; i++)
		if (!texture[i])
		{
			texture[i] = new sf::Texture;
			texture[i]->loadFromFile("../images/textures/"+std::to_string(i)+".png");
			texture[i]->generateMipmap();
		}
	sprite.setTexture(*texture[type]);
	sf::Rect<int> textureRect{0,0,1300,1350};
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

	Wall::type=newType;
}


const bool Wall::isSolid() const{
	return true;
}


//HOLE

Hole::Hole(){

	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/hole.png");
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect({0,0,1080,1080});
	sprite.setScale(64.f/1080,64.f/1080);
}


const bool Hole::isSolid() const {
	return false;
}


//COLORPEDESTRAL


ColorPedestral::ColorPedestral(){

	static sf::Texture* texture;
	if(!texture){
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/colorPedestral.png");
		texture->generateMipmap();	
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect({0,0,1080,1080});
	sprite.setScale(64.f/1080,64.f/1080);
}

const bool ColorPedestral::isSolid() const {
	return true;
}

void ColorPedestral::execute(GameCharacter &gameCharacter, Map &map){}