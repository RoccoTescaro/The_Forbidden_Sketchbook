#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class State;

class Application
{
public:
	static Application app;

	std::vector<std::unique_ptr<State>> states;
	uint8_t currentStateIndex;
	inline static State& getState(uint8_t index) { return *app.states[index]; };
	inline static void addState(const State& state) { app.states.emplace_back(state); };
	inline static void setState(uint8_t index) { app.currentStateIndex = index; };
	inline static void nextState() { app.currentStateIndex++; };
	inline static void prevState() { app.currentStateIndex--; };

	sf::RenderWindow window;
	inline static sf::RenderWindow& getWindow() { return app.window; };

	float dt;
	inline static const float& getDeltaTime() { return app.dt; };

	inline static void run() 
	{
	
	};

private:
	Application() 
	{
		//#TODO add states
	};
};

Application Application::app{};