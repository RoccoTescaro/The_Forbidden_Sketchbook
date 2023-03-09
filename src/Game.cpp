#include "../hdr/Game.h"

Game::Game()
	: cam(sf::Vector2<float>{ Application::getWindow().getSize() }), map(new Map)
{
	backgroundTexture.loadFromFile(Config::gameBackgroundTexturePath);
	backgroundShader.loadFromFile(Config::backgroundShaderPath, sf::Shader::Fragment);
	backgroundSprite.setPosition(0, 0);
	backgroundShader.setUniform("viewPortDim", sf::Glsl::Vec2(window.getSize()));
	backgroundShader.setUniform("resolution", sf::Glsl::Vec2(backgroundTexture.getSize()));
	backgroundShader.setUniform("texture", backgroundTexture);

	mouseIndicator.setOutlineThickness(3);
	mouseIndicator.setSize(sf::Vector2<float>(map->getCellDim()));
	mouseIndicator.setFillColor(sf::Color(0, 0, 0, 0));
	mouseIndicator.setOutlineColor(sf::Color(255, 255, 255, 255));
	mouseFont.loadFromFile(Config::dialogueFontPath);
	mousePosText.setFont(mouseFont);
	mousePosText.setCharacterSize(16);
	window.setMouseCursorVisible(false);

	turnSystem.init(map);
	//Archive arc(Config::gameMapPath, Archive::Load);
	//arc >> *map >> turnSystem;
	map->append({ 0,0 }, new Player{ 50,15,190,190,190 }); //#TODO remove
	map->append({ 5,3 }, new Melee{ 30, 5 }); //#TODO remove
	

	hud.setView(cam.getView());
	hud.setPlayer(map->get<Player>());
}

void Game::update()
{

	//TRANSITION EFFECT
	transitionEffect.update(dt);

	if (input.isKeyPressed(Input::Esc))
		transitionEffect.start();

	if (transitionEffect.isEnded())
		Application::setState(3);

	//MOUSE
	mousePos = map->posFloatToInt(input.getMousePos(&cam.getView()));
	mouseIndicator.setPosition(map->posIntToFloat(mousePos));
	mousePosText.setString(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
	mousePosText.setPosition(mouseIndicator.getPosition() + 
		sf::Vector2<float>{ map->getCellDim().x - mousePosText.getGlobalBounds().width, 
							map->getCellDim().y - mousePosText.getGlobalBounds().height - 4});

	//CAMERA

	if (input.isKeyPressed(Input::Space))
		cam.lock();

	if(cam.isLocked())
		cam.setTarget(turnSystem.getActor().lock());

	if (input.isKeyReleased(Input::MouseR) && map->get<GameCharacter>(mousePos).get())
		cam.setTarget(map->get<GameCharacter>(mousePos));

	cam.update(dt); 

	//HUD 
	hud.update(dt);

	//BACKGROUND
	sf::Vector2<float> bgSize = cam.getView().getSize();
	sf::Vector2<float> bgPos = cam.getView().getCenter() - bgSize * 0.5f;
	backgroundSprite.setSize(bgSize);
	backgroundSprite.setPosition(bgPos);
	backgroundShader.setUniform("viewPos", sf::Glsl::Vec2(bgPos));
	backgroundShader.setUniform("viewDim", sf::Glsl::Vec2(bgSize));

	//UPDATE ACTOR
    if(!turnSystem.isPlayerTurn())
		turnSystem.turnBuild(map->get<Player>()->getPos());
    else if(input.isKeyReleased(Input::MouseL))
	{
		if(mousePos != map->posFloatToInt(map->get<Player>()->getPos()))
            turnSystem.turnBuild(map->posIntToFloat(mousePos));
		else map->get<Player>()->setEnergy(0);
	}
    
    turnSystem.update(dt);

}

void Game::render() 
{
	window.setView(cam.getView());
	window.draw(backgroundSprite, &backgroundShader);
	map->render(window);
	window.draw(mouseIndicator);
	window.draw(mousePosText);
	hud.render(window);
	transitionEffect.render(window);
}

void Game::save() 
{
	Archive arc(Config::gameMapPath, Archive::Save);
	arc << *map << turnSystem;
}

void Game::load()
{
	Archive arc(Config::gameMapPath, Archive::Load);
	arc >> *map >> turnSystem;

	cam.lock(true);
	cam.setCenter({0,0});

	sf::Vector2<float> size = { (float)window.getSize().x,(float)window.getSize().y };
	hud.setView(sf::View{ size*0.5f,size });
	hud.setPlayer(map->get<Player>());
}