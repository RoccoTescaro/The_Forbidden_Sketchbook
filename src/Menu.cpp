#include "../hdr/Menu.h"
#include "../hdr/Config.h"
#include "../hdr/Game.h"

//-----------------------0.2+
//       TITLE           2.0+
//-----------------------0.2+
//-----------------------0.1+
//        start          0.8+
//-----------------------0.1+
//-----------------------0.1+
//        load           0.8+
//-----------------------0.1+
//-----------------------0.1+
//       editor          0.8+
//-----------------------0.1+
//-----------------------0.1+ 
//        exit           0.8+
//-----------------------0.1+
//-----------------------0.1+= 6.5

#define UNIT 1/6.5f

Menu::Menu() :
    start({ 0.f, window.getSize().y * 2.4f * UNIT }, sf::Vector2<int>(window.getSize().x, window.getSize().y * UNIT)),
    load({ 0.f, window.getSize().y * 3.4f * UNIT }, sf::Vector2<int>(window.getSize().x, window.getSize().y * UNIT)),
    editor({ 0.f, window.getSize().y * 4.4f * UNIT }, sf::Vector2<int>(window.getSize().x, window.getSize().y * UNIT)),
    exit({ 0.f, window.getSize().y * 5.4f * UNIT }, sf::Vector2<int>( window.getSize().x, window.getSize().y * UNIT))
{
    //BACKGROUND
    backgroundTexture.loadFromFile(Config::menuBackgroundTexturePath);
    background.setTexture(backgroundTexture);
    float backgroundScale = (float) window.getSize().x / backgroundTexture.getSize().x;
    background.setScale(backgroundScale, backgroundScale);

    //TITLE
    titleFont.loadFromFile(Config::titleFontPath);
    title.setFont(titleFont);
    title.setFillColor(sf::Color(0, 0, 0, 255));
    title.setString("The Forbidden Sketchbook");
    title.setCharacterSize(int(window.getSize().y * 0.8 * UNIT));
    //center allignment
    sf::FloatRect titleBound = title.getGlobalBounds(); 
    title.setPosition(window.getSize().x * 0.5f - titleBound.width * 0.5f, window.getSize().y * 1.2 * UNIT - titleBound.height * 0.5f);

    //BUTTONS
    auto onClick = [this]() 
    {
        transitionEffect.start();
        start.setActive(false); //not allow to press any other button if scene is changing
        load.setActive(false);
        editor.setActive(false);
        exit.setActive(false);
    };

    start.setFont(Config::buttonFontPath);
    start.setText(" StaRt ", window.getSize().y * 0.8 * UNIT);
    start.getText().setFillColor(sf::Color(0, 0, 0, 255));
    start.setOnClick(onClick);
    start.setOnMouseOver([this]() { start.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    load.setFont(Config::buttonFontPath);
    load.setText(" LoaD ", window.getSize().y * 0.8 * UNIT);
    load.getText().setFillColor(sf::Color(0, 0, 0, 255));
    load.setOnClick(onClick);
    load.setOnMouseOver([this]() { load.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    editor.setFont(Config::buttonFontPath);
    editor.setText(" EdiToR ", window.getSize().y * 0.8 * UNIT);
    editor.getText().setFillColor(sf::Color(0, 0, 0, 255));
    editor.setOnClick(onClick);
    editor.setOnMouseOver([this]() { editor.getText().setFillColor(sf::Color(255, 255, 255, 255)); });

    exit.setFont(Config::buttonFontPath);
    exit.setText(" ExiT ", window.getSize().y * 0.8 * UNIT);
    exit.getText().setFillColor(sf::Color(0, 0, 0, 255));
    exit.setOnClick([this]() { window.close(); });
    exit.setOnMouseOver([this]() { exit.getText().setFillColor(sf::Color(255, 255, 255, 255)); });
}

void Menu::update() 
{
    transitionEffect.update(dt);

    start.getText().setFillColor(sf::Color(0, 0, 0, 255));
    load.getText().setFillColor(sf::Color(0, 0, 0, 255));
    editor.getText().setFillColor(sf::Color(0, 0, 0, 255));
    exit.getText().setFillColor(sf::Color(0, 0, 0, 255));

    start.update(input);
    load.update(input);
    editor.update(input);
    exit.update(input);

    //switch state
    if (transitionEffect.isEnded()) 
    {
        //reset transition animation and change application state so that if this state will 
        //be reused it wont be completely black
        start.setActive(true); 
        load.setActive(true); 
        editor.setActive(true); 
        exit.setActive(true);
        
        bool startClicked = start.isClicked();
        bool loadClicked = load.isClicked();
        bool editorClicked = editor.isClicked();

        start.setClicked(false);
        load.setClicked(false);
        editor.setClicked(false);

        if (startClicked)  Application::setState(1);
        else if (loadClicked)
        {
            Game* game = dynamic_cast<Game*>(Application::getState(1));
            ASS(!game);
            game->load();
            Application::setState(1);
        }
        else if (editorClicked) Application::setState(2);
        else Application::getWindow().close();
    }
}

void Menu::render() 
{
    window.draw(background);
    window.draw(title);
    start.render(window);
    load.render(window);
    editor.render(window);
    exit.render(window);
    transitionEffect.render(window);
}
