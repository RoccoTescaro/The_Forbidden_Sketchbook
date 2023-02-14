#pragma once
#include <SFML/Graphics.hpp>

class VignetteEffect 
{
public:
	VignetteEffect();

	void update(const float& dt);
	void render(sf::RenderWindow& window);

	void startAnimation();
	bool isAnimationEnded() const;

private:

	void openingUpdate();
	void closingUpdate();

	sf::Shader shader;
	sf::RectangleShape vignette;
	float openingInterpolationProgress = 0.f;
	float openingInterpolationFactor;
	const float openingAnimationSpeed = 1.0005f;
	float radius;
	const float defaultRadius = 0.2f; //1-radius
	const float radiusFrequency = 0.75f;
	const float radiusAmplitude = 0.1f;
	float intensity;
	const float defaultIntensity = 20.f;
	const float closingAnimationSpeed = 2.f;
	float time = 0.f;
	bool animationStarted = true;
	bool closingAnimation = false; 
};