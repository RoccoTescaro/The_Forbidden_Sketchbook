#pragma once
#include "Config.h"
#include "Utils.h"
#include <algorithm>
#include <memory>
#include "SFML/Window.hpp"
#include "Input.h"

class State; //State Pattern

class Application //Singleton Pattern
{
public:
	Application(const Application&) = delete; 

	//Even if singleton getApplication() or get() is not provvided, we prefered the following form Application::getWindow() 
	//instead of Application::get().window or Application::get().getWindow() (thats the reason for all the method to be static)

	static State* getState(uint8_t index); //return the state based on index, to determine the index see in run method states definition
	static State* getCurrentState(); 
	static State* getPrevState(); 
	
	static void addState(State* state); 

	inline static void setState(uint8_t index) 
	{ 
		app.previousStateIndex = app.currentStateIndex; 
		app.currentStateIndex = index; 
	};

	inline static void nextState() 
	{ 
		app.previousStateIndex = app.currentStateIndex; 
		app.currentStateIndex++; 
	};

	inline static void prevState() { std::swap(app.currentStateIndex,app.previousStateIndex); };

	inline static sf::RenderWindow& getWindow() { return *app.window; }; 
	inline static const float& getDeltaTime() { return app.dt; };
	inline static Input& getInput() { return app.input; };

	static void run(); //initialize the application and main loop
private:
	Application();
	static Application app;

	std::vector<std::unique_ptr<State>> states;
	uint8_t currentStateIndex = 0;
	uint8_t previousStateIndex = -1; //initialized with invalid state index

	//unique_ptr couse sfml creates a window on sf::RenderWindow initialization, 
	//but we dont want to open the window till run method is called
	std::unique_ptr<sf::RenderWindow> window; 
	float dt = 0.f;
	Input input{};
};

