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

	radius = defaultRadius + std::sin(time * radiusOscillationFrequency) * radiusOscillationAmplitude;
	
	if (!waiting) 
	{
		if (ended)
		{
			animationProgress += dt * animationSpeed;
			double dAnimationProgress = animationProgress;
			float fPow = std::pow(dAnimationProgress,8);
			intensity = std::min(defaultIntensity, fPow);
			if (intensity >= defaultIntensity)
			{
				waiting = true;
				intensity = defaultIntensity;
			}
		}
		else
		{
			animationProgress -= dt * animationSpeed;
			double dAnimationProgress = animationProgress;
			float fPow = std::pow(dAnimationProgress,8);
			intensity = std::max(0.f, fPow);
			if (animationProgress <= 0.f)
			{
				ended = true;
				intensity = 0.f;
				animationProgress = 0.f;
			}
		}
	}

	shader.setUniform("radius",radius);
	shader.setUniform("intensity", intensity);
}

void VignetteEffect::render(sf::RenderWindow& window) 
{
	window.draw(vignette, &shader);
}

void VignetteEffect::start() 
{
	ended = false;
	waiting = false;
}

bool VignetteEffect::isEnded() const 
{
	return ended && intensity <= 0.f;
}



/*
void VignetteEffect::openingUpdate() 
{
	openingInterpolationFactor *= openingAnimationSpeed;
	openingInterpolationProgress += Application::getDeltaTime() * openingInterpolationFactor;
    intensity = Utils::Math::lerp(intensity, defaultIntensity, openingInterpolationProgress);

	if (intensity >= defaultIntensity - Config::eps)
	{
		intensity = defaultIntensity;
		closingAnimation = true;
		animationStarted = false;
	}
}

void VignetteEffect::closingUpdate()
{
	intensity = Utils::Math::lerp(intensity, 0.f, closingAnimationSpeed * Application::getDeltaTime());

	if (intensity <= Config::eps)
		closingAnimation = false;
}
*/