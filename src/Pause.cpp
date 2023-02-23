#include "../hdr/Pause.h"
#include "../hdr/Config.h"
#include "../hdr/Game.h"

Pause::Pause() :
    save({ 0.f, window.getSize().y * 0.15f }, sf::Vector2<int>(window.getSize().x, window.getSize().y *0.14f)),
    back({ 0.f, window.getSize().y * 0.43f }, sf::Vector2<int>(window.getSize().x, window.getSize().y * 0.14f)),
    menu({ 0.f, window.getSize().y * 0.71f }, sf::Vector2<int>(window.getSize().x, window.getSize().y * 0.14f))
{
    //BACKGROUND
    backgroundTexture.loadFromFile(Config::menuBackgroundTexturePath);
    background.setTexture(backgroundTexture);
    float backgroundScale = (float)window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //BUTTONS
    save.setFont(Config::buttonFontPath);
    save.setText(" SavE ", window.getSize().y * 0.125f);
    save.getText().setFillColor(sf::Color(0, 0, 0, 255));
    save.setOnClick([this]()
        {
            Game* game = dynamic_cast<Game*>(Application::getState(1));
            WARNING(!game, "wrong state index!");
            game->save();
        });
    save.setOnMouseOver([this]() { save.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    back.setFont(Config::buttonFontPath);
    back.setText(" BaCk ", window.getSize().y * 0.125f);
    back.getText().setFillColor(sf::Color(0, 0, 0, 255));
    back.setOnClick([this]()
        {
            transitionEffect.start();
            save.setActive(false); //not allow to press any other button if scene is changing
            back.setActive(false);
            menu.setActive(false);

            Game* game = dynamic_cast<Game*>(Application::getState(1)); //TODO make it flexible with editor
            ASSERT(!game);
            game->load();
        });
    back.setOnMouseOver([this]() { back.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    menu.setFont(Config::buttonFontPath);
    menu.setText(" MeNu ", window.getSize().y * 0.125f);
    menu.getText().setFillColor(sf::Color(0, 0, 0, 255));
    menu.setOnClick([this]()
        {
            transitionEffect.start();
            save.setActive(false); //not allow to press any other button if scene is changing
            back.setActive(false);
            menu.setActive(false);
        });
    menu.setOnMouseOver([this]() { menu.getText().setFillColor(sf::Color(255, 255, 255, 255)); });
}

void Pause::update()
{
    transitionEffect.update(dt);

    save.getText().setFillColor(sf::Color(0, 0, 0, 255));
    back.getText().setFillColor(sf::Color(0, 0, 0, 255));
    menu.getText().setFillColor(sf::Color(0, 0, 0, 255));

    save.update(input);
    back.update(input);
    menu.update(input);

    //switch state
    if (transitionEffect.isEnded())
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        save.setActive(true);
        back.setActive(true);
        menu.setActive(true);
        
        if (back.isClicked()) //TODO make it flexible with editor
            Application::prevState(); 
        else if (menu.isClicked())
            Application::setState(0);
    }
}

void Pause::render()
{
    window.draw(background);
    save.render(window);
    back.render(window);
    menu.render(window);
    transitionEffect.render(window);
}
