#include "../hdr/Button.h"

void Button::setTextAlignment(TextAlignment hAlignment, TextAlignment vAlignment)
{
    sf::Rect<float> textBounds = text.getLocalBounds();
    float xOffset, yOffset;

    switch (hAlignment) 
    {
    case LEFT: xOffset = 0; break;
    case RIGHT: xOffset = dim.x - textBounds.width; break;
    case CENTER:
    default:
        xOffset = (dim.x - textBounds.width) * 0.5f;
        break;
    }

    switch (vAlignment) 
    {
    case TOP: yOffset = 0; break;
    case BOTTOM: yOffset = dim.y - textBounds.height; break;
    case CENTER:
    default:
        yOffset = (dim.y - textBounds.height) * 0.5f;
        break;
    }

    text.setPosition(pos.x + xOffset - text.getGlobalBounds().left, pos.y + yOffset - text.getGlobalBounds().top);
}

void Button::setText(const std::string& text, int charSize, TextAlignment hAlignment, TextAlignment vAlignment)
{
    this->text.setFillColor(sf::Color::Black);
    this->text.setString(text);
    this->text.setCharacterSize(charSize);
    setTextAlignment(hAlignment,vAlignment);
}

void Button::setText(const std::string& text, int charSize, TextAlignment alignment)
{
    this->text.setFillColor(sf::Color::Black);
    this->text.setString(text);
    this->text.setCharacterSize(charSize);
    setTextAlignment(alignment);
}

void Button::setSprite(sf::Texture& texture) 
{
    this->texture = texture; 
    sprite.setPosition(pos); 
    sprite.setTexture(texture); 
    sprite.setScale(texture.getSize().x / dim.x, texture.getSize().y / dim.y); 
}

bool Button::isMouseOver(const Input& input) const 
{
    sf::Rect<float> bounds{ pos, sf::Vector2<float>(dim) };
    return bounds.contains(input.getMousePos());
}

void Button::update(const Input& input)
{
    if (!active) return;

    sprite.setColor(sf::Color::Black);

    if (isMouseOver(input))
    {
        sprite.setColor(sf::Color::White);

        if (onMouseOver)
            onMouseOver();

        if (input.isKeyReleased(Input::Key::MouseL))
        {
            setClicked(true);
            if (onClick) onClick();
        }
    }
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(text);
}
