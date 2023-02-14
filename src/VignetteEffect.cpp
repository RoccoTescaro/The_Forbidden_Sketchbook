#include "../hdr/VignetteEffect.h"
#include "../hdr/Utils.h"
#include "../hdr/Application.h"

VignetteEffect::VignetteEffect() :
	radius(defaultRadius), intensity(0.f)
{
	sf::RenderWindow& window = Application::getWindow();
	shader.loadFromFile(Config::vignetteEffectShaderPath, sf::Shader::Fragment);
	shader.setUniform("resolution", sf::Vector2<float>(window.getSize()));
	vignette.setPosition(0, 0);
	vignette.setSize(sf::Vector2<float>{window.getSize()});
}

void VignetteEffect::update(const float& dt) 
{
	time += dt;

	radius = defaultRadius + std::abs(std::sin(time * radiusFrequency)) * radiusAmplitude;
	
	if (animationStarted)
	{
		if (closingAnimation) closingUpdate();	
		else openingUpdate();
	}
	

	shader.setUniform("radius",radius);
	shader.setUniform("intensity", intensity);
}

void VignetteEffect::render(sf::RenderWindow& window) 
{
	//TODO test/check view
	window.draw(vignette, &shader);
}

void VignetteEffect::startAnimation() 
{
	closingAnimation = true;
	animationStarted = true;
}

bool VignetteEffect::isAnimationEnded() const 
{
	return intensity <= Config::eps;
}

void VignetteEffect::openingUpdate() //FIX
{
	intensity = Utils::Math::lerp(intensity, defaultIntensity, animationSpeed * Application::getDeltaTime());

	if (intensity >= defaultIntensity - Config::eps)
	{
		closingAnimation = true;
		animationStarted = false;
	}
}

void VignetteEffect::closingUpdate()
{
	intensity = Utils::Math::lerp(intensity, 0.f, animationSpeed * Application::getDeltaTime());

	if (intensity <= Config::eps)
		closingAnimation = false;
}
