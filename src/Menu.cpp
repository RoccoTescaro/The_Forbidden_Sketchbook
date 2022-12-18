#include "../hdr/Menu.h"
#include "../hdr/Config.h"

Menu::Menu() : 
    start({ 0.f, window.getSize().y * 0.5f }, sf::Vector2<int>( window.getSize().x, window.getSize().y * 0.125f )),
    exit({ 0.f, window.getSize().y * 0.75f }, sf::Vector2<int>( window.getSize().x, window.getSize().y * 0.125f ))
{
    //BACKGROUND
    backgroundTexture.loadFromFile(Config::backgroundTexturePath);
    background.setTexture(backgroundTexture);
    float backgroundScale = (float) window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //TITLE
    titleFont.loadFromFile(Config::titleFontPath);
    title.setFont(titleFont);
    title.setFillColor(sf::Color(0, 0, 0, 255));
    title.setString("The Forbidden Sketchbook");
    title.setCharacterSize(int(window.getSize().y * 0.125));
    //center allignment
    sf::FloatRect titleBound = title.getGlobalBounds(); 
    title.setPosition(window.getSize().x * 0.5f - titleBound.width * 0.5f, window.getSize().y * 0.25f - titleBound.height * 0.5f);

    //BUTTONS
    buttonFont.loadFromFile(Config::buttonFontPath);
    //start
    sf::Text startText;
    startText.setFont(buttonFont);
    startText.setFillColor(sf::Color(0, 0, 0, 255));
    startText.setString(" StaRt ");
    startText.setCharacterSize(window.getSize().y * 0.125f);
    //center allignment
    sf::FloatRect startBound = startText.getGlobalBounds();
    startText.setPosition({ (window.getSize().x - startBound.width) * 0.5f,(window.getSize().y - startBound.height) * 0.5f });
    start.setText(startText);
    //exit
    sf::Text exitText;
    exitText.setFont(buttonFont);
    exitText.setFillColor(sf::Color(0, 0, 0, 255));
    exitText.setString(" ExiT ");
    exitText.setCharacterSize(window.getSize().y * 0.125f);
    //center allignment
    sf::FloatRect exitBound = exitText.getGlobalBounds();
    exitText.setPosition({ (window.getSize().x - exitBound.width) * 0.5f,(window.getSize().y - exitBound.height) * 0.75f });
    exit.setText(exitText);
}

void Menu::update() 
{
    transitionEffect.update();

    start.getText().setFillColor(sf::Color(0, 0, 0, 255));
    exit.getText().setFillColor(sf::Color(0, 0, 0, 255));

    if (start.isMouseOver(input.getMousePos()))
    {
        start.getText().setFillColor(sf::Color(255, 255, 255, 255));
        if (input.isKeyReleased(Input::Key::MouseL)) 
        {
            transitionEffect.startAnimation();
            start.setActive(false); //not allow to press any other button if scene is changing
            exit.setActive(false);
        }
    }
    else if (exit.isMouseOver(input.getMousePos())) 
    {
        exit.getText().setFillColor(sf::Color(255, 255, 255, 255));
        if (input.isKeyReleased(Input::Key::MouseL)) 
            window.close();
    }

    //switch state
    if (transitionEffect.isAnimationEnded()) 
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        start.setActive(true); 
        exit.setActive(true);
        Application::nextState(); //#TODO check which button has been pressed and change state according to that
    }
}

void Menu::render() 
{
    window.draw(background);
    window.draw(title);
    start.render();
    exit.render();
    transitionEffect.render();
}