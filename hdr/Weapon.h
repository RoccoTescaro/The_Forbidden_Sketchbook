#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <memory>

class Weapon //TODO make it serializable
{
public:

	class Bullet 
	{
	public:

		Bullet(const sf::Vector2<float>& pos, const sf::Vector2<float>& target, float speed = 300.f);

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

	Weapon(uint8_t attack = 1, uint8_t cost = 1, uint8_t range = 1, bool hidden = true);

	Weapon(const Weapon &weapon): Weapon(weapon.getAttack(), weapon.getCost(), weapon.getRange(), weapon.isHidden()){};

	void update(const sf::Vector2<float>& target, const float& dt);
	void render(sf::RenderWindow& window);

    inline uint8_t getAttack() const { return attack; }; 
    inline uint8_t getCost() const { return cost; }; 
    inline uint8_t getRange() const { return range; }; 
    inline bool isHidden() const { return hidden; }; 

	inline void setPos(const sf::Vector2<float>& pos) { sprite.setPosition(pos+sf::Vector2<float>{28,-32}); };

	inline bool isAnimationEnded() { return !bullet.get();}; 

	void updateTexture();

private:
	const uint8_t attack;
	const uint8_t cost;
	const uint8_t range;
	const bool hidden;
	sf::Sprite sprite;
	std::unique_ptr<Bullet> bullet;
};

