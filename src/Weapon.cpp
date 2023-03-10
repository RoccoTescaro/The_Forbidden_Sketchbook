#include "../hdr/Weapon.h"
#include "../hdr/Utils.h"

Weapon::Weapon(uint8_t attack, uint8_t cost, uint8_t range, bool hidden)
	: attack(attack), cost(cost), range(range), hidden(hidden)
{
	//TODO load sprite
	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		if(range>1)
			texture->loadFromFile(Config::rangedWeaponTexturePath);
		else
			texture->loadFromFile(Config::meleeWeaponTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 0,0,420,420 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(32.f / textureRect.width, 32.f / textureRect.height);
	sprite.setOrigin(0.f, 210.f);


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
	static sf::Texture* texture;
	if (!texture)
	{
		texture = new sf::Texture;
		texture->loadFromFile(Config::bulletTexturePath);
		texture->generateMipmap();
	}

	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 0,0,100,100 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(16.f / textureRect.width, 16.f / textureRect.height);
	sprite.setOrigin(0.f, 50.f);
}

void Weapon::Bullet::update(const float& dt)
{

	sf::Vector2<float> pos = sprite.getPosition();
	sf::Vector2<float> dir = Utils::Math::normalize(target - pos);
	pos += dir * speed * dt;
	//sprite.setRotation(dir);
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
