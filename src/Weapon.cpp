#include "../hdr/Weapon.h"
#include "../hdr/Utils.h"

Weapon::Weapon(uint8_t attack, uint8_t cost, uint8_t range, bool hidden)
	: attack(attack), cost(cost), range(range), hidden(hidden)
{
	//TODO load sprite
}

void Weapon::update(const sf::Vector2<float>& target, const float& dt)
{
	if (bullet.get()) 
	{
		bullet->update(dt);
		if (bullet->getTargetDistance() < Config::epsDistance)
			bullet.reset();
		return;
	}
	
	bullet = static_cast<std::unique_ptr<Bullet>>(new Bullet{target});

}

void Weapon::render(sf::RenderWindow& window)
{
	if (!hidden) window.draw(sprite);
	if (bullet) bullet->render(window);
}

Weapon::Bullet::Bullet(const sf::Vector2<float>& target, float speed)
	: target(target), speed(speed)
{
	//TODO load sprite
}

void Weapon::Bullet::update(const float& dt)
{

	sf::Vector2<float> pos = sprite.getPosition();
	sf::Vector2<float> dir = Utils::Math::normalize(target - pos);
	pos += dir * speed * dt;
	sprite.setPosition(pos);
}

void Weapon::Bullet::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

float Weapon::Bullet::getTargetDistance()
{
	return Utils::Math::distance(target,sprite.getPosition());
}
