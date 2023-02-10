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
    start.setFont(Config::buttonFontPath);
    start.setText(" StaRt ", window.getSize().y * 0.125f);
    start.getText().setFillColor(sf::Color(0, 0, 0, 255));
    start.setOnClick([this]() 
        { 
            transitionEffect.startAnimation();
            start.setActive(false); //not allow to press any other button if scene is changing
            exit.setActive(false); 
        });
    start.setOnMouseOver([this]() { start.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    exit.setFont(Config::buttonFontPath);
    exit.setText(" ExiT ", window.getSize().y * 0.125f);
    exit.getText().setFillColor(sf::Color(0, 0, 0, 255));
    exit.setOnClick([this]() { window.close(); });
    exit.setOnMouseOver([this]() { exit.getText().setFillColor(sf::Color(255, 255, 255, 255)); });
}

void Menu::update() 
{
    State::update();
    transitionEffect.update();

    start.getText().setFillColor(sf::Color(0, 0, 0, 255));
    exit.getText().setFillColor(sf::Color(0, 0, 0, 255));

    start.update();
    exit.update();

    //switch state
    if (transitionEffect.isAnimationEnded()) 
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        start.setActive(true); 
        exit.setActive(true);
        Application::nextState(); //TODO check which button has been pressed and change state according to that
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

void Menu::onResize() 
{
    State::onResize();

    //BACKGROUND
    float backgroundScale = (float)window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //TITLE
    title.setCharacterSize(int(window.getSize().y * 0.125));
    //center allignment
    sf::FloatRect titleBound = title.getGlobalBounds();
    title.setPosition(window.getSize().x * 0.5f - titleBound.width * 0.5f, window.getSize().y * 0.25f - titleBound.height * 0.5f);

    //BUTTONS
    start.setText(" StaRt ", window.getSize().y * 0.125f);
    exit.setText(" ExiT ", window.getSize().y * 0.125f);
}