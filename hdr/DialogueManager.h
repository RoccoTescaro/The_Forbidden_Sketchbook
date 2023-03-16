#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Config.h"
#include <vector>
#include <string>

class DialogueManager
{
public:
    DialogueManager() 
    {
        for (auto i = 0; i < Config::nTypingSounds; i++)
        {
            soundBuffer.emplace_back(sf::SoundBuffer{});
            soundBuffer[i].loadFromFile(Config::typingSoundPathBuffer[i]);
        }
    };

    bool isAnimationEnded() { return charIndex == dialogueBuffer.front().length() && timer > readTime; };

    void next()
    {
        if (!visible) return;
        charIndex = 0;
        timer = 0.f;
        dialogueBuffer.pop_front();
    };

    void show() { visible = true; };
    void unshow() { visible = false; };

    void update(const float& dt)
    {
        if (dialogueBuffer.empty()) visible = false;
        if (!visible) return;
        
        auto& currentText = dialogueBuffer.front();

        timer += dt;

        if (timer > typingSpeed && charIndex != currentText.length())
        {
            timer = 0.f;
            if (charIndex < currentText.size())
            {
                charIndex++;
                sound.setBuffer(soundBuffer[rand() % soundBuffer.size()]);
                sound.play();
            }
        }

        text.setString(currentText.substr(0, charIndex));

        //Fix text on width
        if (text.getGlobalBounds().width > textBox.width)
        {
            auto backIndex = text.getString().toWideString().find_last_of('\n');
            auto spaceIndex = text.getString().toWideString().find_last_of(' ');

            if (spaceIndex != std::string::npos && (backIndex == std::string::npos || backIndex < spaceIndex))
                currentText[spaceIndex] = '\n';
            else
                currentText.insert(charIndex - 1, "\n");
        }

        //Fix text on height
        if (text.getGlobalBounds().height > textBox.height)
        {
            auto backIndex = text.getString().toWideString().find_last_of('\n');

            if (backIndex != std::string::npos)
            {
                dialogueBuffer.insert(++dialogueBuffer.begin(), currentText.substr(backIndex + 1, currentText.size()));
                currentText = currentText.substr(0, backIndex);
            }
            else
            {
                dialogueBuffer.insert(++dialogueBuffer.begin(), currentText.substr(charIndex - 3, currentText.size()));
                currentText = currentText.substr(0, charIndex - 3);
            }

            currentText.append("...");
        }
    };

    void render(sf::RenderWindow& window)
    {
        if (!visible) return;
        window.draw(sprite);
        window.draw(text);
    };

    void addText(const std::string& string) { dialogueBuffer.push_back(string); };

    //TEXTURE
    void setTexture(const std::string& path) 
    { 
        texture.loadFromFile(path); 
        sprite.setTexture(texture);
    };

    void setTextureRect(const sf::Rect<int>& rect) { textBox = rect; };
    void setPos(const sf::Vector2<int>& pos) { sprite.setPosition(pos.x, pos.y); };
    void setScale(float x, float y) { sprite.setScale(x, y); };

    //TEXT
    void setFont(const std::string& path)
    {
        font.loadFromFile(path);
        text.setFont(font);
    };

    void setTextPos(const sf::Vector2<int>& pos)
    {
        text.setPosition(pos.x, pos.y);
        textBox.left = pos.x;
        textBox.top = pos.y;
    };
    
    void setTextBox(const sf::Rect<int>& rect) 
    {
        textBox = rect;
        text.setPosition(textBox.left, textBox.top);
    };

    void setTextSize(uint32_t size) { text.setCharacterSize(size); };
    void setTextColor(const sf::Color& color) { text.setFillColor(color); };

private:
    
    std::list<std::string> dialogueBuffer;
    uint32_t charIndex = 0;
    const float readTime = 3.f;
    const float typingSpeed = 0.075f;
    float timer = 0.f;
    bool visible = true;

    //TEXTURE
    sf::Texture texture;
    sf::Sprite sprite;
    
    //TEXT
    sf::Font font;
    sf::Text text;
    sf::Rect<int> textBox{0,0,0,0};

    //SOUND
    std::vector<sf::SoundBuffer> soundBuffer;
    sf::Sound sound;
};