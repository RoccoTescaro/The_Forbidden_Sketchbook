#pragma once
#include <SFML/Graphics.hpp>

class VignetteEffect 
{
public:
	VignetteEffect();

	void update();
	void render();

	void startAnimation();
	bool isAnimationEnded() const;

private:

	void openingUpdate();
	void closingUpdate();

	sf::Shader shader;
	sf::RectangleShape vignette;
	float radius;
	const float defaultRadius = 1.f;
	const float radiusFrequency = 1.f;
	const float radiusAmplitude = 1.25f;
	float intensity;
	const float defaultIntensity = 15.f;
	const float animationSpeed = 1.75f;
	float time = 0.f;
	bool animationStarted = true;
	bool closingAnimation = false; 
};