#include "../hdr/Camera.h"

Camera::Camera() 
	: input(Application::getInput())
{
	setView(Application::getWindow().getSize());
	setCenter(sf::Vector2<int>{
		sf::Vector2<float>{ (float)Application::getWindow().getSize().x,
			(float)Application::getWindow().getSize().y } *0.5f });
};

void Camera::update()
{
	float dt = Application::getDeltaTime(); //TODO decide if make this indipendent from application or not
	const sf::Vector2<float>& mousePos = input.getMousePos(&view);
	std::shared_ptr shrTarget = target.lock();

	if (locked && shrTarget.get())
	{
		sf::Vector2<float> dir;
		dir = shrTarget->getCenter() - view.getCenter();
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

void Camera::setTarget(const std::shared_ptr<GameCharacter>& target)
{
	locked = true;
	this->target = target;
}