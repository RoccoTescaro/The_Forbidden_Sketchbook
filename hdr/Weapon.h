#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <memory>

class Weapon
{
public:

	class Bullet 
	{
	public:

		Bullet(const sf::Vector2<float>& target, float speed = 800.f);

		void update(const float& dt);
		void render(sf::RenderWindow& window);

		inline sf::Vector2<float> getPos(){ return sprite.getPosition();};

		float getTargetDistance();

	private:
		const sf::Vector2<float> target;
		const float speed;
		sf::Sprite sprite;
	};

public:

	Weapon(uint8_t attack = 1, uint8_t cost = 1, uint8_t range = 1, bool hidden = false);

	void update(const sf::Vector2<float>& target, const float& dt);
	void render(sf::RenderWindow& window);

    inline uint8_t getAttack() const { return attack; }; 
    inline uint8_t getCost() const { return cost; }; 
    inline uint8_t getRange() const { return range; }; 


	inline virtual void setPos(const sf::Vector2<float>& pos) { sprite.setPosition(pos); };


	inline bool isAnimationEnded() { return !bullet.get();};
 
private:
	const uint8_t attack;
	const uint8_t cost;
	const uint8_t range;
	const bool hidden;
	sf::Sprite sprite;
	std::unique_ptr<Bullet> bullet;
};

