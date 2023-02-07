#include "..\hdr\Hud.h"

Hud::Hud(Map& map)
{
	player = map.getPlayer();
	auto shrPlayer = player.lock();
	
	filter.setTargetColor(shrPlayer->getFilterColor());

	playerHealth.setTexture(Config::healthBarTexturePath);
	playerHealth.setPos({ 8,128 }); //#TODO decide if define parameters here or in config
	playerHealth.setScale(0.25f,0.25f);
	playerHealth.setMask(Config::healthBarMaskTexturePath);
	playerHealth.setColorMask({ 242.25f,0.f,0.f });
	playerHealth.setChargingLine({55,85}, {55,310});
	playerHealth.setFont(Config::barFontPath);
	playerHealth.setCharacterSize(32);
	playerHealth.setTextPos({ 55,40 });
	playerHealth.setTargetValue(&(shrPlayer->getHealth()));
	playerHealth.setTargetMaxValue(&(shrPlayer->getMaxHealth()));

	playerEnergy.setTexture(Config::energyBarTexturePath);
	playerEnergy.setPos({ 16,0 });
	playerEnergy.setScale(0.25f, 0.25f);
	playerEnergy.setMask(Config::energyBarMaskTexturePath);
	playerEnergy.setColorMask({ 0.f,0.f,0.f });
	playerEnergy.setChargingLine({ 110,35 }, { 143,428 });
	playerEnergy.setFont(Config::barFontPath);
	playerEnergy.setCharacterSize(32);
	playerEnergy.setTextPos({ 144,370 });
	playerEnergy.setTargetValue(&(shrPlayer->getEnergy()));
	playerEnergy.setTargetMaxValue(&(shrPlayer->getMaxEnergy()));

	dialogueManager.setFont(Config::dialogueFontPath);
	dialogueManager.setTexture(Config::dialogueTexturePath);
}

void Hud::update(const float& dt)
{
	auto shrPlayer = player.lock();
	if (!shrPlayer) return;

	filter.setTargetColor(shrPlayer->getFilterColor());
	filter.update(dt);

	playerHealth.update(dt);
	playerEnergy.update(dt);

	//check if player started a dialogue
}

void Hud::render(sf::RenderWindow& window)
{
	filter.render(window);
	
	auto shrPlayer = player.lock();
	if (!shrPlayer) return;

	playerHealth.render(window);
	playerEnergy.render(window);

	//dialogueManager.setTextureRect({ window.getSize().x * 0.22f, window.getSize().y * 0.78f, window.getSize().x * 0.56f, window.getSize().y * 0.14f });
	//dialogueManager.setPos({ window.getSize().x * 0.2f, window.getSize().y * 0.75f });
	//dialogueManager.setScale({ window.getSize().x * 0.6f, window.getSize().y * 0.2f});
	dialogueManager.render(window);
}


