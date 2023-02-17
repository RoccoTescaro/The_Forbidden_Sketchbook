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
			intensity = std::min(defaultIntensity, std::powf(animationProgress,8));
			if (intensity >= defaultIntensity)
			{
				waiting = true;
				intensity = defaultIntensity;
			}
		}
		else
		{
			animationProgress -= dt * animationSpeed;
			intensity = std::max(0.f, std::powf(animationProgress, 8));
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

