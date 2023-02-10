#include "../hdr/Pause.h"
#include "../hdr/Config.h"

Pause::Pause() :
    save({ 0.f, window.getSize().y * 0.15f }, sf::Vector2<int>(window.getSize().x, window.getSize().y *0.14f)),
    back({ 0.f, window.getSize().y * 0.43f }, sf::Vector2<int>(window.getSize().x, window.getSize().y * 0.14f)),
    menu({ 0.f, window.getSize().y * 0.71f }, sf::Vector2<int>(window.getSize().x, window.getSize().y * 0.14f))
{
    //BACKGROUND
    backgroundTexture.loadFromFile(Config::backgroundTexturePath);
    background.setTexture(backgroundTexture);
    float backgroundScale = (float)window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //BUTTONS
    save.setFont(Config::buttonFontPath);
    save.setText(" SavE ", window.getSize().y * 0.125f);
    save.getText().setFillColor(sf::Color(0, 0, 0, 255));

    save.setOnMouseOver([this]() { save.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    back.setFont(Config::buttonFontPath);
    back.setText(" BaCk ", window.getSize().y * 0.125f);
    back.getText().setFillColor(sf::Color(0, 0, 0, 255));
    back.setOnClick([this]()
        {
            transitionEffect.startAnimation();
            save.setActive(false); //not allow to press any other button if scene is changing
            back.setActive(false);
            menu.setActive(false);
        });
    back.setOnMouseOver([this]() { back.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    menu.setFont(Config::buttonFontPath);
    menu.setText(" MeNu ", window.getSize().y * 0.125f);
    menu.getText().setFillColor(sf::Color(0, 0, 0, 255));

    menu.setOnMouseOver([this]() { menu.getText().setFillColor(sf::Color(255, 255, 255, 255)); });
}

void Pause::update()
{
    State::update();
    transitionEffect.update();

    save.getText().setFillColor(sf::Color(0, 0, 0, 255));
    back.getText().setFillColor(sf::Color(0, 0, 0, 255));
    menu.getText().setFillColor(sf::Color(0, 0, 0, 255));

    save.update();
    back.update();
    menu.update();

    //switch state
    if (transitionEffect.isAnimationEnded())
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        save.setActive(true);
        back.setActive(true);
        menu.setActive(true);
        
        if(back.isClicked())
            Application::prevState(); //TODO check which button has been pressed and change state according to that
    }
}

void Pause::render()
{
    window.draw(background);
    save.render();
    back.render();
    menu.render();
    transitionEffect.render();
}

void Pause::onResize()
{
    State::onResize();
    
    //BACKGROUND
    float backgroundScale = (float)window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //BUTTONS
    save.setText(" SavE ", window.getSize().y * 0.125f);
    back.setText(" BaCk ", window.getSize().y * 0.125f);
    menu.setText(" MeNu ", window.getSize().y * 0.125f);
}
