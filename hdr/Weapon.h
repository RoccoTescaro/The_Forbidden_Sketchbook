#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>

class Weapon
{
public:

	class Bullet 
	{
	public:

		Bullet(const sf::Vector2<float>& target, float speed = 2.5f);

		void update(const float& dt);
		void render(sf::RenderWindow& window);

		float getTargetDistance();

	private:
		const sf::Vector2<float> target;
		const float speed;
		sf::Sprite sprite;
	};

public:

	Weapon(uint8_t attack, uint8_t cost, uint8_t range = 1, bool hidden = false);

	void update(const float& dt);
	void render(sf::RenderWindow& window);

	void shoot(const sf::Vector2<float>& target);

	//bool isAnimationEnded();

private:
	const uint8_t attack;
	const uint8_t cost;
	const uint8_t range;
	const bool hidden;
	sf::Sprite sprite;
	std::unique_ptr<Bullet> bullet;
};

