#pragma once
#include "Application.h"
#include <SFML/Graphics.hpp>

class Bar 
{
public: 
	Bar(float transitionSpeed) : transitionSpeed(transitionSpeed) 
	{
		shader.loadFromFile(Config::barShaderPath, sf::Shader::Fragment);
	};

	void update(const float& dt) 
	{
		*value += transitionSpeed * dt * (targetValue - *value);
		
		shader.setUniform("value", *value/targetValue);
		text.setString(std::to_string(*value));
	};
	
	void render(sf::RenderWindow& window) 
	{
		shader.setUniform("windowHeight", window.getView().getSize().y); //#TODO check if view or default view
		window.draw(sprite, &shader);
		//fix text pos according to number of digits
		text.setPosition(sprite.getPosition().x + relativeTextCenteredPos.x - static_cast<int>(text.getGlobalBounds().width * 0.5f),  //cast to int fix the text position to an int value preventing antialiasing blur effect
						sprite.getPosition().y + relativeTextCenteredPos.y - static_cast<int>(text.getGlobalBounds().height * 0.5f));
		window.draw(text);
	};

	void setTexture(const std::string& path) 
	{
		texture.loadFromFile(path);
		texture.generateMipmap();
		shader.setUniform("texture", texture);
		sprite.setTexture(texture);
	};

	void setPos(const sf::Vector2<int>& pos) { sprite.setPosition(pos.x,pos.y); };
	void setScale(float x, float y) { sprite.setScale(x,y); };
	void setColorMask(const sf::Color& color) {	shader.setUniform("colorMask", sf::Glsl::Vec3(color.r/256, color.g/256, color.b/256)); };
	
	void setMask(const std::string& path) 
	{	
		mask.loadFromFile(path); 
		shader.setUniform("textureMask", mask);
	};
	
	void setFont(const std::string& path) 
	{
		font.loadFromFile(path);
		text.setFont(font);
	};

	void setCharacterSize(uint32_t size) { text.setCharacterSize(size); };
	void setTextPos(const sf::Vector2<int>& pos) { relativeTextCenteredPos = pos; };
	
	void setChargingLine(const sf::Vector2<int>& start, const sf::Vector2<int>& end)
	{
		chargingLineStart = start;
		chargingLineEnd = end;
		shader.setUniform("start", sf::Glsl::Vec2(static_cast<sf::Vector2<int>>(sprite.getPosition()) + chargingLineStart));
		shader.setUniform("end", sf::Glsl::Vec2(static_cast<sf::Vector2<int>>(sprite.getPosition()) + chargingLineEnd));
	};
	
	void setValue(float* value)	{ this->value = value; };
	void setTargetValue(float targetValue) { this->targetValue = targetValue; };

private:
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Texture mask;
	sf::Shader shader;
	sf::Vector2<int> chargingLineStart;
	sf::Vector2<int> chargingLineEnd;
	
	sf::Font font;
	sf::Text text;
	sf::Vector2<int> relativeTextCenteredPos;

	float* value;
	float targetValue;
	float transitionSpeed;
};