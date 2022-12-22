#include "../hdr/Button.h"

void Button::setTextAlignment(TextAlignment hAlignment, TextAlignment vAlignment)
{
    float xoffset;
    switch (hAlignment)
    {
    case LEFT: xoffset = 0; break;
    case RIGHT: xoffset = dim.x - this->text.getLocalBounds().width; break;
    case CENTER:
    default:
        xoffset = (dim.x - this->text.getLocalBounds().width) * 0.5f;
        break;
    }

    float yoffset;
    switch (vAlignment)
    {
    case TOP: yoffset = 0; break;
    case BOTTOM: yoffset = dim.y - this->text.getLocalBounds().height; break;
    case CENTER:
    default:
        yoffset = (dim.y - this->text.getLocalBounds().height) * 0.5f;
        break;
    }

    this->text.setPosition(pos.x + xoffset, pos.y + yoffset);
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

bool Button::isMouseOver() const 
{
    sf::Rect<float> bounds{ pos, sf::Vector2<float>(dim) };
    return bounds.contains(Application::getInput().getMousePos());
}

void Button::update() 
{
    if (!active) return;

    sprite.setColor(sf::Color::Black);

    if (isMouseOver()) 
    {
        sprite.setColor(sf::Color::White);

        if (onMouseOver) 
            onMouseOver();
        if (Application::getInput().isKeyReleased(Input::Key::MouseL) && onClick)
            onClick();
    }
}

void Button::render() 
{
    Application::getWindow().draw(sprite);
    Application::getWindow().draw(text);
}
