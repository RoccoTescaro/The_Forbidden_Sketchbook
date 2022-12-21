
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
BaseStats Player::baseStats={100,100,0};

Player::Player  (uint8_t health, uint8_t energy, bool filterColorR, bool filterColorG, bool filterColorB):
                GameCharacter(  baseStats.maxHealth,(health>baseStats.maxHealth)?baseStats.maxHealth:health,
                                baseStats.maxEnergy,(energy>baseStats.maxEnergy)?baseStats.maxEnergy:energy,
                                baseStats.priority){

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
BaseStats Melee::baseStats={30,10,2};

Melee::Melee  (uint8_t health, uint8_t energy):
                GameCharacter(  baseStats.maxHealth,(health>baseStats.maxHealth)?baseStats.maxHealth:health,
                                baseStats.maxEnergy,(energy>baseStats.maxEnergy)?baseStats.maxEnergy:energy,
                                baseStats.priority){

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
BaseStats Bat::baseStats={10,30,3};

Bat::Bat  (uint8_t health, uint8_t energy):
                GameCharacter(  baseStats.maxHealth,(health>baseStats.maxHealth)?baseStats.maxHealth:health,
                                baseStats.maxEnergy,(energy>baseStats.maxEnergy)?baseStats.maxEnergy:energy,
                                baseStats.priority){

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

BaseStats Ranged::baseStats={5,50,1};

Ranged::Ranged  (uint8_t health, uint8_t energy):
                GameCharacter(  baseStats.maxHealth,(health>baseStats.maxHealth)?baseStats.maxHealth:health,
                                baseStats.maxEnergy,(energy>baseStats.maxEnergy)?baseStats.maxEnergy:energy,
                                baseStats.priority), animationDuration(1), animationTime(0){

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
