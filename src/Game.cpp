#include "../hdr/Game.h"

Game::Game()
	: turnSystem(map)
{

	//test 


	backgroundTexture.loadFromFile(Config::gameBackgroundTexturePath); //FIX bg
	backgroundShader.loadFromFile(Config::backgroundShaderPath, sf::Shader::Fragment);
	backgroundSprite.setPosition(0, 0);
	backgroundShader.setUniform("viewPortDim", sf::Glsl::Vec2(window.getSize()));
	backgroundShader.setUniform("resolution", sf::Glsl::Vec2(backgroundTexture.getSize()));
	backgroundShader.setUniform("texture", backgroundTexture);

	mouseIndicator.setOutlineThickness(3);
	mouseIndicator.setSize(sf::Vector2<float>(map.getCellDim()));
	mouseIndicator.setFillColor(sf::Color(0, 0, 0, 0));
	mouseIndicator.setOutlineColor(sf::Color(255,255,255,255));
	mouseFont.loadFromFile(Config::dialogueFontPath);
	mousePosText.setFont(mouseFont);
	mousePosText.setCharacterSize(16);
	window.setMouseCursorVisible(false);

	/*
	Archive arc(Config::gameMapPath, Archive::Load);
	arc >> map >> turnSystem;
	ASSERT(map.getPlayer().get(), "Doesn't exist a player in the map");
	*/

	map.add({ 0,0 }, new Player); //TODO remove this initialization

	actor = turnSystem.getActor(); //we need to initialize the actor to update him


    map.addGameCharacter({8,8}, new Melee(1,1));
    map.addGameCharacter({3,3}, new Melee(1,1));
    map.addGameCharacter({8,3}, new Melee(1,1));


	cam.lock(true);
	cam.setTarget(actor); 

	hud.setView(cam.getView());
	hud.setPlayer(map.getPlayer());
}

void Game::update()
{
	//TRANSITION EFFECT
	transitionEffect.update(dt);

	if (input.isKeyPressed(Input::Enter))
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

	cam.update(); 

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
    //TURNSYSTEM
    if(actor.get()->getEnergy()==0)
        actor=turnSystem.getActor();
    if(!turnSystem.isPlayerTurn()){
        actor.get()->updateStepQueue(map, map.getPlayer().get()->getPos());
    }
    else{
        if(actor.get()->isStepQueueEmpty()&&input.isKeyReleased(Input::MouseL)){
                    actor.get()->updateStepQueue(map, map.posIntToFloat(mousePos));
        }
    }
    actor.get()->update(map, Application::getDeltaTime());


}

void Game::render() 
{
	window.setView(cam.getView());
	window.draw(backgroundSprite, &backgroundShader);
	map.render(window);
	window.draw(mouseIndicator);
	window.draw(mousePosText);
	hud.render(window);
	transitionEffect.render(window);
}

