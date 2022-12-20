#include "../hdr/Camera.h"

void Camera::update()
{
	float dt = Application::getDeltaTime();
	const sf::Vector2<float>& mousePos = input.getMousePos();

	if (locked)
	{
		sf::Vector2<float> dir;
		//dir = target->getPos() - view.getCenter();
		dir *= dt;
		view.move(dir);
		return;
	}

	view.zoom(1 - input.getWheelDelta() * viewZoomSpeed);
	view.move
	(
		(mousePos.x - view.getCenter().x) * input.getWheelDelta() * viewZoomSpeed,
		(mousePos.y - view.getCenter().y) * input.getWheelDelta() * viewZoomSpeed
	);

	viewMovementSpeed = view.getSize().x * 0.5f;

	int viewDirX = input.isKeyDown(Input::Key::D) - input.isKeyDown(Input::Key::A);
	int viewDirY = input.isKeyDown(Input::Key::S) - input.isKeyDown(Input::Key::W);

	view.move(viewDirX * viewMovementSpeed * dt, viewDirY * viewMovementSpeed * dt);
}

void Camera::setTarget(std::shared_ptr<GameCharacter>& target)
{
	locked = true;
	this->target = target;
}