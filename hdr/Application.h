#pragma once
#include "SFML/Window.hpp"
#include <algorithm>
#include "Utils.h"
#include "Config.h"
#include "Input.h"

class State;

class Application
{
public:
	static State& getState(uint8_t index);
	static void addState(State* state); 
	inline static void setState(uint8_t index) { app.previousStateIndex = app.currentStateIndex; app.currentStateIndex = index; };
	inline static void nextState() { app.previousStateIndex = app.currentStateIndex; app.currentStateIndex++; };
	inline static void prevState() { std::swap(app.currentStateIndex,app.previousStateIndex); };

	inline static sf::RenderWindow& getWindow() { return app.window; };
	inline static const float& getDeltaTime() { return app.dt; };

	inline static Input& getInput() { return app.input; };

	static void run();

private:
	Application();

	static Application app;

	std::vector<std::unique_ptr<State>> states;
	uint8_t currentStateIndex = 0;
	uint8_t previousStateIndex = -1; //#TODO fix me

	sf::RenderWindow window{};
	float dt = 0.f;

	Input input{};
};

