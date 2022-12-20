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
    buttonFont.loadFromFile(Config::buttonFontPath);
    //save
    sf::Text saveText;
    saveText.setFont(buttonFont);
    saveText.setFillColor(sf::Color(0, 0, 0, 255));
    saveText.setString(" SavE ");
    saveText.setCharacterSize(window.getSize().y * 0.125f);
    //center allignment
    sf::FloatRect saveBound = saveText.getGlobalBounds();
    saveText.setPosition({ (window.getSize().x - saveBound.width) * 0.5f,(window.getSize().y - saveBound.height) * 0.15f });
    save.setText(saveText);
    //backToGame
    sf::Text backText;
    backText.setFont(buttonFont);
    backText.setFillColor(sf::Color(0, 0, 0, 255));
    backText.setString(" BaCk");
    backText.setCharacterSize(window.getSize().y * 0.125f);
    //center allignment
    sf::FloatRect backBound = backText.getGlobalBounds();
    backText.setPosition({ (window.getSize().x - backBound.width) * 0.5f,(window.getSize().y - backBound.height) * 0.43f });
    back.setText(backText);
    //backToMenu
    sf::Text menuText;
    menuText.setFont(buttonFont);
    menuText.setFillColor(sf::Color(0, 0, 0, 255));
    menuText.setString(" MeNu ");
    menuText.setCharacterSize(window.getSize().y * 0.125f);
    //center allignment
    sf::FloatRect menuBound = menuText.getGlobalBounds();
    menuText.setPosition({ (window.getSize().x - menuBound.width) * 0.5f,(window.getSize().y - menuBound.height) * 0.71f });
    menu.setText(menuText);
}

void Pause::update()
{
    transitionEffect.update();

    save.getText().setFillColor(sf::Color(0, 0, 0, 255));
    back.getText().setFillColor(sf::Color(0, 0, 0, 255));
    menu.getText().setFillColor(sf::Color(0, 0, 0, 255));

    if (save.isMouseOver(input.getMousePos()))
    {
        save.getText().setFillColor(sf::Color(255, 255, 255, 255));
        if (input.isKeyReleased(Input::Key::MouseL))
        {
        }
    }
    else if (back.isMouseOver(input.getMousePos()))
    {
        back.getText().setFillColor(sf::Color(255, 255, 255, 255));
        if (input.isKeyReleased(Input::Key::MouseL))
        {
            transitionEffect.startAnimation();
            save.setActive(false); //not allow to press any other button if scene is changing
            back.setActive(false);
            menu.setActive(false);
        }
    }
    else if (menu.isMouseOver(input.getMousePos()))
    {
        menu.getText().setFillColor(sf::Color(255, 255, 255, 255));
        if (input.isKeyReleased(Input::Key::MouseL))
        {
        }
    }

    //switch state
    if (transitionEffect.isAnimationEnded())
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        save.setActive(true);
        back.setActive(true);
        menu.setActive(true);
        
        if(back.isClicked())
            Application::prevState(); //#TODO check which button has been pressed and change state according to that
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