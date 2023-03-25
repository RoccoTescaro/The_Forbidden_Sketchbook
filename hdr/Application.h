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
	Application& operator=(const Application&) = delete;

	inline static Application& get() { return app; };

	enum Index 
	{ 
		MENU, 
		GAME, 
		PAUSE, 
		EDITOR,
		ENUM_SIZE 
	}; 

	State* getState(Index index); //return the state based on index, to determine the index see in run method states definition
	State* getCurrentState(); 
	State* getPrevState(); 
	
	inline void setState(Index index) 
	{ 
		app.previousStateIndex = app.currentStateIndex; 
		app.currentStateIndex = index; 
	};

	inline void prevState() { std::swap(app.currentStateIndex,app.previousStateIndex); };

	inline sf::RenderWindow& getWindow() { return *app.window; }; 
	inline const float& getDeltaTime() { return app.dt; };
	inline Input& getInput() { return app.input; };

	static void run(); //initialize the application and main loop
private:
	Application() = default;
	static Application app;

	std::unique_ptr<State> states[Index::ENUM_SIZE]{};
	Index currentStateIndex = MENU;
	Index previousStateIndex = MENU; //Will be overwritten after first setState

	//unique_ptr couse sfml creates a window on sf::RenderWindow initialization, 
	//but we dont want to open the window till run method is called
	std::unique_ptr<sf::RenderWindow> window; 
	float dt = 0.f;
	Input input{};
};


