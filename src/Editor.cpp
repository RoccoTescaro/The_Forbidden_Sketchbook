#include "../hdr/Editor.h"
#include "../hdr/Config.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"

Editor::Editor() 
    : cam(sf::Vector2<float>{ Application::getWindow().getSize() })
{   
	backgroundTexture.loadFromFile(Config::gameBackgroundTexturePath);
	backgroundShader.loadFromFile(Config::backgroundShaderPath, sf::Shader::Fragment);
	backgroundSprite.setPosition(0, 0);
	backgroundShader.setUniform("viewPortDim", sf::Glsl::Vec2(window.getSize()));
	backgroundShader.setUniform("resolution", sf::Glsl::Vec2(backgroundTexture.getSize()));
	backgroundShader.setUniform("texture", backgroundTexture);

	mouseIndicator.setOutlineThickness(3);
	mouseIndicator.setSize(sf::Vector2<float>(map.getCellDim()));
	mouseIndicator.setFillColor(sf::Color(0, 0, 0, 0));
	mouseIndicator.setOutlineColor(sf::Color(255, 255, 255, 255));
	mouseFont.loadFromFile(Config::dialogueFontPath);
	mousePosText.setFont(mouseFont);
	mousePosText.setCharacterSize(16);
	window.setMouseCursorVisible(false);

	//Archive arc(Config::gameMapPath, Archive::Load);
	//arc >> map;

	map.add({ 0,0 }, new Player);
	
	entitiesFactories.emplace_back(Wall::create);
	entitiesFactories.emplace_back(Hole::create);
	entitiesFactories.emplace_back(ColorPedestral::create);
	entitiesFactories.emplace_back(Melee::create);
	entitiesFactories.emplace_back(Bat::create);
	entitiesFactories.emplace_back(Ranged::create);

	factory = entitiesFactories.begin();
	placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));

	gui = cam.getView();
}

void Editor::update() 
{
	//TRANSITION EFFECT
	transitionEffect.update(dt);

	if (input.isKeyPressed(Input::Esc))
		transitionEffect.start();

	if (transitionEffect.isEnded())
		Application::nextState();

	//MOUSE
	mousePos = map.posFloatToInt(input.getMousePos(&cam.getView()));
	mouseIndicator.setPosition(map.posIntToFloat(mousePos));
	mousePosText.setString(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
	mousePosText.setPosition(mouseIndicator.getPosition() +
		sf::Vector2<float>{ map.getCellDim().x - mousePosText.getGlobalBounds().width,
		map.getCellDim().y - mousePosText.getGlobalBounds().height - 4});

	//CAMERA

	if (input.isKeyPressed(Input::Space))
		cam.lock();

	if (input.isKeyReleased(Input::MouseR) && map.getGameCharacter(mousePos).get())
		cam.setTarget(map.getGameCharacter(mousePos));

	cam.update(dt);

	//BACKGROUND
	sf::Vector2<float> bgSize = cam.getView().getSize();
	sf::Vector2<float> bgPos = cam.getView().getCenter() - bgSize * 0.5f;
	backgroundSprite.setSize(bgSize);
	backgroundSprite.setPosition(bgPos);
	backgroundShader.setUniform("viewPos", sf::Glsl::Vec2(bgPos));
	backgroundShader.setUniform("viewDim", sf::Glsl::Vec2(bgSize));

	//FACTORY
	if (input.getWheelDelta() > 0) 
	{
		factory++;
		if (factory == entitiesFactories.end()) factory = entitiesFactories.begin();
		placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
	}
	else if(input.getWheelDelta() < 0)
	{
		if (factory == entitiesFactories.begin()) factory = entitiesFactories.end();
		factory--;
		placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
	}

	if (input.isKeyReleased(Input::MouseL)) 
	{
		if (dynamic_cast<Wall*>(placeHolderEntity.get())); //wall inititalization based on map
		else map.add(mousePos, dynamic_cast<Entity*>((*factory)()));
	}
}

void Editor::render() 
{
	window.setView(cam.getView());
	window.draw(backgroundSprite, &backgroundShader);
	map.render(window);
	window.draw(mouseIndicator);
	window.draw(mousePosText);
	window.setView(gui);
	//window.draw(placeHolderBackground);
	//placeHolderEntity->render(window);
	transitionEffect.render(window);
}

