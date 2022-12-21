
#include "../hdr/GameCharacter.h"


//GAMECHARACTER

void GameCharacter::update  (Map &map, const float &dt){}
void GameCharacter::execute (GameCharacter &gameCharacter, Map &map){}
const bool GameCharacter::isInRange (Map &map){}

void GameCharacter::updateStepQueue (Map &map,const sf::Vector2<float> target){

	if(stepQueue.empty())
		stepQueue=movementStrategy->findPath(this->getPos(),target,isSolid());

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
BaseStats Melee::baseStats={100,100,0};

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

//RANGED