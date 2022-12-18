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
	time += Application::getDeltaTime();

	sf::RenderWindow& window = Application::getWindow();
	vignette.setSize(sf::Vector2<float>{window.getSize()});

	if (animationStarted) //CLOSE SCENE
		intensity = Utils::Math::lerp(intensity, 0.f, Application::getDeltaTime());
	else				  //OSCILLATING RADIUS 
		radius = minRadius + std::abs(std::sin(time * radiusFrequency)) * radiusAmplitude;		
		//more spiky ocillation
		//Utils::Math::clamp(minRadius, maxRadius,
		//		Utils::Math::lerp(radius,radius+((float)std::rand()/RAND_MAX - 0.5f)*radiusNoise,Application::getDeltaTime())
		//					);
		
	shader.setUniform("radius",radius);
	shader.setUniform("intensity", intensity);
}

void VignetteEffect::render() 
{
	//#TODO check view
	Application::getWindow().draw(vignette, &shader);
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