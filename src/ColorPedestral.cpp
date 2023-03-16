#include "../hdr/ColorPedestral.h"
#include "../hdr/Map.h"
#include "../hdr/Player.h"

Trigger ColorPedestral::trigger{ Achievement{"Colors speak louder than words", "First color unlocked in the game", 1} };

ColorPedestral::ColorPedestral()
	: color(255, 125, 125, 255)
{
	static sf::Texture* texture;
	if (!texture) {
		texture = new sf::Texture;
		texture->loadFromFile(Config::colorPedestralTexturePath);
		texture->generateMipmap();
	}
	sprite.setTexture(*texture);
	sf::Rect<int> textureRect{ 200,0,900,1300 };
	sprite.setTextureRect(textureRect);
	sprite.setScale(64.f / textureRect.width, 96.f / textureRect.height);
	sprite.setOrigin(0.f, 867.f);

	colorSprite.setFillColor(color);
	colorSprite.setOrigin(8.f, 8.f);
}

void ColorPedestral::render(sf::RenderWindow& window)
{
	colorSprite.setPosition(getPos() + sf::Vector2<float>{33, -27});
	window.draw(colorSprite);
	Tile::render(window);
};

void ColorPedestral::interact(Map& map, sf::Vector2<float> target, const float& dt)
{
	auto playerShr = map.get<Player>();

	if (color != nullColor) 
	{
		trigger.update();
		playerShr->setFilterColor(color);
		color = nullColor;
		colorSprite.setFillColor(color);
	}
}