#include "../hdr/Editor.h"
#include "../hdr/Config.h"
#include "../hdr/Tile.h"
#include "../hdr/GameCharacter.h"
#include "../hdr/Player.h"
#include "../hdr/Ranged.h"
#include "../hdr/Melee.h"
#include "../hdr/Bat.h"
#include "../hdr/Wall.h"
#include "../hdr/Hole.h"
#include "../hdr/ColorPedestral.h"
#include "../hdr/TurnSystem.h"

Editor::Editor() 
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

	//Archive arc(Config::editorMapPath, Archive::Load);
	//arc >> *map;

	map->append({ 0,0 }, new Player{ 50,15,190,190,190 }); //#TODO remove
	
	entitiesFactories.emplace_back(Wall::create);
	entitiesFactories.emplace_back(Hole::create);
	entitiesFactories.emplace_back(ColorPedestral::create);
	entitiesFactories.emplace_back(Melee::create);
	entitiesFactories.emplace_back(Bat::create);
	entitiesFactories.emplace_back(Ranged::create);

	placeHolderBackgroundTexture.loadFromFile(Config::placeHolderTexturePath); //TODO fix magic numbers
	placeHolderBackground.setTexture(placeHolderBackgroundTexture);
	placeHolderBackground.setOrigin(placeHolderBackgroundTexture.getSize().x*0.5f, placeHolderBackgroundTexture.getSize().y*0.5f);
	placeHolderBackground.setScale(1.5f, 1.5f);
	placeHolderBackground.setPosition(window.getSize().x*0.5f,window.getSize().y*0.85f);

	factory = entitiesFactories.begin();
	placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
	placeHolderEntity->setPos({ window.getSize().x * 0.5f - placeHolderEntity->getCenter().x, window.getSize().y * 0.85f - placeHolderEntity->getCenter().y });
	previewEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
	previewEntity->getSprite().setColor(sf::Color(255, 255, 255, 120));

	gui = cam.getView();
}

void Editor::update() 
{
	//TRANSITION EFFECT
	transitionEffect.update(dt);

	if (input.isKeyPressed(Input::Esc))
		transitionEffect.start();

	if (transitionEffect.isEnded())
		Application::setState(Application::Index::PAUSE);

	//MOUSE
	mousePos = map->posFloatToInt(input.getMousePos(&cam.getView()));
	mouseIndicator.setPosition(map->posIntToFloat(mousePos));
	mousePosText.setString(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
	mousePosText.setPosition(mouseIndicator.getPosition() +
		sf::Vector2<float>{ map->getCellDim().x - mousePosText.getGlobalBounds().width,
		map->getCellDim().y - mousePosText.getGlobalBounds().height - 4});

	previewEntity->setPos(mouseIndicator.getPosition());

	cam.update(dt);

	//BACKGROUND
	sf::Vector2<float> bgSize = cam.getView().getSize();
	sf::Vector2<float> bgPos = cam.getView().getCenter() - bgSize * 0.5f;
	backgroundSprite.setSize(bgSize);
	backgroundSprite.setPosition(bgPos);
	backgroundShader.setUniform("viewPos", sf::Glsl::Vec2(bgPos));
	backgroundShader.setUniform("viewDim", sf::Glsl::Vec2(bgSize));

	//FACTORY
	if (input.isKeyPressed(Input::Right)) 
	{
		factory++;
		if (factory == entitiesFactories.end()) factory = entitiesFactories.begin();
		placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
		placeHolderEntity->setPos({ window.getSize().x * 0.5f - placeHolderEntity->getCenter().x, window.getSize().y * 0.85f - placeHolderEntity->getCenter().y});
		previewEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
		previewEntity->getSprite().setColor(sf::Color(255,255,255,120));
	}
	else if(input.isKeyPressed(Input::Left))
	{
		if (factory == entitiesFactories.begin()) factory = entitiesFactories.end();
		factory--;
		placeHolderEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
		placeHolderEntity->setPos({ window.getSize().x * 0.5f - placeHolderEntity->getCenter().x, window.getSize().y * 0.85f - placeHolderEntity->getCenter().y });
		previewEntity = static_cast<std::unique_ptr<Entity>>(dynamic_cast<Entity*>((*factory)()));
		previewEntity->getSprite().setColor(sf::Color(255, 255, 255, 120));
	}

	//CREATION
	if (input.isKeyDown(Input::MouseL))
	{
		bool thereIsNoWall = !map->get<Wall>(mousePos).get();
		if (dynamic_cast<Wall*>(placeHolderEntity.get()) && thereIsNoWall) 
		{
			bool canBuild = true;
			sf::Vector2<int> adj[5] = { {0,-1},{1,0},{0,1},{-1,0},{0,0} };
			for (auto i = 0; i < 5; i++)
			{
				if (!map->get<Wall>(mousePos + adj[i]).get() && i != 4) continue;
				uint8_t nWall = 0;
				
				for (auto j = 0; j < 4; j++) 
					nWall += (bool) map->get<Wall>(mousePos + adj[i] + adj[j]).get();

				if (nWall > 1+(i==4)) 
				{
					canBuild = false;
					break;
				}
			}
			if (canBuild) 
			{
				map->append(mousePos, dynamic_cast<Wall*>((*factory)()));
				for (auto i = 0; i < 5; i++) 
				{
					if (!map->get<Wall>(mousePos + adj[i]).get()) continue;
					bool u = map->get<Wall>(mousePos + adj[i] + adj[0]).get();
					bool r = map->get<Wall>(mousePos + adj[i] + adj[1]).get();
					bool d = map->get<Wall>(mousePos + adj[i] + adj[2]).get();
					bool l = map->get<Wall>(mousePos + adj[i] + adj[3]).get();
					uint8_t index = (d & (r | l)) << 2 | (u & (r | l)) << 1 | (!r & (d | u));
					map->get<Wall>(mousePos + adj[i])->setTexture(index);
				}
			}
		}
		else map->append(mousePos, dynamic_cast<Entity*>((*factory)()));
	}

	if (input.isKeyDown(Input::MouseR) && 
		map->posFloatToInt(input.getMousePos(&cam.getView())) != map->posFloatToInt(map->get<Player>()->getPos())) //TODO update wall texture on remove
		map->remove(map->posFloatToInt(input.getMousePos(&cam.getView())));
}

void Editor::render() 
{
	window.setView(cam.getView());
	window.draw(backgroundSprite, &backgroundShader);
	map->render(window);
	window.draw(mouseIndicator);
	previewEntity->render(window);
	window.draw(mousePosText);
	window.setView(gui);
	window.draw(placeHolderBackground);
	placeHolderEntity->render(window);
	transitionEffect.render(window);
}

void Editor::save()
{
	Archive arc(Config::editorMapPath, Archive::Save);
	arc << *map;
}

void Editor::load()
{
	Archive arc(Config::editorMapPath, Archive::Load);
	arc >> *map;
}

