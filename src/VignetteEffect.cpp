#include "../hdr/VignetteEffect.h"
#include "../hdr/Utils.h"
#include "../hdr/Application.h"

VignetteEffect::VignetteEffect() 
{
	sf::RenderWindow& window = Application::getWindow();
	shader.loadFromFile("shd/VignetteEffect.frag", sf::Shader::Fragment);
	shader.setUniform("resolution", sf::Vector2<float>(window.getSize()));
	vignette.setPosition(0, 0);
	reset();
}

void VignetteEffect::update() 
{
	sf::RenderWindow& window = Application::getWindow();
	vignette.setSize(sf::Vector2<float>{window.getSize()});

	if (animationStarted) //CLOSE SCENE
		intensity = Utils::Math::lerp(intensity, 0.f, Application::getDeltaTime()); 
	else				  //OSCILLATING RADIUS
		radius = Utils::Math::clamp(minRadius,														
									maxRadius,
									Utils::Math::lerp(radius,
															radius - (float)std::rand() / RAND_MAX, 
															Application::getDeltaTime()));
	shader.setUniform("radius",radius);
	shader.setUniform("intensity", intensity);
}

void VignetteEffect::render() 
{
	sf::RenderWindow& window = Application::getWindow();
	sf::View preView = window.getView();
	sf::View newView = sf::View(sf::Vector2<float>{0, 0}, sf::Vector2<float>{window.getSize()});
	window.setView(newView);
	window.draw(vignette, &shader);
	window.setView(preView);
}

void VignetteEffect::startAnimation() 
{
	animationStarted = true;
}

bool VignetteEffect::isAnimationEnded() const 
{
	return intensity <= Config::eps;
}

void VignetteEffect::reset() 
{
	radius = defaultRadius;
	intensity = defaultIntensity;
	animationStarted = false;
}