#include "..\hdr\Game.h"

Game::Game() 
{
	backgroundTexture.loadFromFile(Config::backgroundTexturePath);
	backgroundShader.loadFromFile(Config::backgroundShaderPath, sf::Shader::Fragment);
	backgroundSprite.setPosition(0, 0);
	backgroundShader.setUniform("resolution", sf::Glsl::Vec2(backgroundTexture.getSize()));
	backgroundShader.setUniform("texture", backgroundTexture);

	mouseIndicator.setOutlineThickness(3);
	mouseIndicator.setSize(sf::Vector2<float>(map.getCellDim()));
	mouseIndicator.setFillColor(sf::Color(0, 0, 0, 0));
	mouseFont.loadFromFile(Config::dialogueFontPath);
	mousePosText.setFont(mouseFont);
	mousePosText.setCharacterSize(16);
	window.setMouseCursorVisible(false);

	Archive arc(Config::gameMapPath, Archive::Load);
	arc >> map; // >> turnSystem;
	ASSERT(map.getPlayer().get(), "Doesn't exist a player in the map")
	
	//turnSystem.load("maps/loadFile.txt");
	//actor = turnSystem.getActor();

	cam.lock(false);
	//cam.setTarget(actor); //TODO setCenter might be needed.

	hud.setView(cam.getView());
	hud.setPlayer(map.getPlayer());
}

void Game::update()
{
	transitionEffect.update(dt);


}

void Game::render() 
{

}

