#pragma once
#include "SFML/Window.hpp"
#include "Utils.h"
#include "Config.h"
#include "Input.h"

class State;

class Application
{
public:
	static State& getState(uint8_t index);
	static void addState(State* state); 
	inline static void setState(uint8_t index) { app.currentStateIndex = index; };
	inline static void nextState() { app.currentStateIndex++; };
	inline static void prevState() { app.currentStateIndex--; };

	inline static sf::RenderWindow& getWindow() { return app.window; };
	inline static const float& getDeltaTime() { return app.dt; };

	inline static Input& getInput() { return app.input; };

	static void run();

private:
	Application();

	static Application app;

	std::vector<std::unique_ptr<State>> states;
	uint8_t currentStateIndex = 0;

	sf::RenderWindow window{};
	float dt = 0.f;

	Input input{};
};

