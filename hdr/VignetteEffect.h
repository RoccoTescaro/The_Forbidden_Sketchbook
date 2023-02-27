#pragma once
#include <SFML/Graphics.hpp>

class VignetteEffect 
{
public:
	VignetteEffect();

	void update(const float& dt);
	void render(sf::RenderWindow& window);

	void start();
	bool isEnded() const;

private:
	sf::Shader shader;
	sf::RectangleShape vignette;

	const float defaultRadius = 0.8f;
	const float defaultIntensity = 15.f;
	const float radiusOscillationAmplitude = 0.1f;
	const float radiusOscillationFrequency = 1.f;
	const float animationSpeed = 0.75f;

	float time;
	float radius;
	float intensity;
	float animationProgress = 0.f;
	bool ended = true;
	bool waiting = false;
};