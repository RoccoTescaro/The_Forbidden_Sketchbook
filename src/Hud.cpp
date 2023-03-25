#include "../hdr/Hud.h"
#include "../hdr/Player.h"

Hud::Hud() 
{
	playerHealth.setTexture(Config::healthBarTexturePath);
	playerHealth.setPos({ 8,128 }); 
	playerHealth.setScale(0.25f,0.25f);
	playerHealth.setMask(Config::healthBarMaskTexturePath);
	playerHealth.setColorMask({ 255,20,20 });
	playerHealth.setChargingLine({55,85}, {55,310});
	playerHealth.setFont(Config::barFontPath);
	playerHealth.setCharacterSize(32);
	playerHealth.setTextPos({ 55,40 });
	
	playerEnergy.setTexture(Config::energyBarTexturePath);
	playerEnergy.setPos({ 16,0 });
	playerEnergy.setScale(0.25f, 0.25f);
	playerEnergy.setMask(Config::energyBarMaskTexturePath);
	playerEnergy.setColorMask({ 0,0,0 });
	playerEnergy.setChargingLine({ 110,35 }, { 143,428 });
	playerEnergy.setFont(Config::barFontPath);
	playerEnergy.setCharacterSize(32);
	playerEnergy.setTextPos({ 144,370 });
	
	dialogueManager.setFont(Config::dialogueFontPath);
	dialogueManager.setTexture(Config::dialogueTexturePath);
	dialogueManager.setTextSize(20);
	sf::Vector2<int> windowDim{ (int)Application::get().getWindow().getSize().x, (int)Application::get().getWindow().getSize().y };
	dialogueManager.setPos(sf::Vector2<int>{windowDim.x * 5 / 8 - 16, 8});
	dialogueManager.setScale(0.5f, 0.6f);
	dialogueManager.setTextBox(sf::Rect<int>{windowDim.x * 5 / 8, 20 , windowDim.x * 3 / 8, windowDim.y / 10});
}

void Hud::setPlayer(const std::shared_ptr<Player>& player) 
{
	this->player = player;
	filter.setTargetColor(player->getFilterColor());
	playerHealth.setTargetValue(&(player->getHealth()));
	playerHealth.setTargetMaxValue(player->getMaxHealth());
	playerEnergy.setTargetValue(&(player->getEnergy()));
	playerEnergy.setTargetMaxValue(player->getMaxEnergy());

}

void Hud::update(const float& dt)
{
	auto shrPlayer = player.lock();
	if (!shrPlayer) return;

	filter.setTargetColor(player.lock()->getFilterColor());
	filter.update(dt);

	playerHealth.update(dt);
	playerEnergy.update(dt);

	if(Application::get().getInput().isKeyPressed(Input::Enter) || dialogueManager.isAnimationEnded())
		dialogueManager.next();

	dialogueManager.update(dt);
}

void Hud::showAchievement(const std::string& name, const std::string& description)
{
	dialogueManager.addText(name + ":\n" + description);
	dialogueManager.show();
}

void Hud::render(sf::RenderWindow& window)
{
	window.setView(gui);

	filter.render(window);
	
	auto shrPlayer = player.lock();
	if (!shrPlayer) return;

	playerHealth.render(window);
	playerEnergy.render(window);

	dialogueManager.render(window);
}


