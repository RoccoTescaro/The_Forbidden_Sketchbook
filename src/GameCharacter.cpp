
#include "../hdr/GameCharacter.h"


//GAMECHARACTER

void GameCharacter::update  (Map &map, const float &dt){}
void GameCharacter::execute (GameCharacter &gameCharacter, Map &map){}
const bool GameCharacter::isInRange (Map &map){return true;}

void GameCharacter::updateStepQueue (Map &map,const sf::Vector2<float> target){

	if(stepQueue.empty())
		//stepQueue=movementStrategy->findPath(this->getPos(),target,isSolid())
		;

}

//PLAYER

Player::Player  (uint8_t health, uint8_t energy, bool filterColorR, bool filterColorG, bool filterColorB):
                GameCharacter(  100,(health>100)?100:health,
                                100,(energy>100)?100:energy,
                                0){

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/Player.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

	//FILTER
	filterColor = sf::Color(filterColorR*255, filterColorG*255, filterColorB*255, 255);

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}

//MELEE

Melee::Melee  (uint8_t health, uint8_t energy):
                GameCharacter(  30,(health>30)?30:health,
                                10,(energy>10)?10:energy,
                                2){

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/Melee.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}
//BAT

Bat::Bat  (uint8_t health, uint8_t energy):
                GameCharacter(  10,(health>10)?10:health,
                                30,(energy>30)?30:energy,
                                3){

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/Bat.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new AStar());
}
//RANGED


Ranged::Ranged  (uint8_t health, uint8_t energy):
                GameCharacter(  5,(health>5)?5:health,
                                50,(energy>50)?50:energy,
                                1), animationDuration(1), animationTime(0){

	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile("../images/textures/Ranged.png");
		texture->generateMipmap();
	}

	sprite.setTexture(*texture); 
	sf::Rect<int> textureRect{ 0,0,1080,1920 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f/textureRect.width,64.f/textureRect.height);//TODO ADD GETCELLDIM

    //MOVEMENTSTRATEGY
    movementStrategy = std::unique_ptr<PathAlgorithm>(new DiglettMovement());
}

void Ranged::update  (Map &map, const float &dt){}
