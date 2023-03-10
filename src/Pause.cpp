#include "../hdr/Pause.h"
#include "../hdr/Config.h"
#include "../hdr/Game.h"
#include "../hdr/Editor.h"

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
            Game* game = dynamic_cast<Game*>(Application::getPrevState());
            Editor* editor = dynamic_cast<Editor*>(Application::getPrevState());
            ASSERT(!(game || editor));
            
            if (game) game->save();
            else if (editor) editor->save();
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

            Game* game = dynamic_cast<Game*>(Application::getPrevState());
            Editor* editor = dynamic_cast<Editor*>(Application::getPrevState());
            ASSERT(!(game || editor));

            if (game)
            {   
                game->save();
                game->load();
            }
            else if (editor)
            {
                editor->save();
                editor->load();
            }
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
        
        bool backClicked = back.isClicked();

        back.setClicked(false);
        menu.setClicked(false);

        if (backClicked)
            Application::prevState(); 
        else //menuClicked
            Application::setState(Application::Index::MENU);
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
