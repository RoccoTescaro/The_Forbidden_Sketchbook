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

	void reset();
private:
	sf::Shader shader;
	sf::RectangleShape vignette;
	float radius;
	const float defaultRadius = 1.f;
	const float minRadius = 0.75f;
	const float maxRadius = 1.25f;
	float intensity;
	const float defaultIntensity = 15.f;
	bool animationStarted = false;
};