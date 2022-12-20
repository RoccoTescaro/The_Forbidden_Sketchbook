#pragma once
#include "Application.h"
#include <SFML/System.hpp>

class GameCharacter; //#TODO remove

class Camera
{
public:
	Camera(const sf::Vector2<float>& viewSize) : input(Application::getInput()), view(viewSize * 0.5f, viewSize) {};

	void update();

	void setTarget(std::shared_ptr<GameCharacter>& target);

	inline void lock(bool condition) { locked = condition; };

	inline void lock() { locked = !locked; }; //toggle without parameters

	inline bool isLocked() { return locked; };

	inline const sf::View& getView() const { return view; }

private:
	sf::View view;
	const float viewZoomSpeed = 0.02f;
	float viewMovementSpeed = 0.02f; //movement speed changes with view dim.

	std::weak_ptr<GameCharacter> target;
	bool locked = false;

	const Input& input;
};