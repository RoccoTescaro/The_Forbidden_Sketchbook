#pragma once
#include "SFML/Graphics.hpp"
#include "Utils.h"
#include "Config.h"
#include <vector>

class State;

class Application
{
public:
	inline static State& getState(uint8_t index) { return *app.states[index]; };
	inline static void addState(const State& state) { app.states.emplace_back(state); };
	inline static void setState(uint8_t index) { app.currentStateIndex = index; };
	inline static void nextState() { app.currentStateIndex++; };
	inline static void prevState() { app.currentStateIndex--; };

	inline static sf::RenderWindow& getWindow() { return app.window; };
	inline static const float& getDeltaTime() { return app.dt; };

	inline static void run() 
	{
		sf::Clock clock;
		float dt = 0.0001f;

		sf::RenderWindow window(sf::VideoMode(Config::windowDim.x, Config::windowDim.y), "", sf::Style::Default);
		window.setFramerateLimit(Config::fps);

		std::unique_ptr<State>& state = app.states[app.currentStateIndex];
		sf::Event ev{};

		while (window.isOpen())
		{
			ASSERT("states got destroyed", state);
			
			//#TODO add input update
			state->update(); 
			
			window.clear();
			state->render();
			window.display();

			dt = fmax(clock.getElapsedTime().asSeconds(), 0.0001f);
			clock.restart();
		}
	};

private:
	Application() 
	{
		//#TODO add states
	};

	static Application app;

	std::vector<std::unique_ptr<State>> states;
	uint8_t currentStateIndex;

	sf::RenderWindow window;
	float dt;
};

Application Application::app{};