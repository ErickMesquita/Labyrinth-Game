#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "grid.h"
#include "player.hpp"
#include "textureHolder.hpp"

#include <iostream>
#include <sstream>
#include <string>
using namespace sf;


//Text
Text menuTitle;
Text option1;
Text option2;
Text option3;
Text currentLevel;


void positionText(Vector2f resolution);

void toPAUSED(){
    menuTitle.setString("PAUSE");
    option1.setString("Resume");
    option2.setString("Main Menu");
    option3.setString("Quit");
}

void toLEVELING_UP(int level, Vector2i arena){
    menuTitle.setString("LEVEL " + std::to_string(level+1));
    option1.setString("Speed Bonus +20%");
    option2.setString("Arena size: " + std::to_string(arena.x) + " x " + std::to_string(arena.y));
    option3.setString("press enter to continue");
}

void toSTART_MENU(){
    menuTitle.setString("LABYRINTH");
    option1.setString("Play");
    option2.setString("Options");
    option3.setString("Quit");
}

void toOPTIONS(){
    menuTitle.setString("OPTIONS");
    option1.setString("N");
    option2.setString("A");
    option3.setString("O");
}


enum class Option { OPTION1, OPTION2, OPTION3};

void changeOption(Option &option) {

    // Handle the pressing and releasing of the WS keys
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
        switch(option) {
        case Option::OPTION1:
            option = Option::OPTION3;
            option1.setFillColor(Color::Black);
            option3.setFillColor(Color::Yellow);
            break;
        case Option::OPTION2:
            option = Option::OPTION1;
            option2.setFillColor(Color::Black);
            option1.setFillColor(Color::Yellow);
            break;
        case Option::OPTION3:
            option = Option::OPTION2;
            option3.setFillColor(Color::Black);
            option2.setFillColor(Color::Yellow);
            break;
        default:
            option = Option::OPTION3;
            option1.setFillColor(Color::Black);
            option2.setFillColor(Color::Black);
            option3.setFillColor(Color::Yellow);
            break;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
        switch(option) {
        case Option::OPTION1:
            option = Option::OPTION2;
            option1.setFillColor(Color::Black);
            option2.setFillColor(Color::Yellow);
            break;
        case Option::OPTION2:
            option = Option::OPTION3;
            option2.setFillColor(Color::Black);
            option3.setFillColor(Color::Yellow);
            break;
        case Option::OPTION3:
            option = Option::OPTION1;
            option3.setFillColor(Color::Black);
            option1.setFillColor(Color::Yellow);
            break;
        default:
            option = Option::OPTION2;
            option1.setFillColor(Color::Black);
            option2.setFillColor(Color::Yellow);
            option3.setFillColor(Color::Black);
            break;

        }
    }
}


int main(){

    // Get the screen resolution and create an SFML window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Labyrinth", Style::Fullscreen);

    // Create a SFML View for the main action
    View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // Create a SFML View for the HUD
    View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // The game will always be in one of the following states:
    enum class State { PAUSED, LEVELING_UP, PLAYING, START_MENU };

    // Start with the OPTION1 Option
    Option option = Option::OPTION1;

    int level = 1;
    int scale = 100;

    // Here is our clock for timing everything
    Clock clock;

    // How long has the PLAYING state been active
    Time gameTimeTotal;

    // Where is the mouse in relation to world coordinates
    Vector2f mouseWorldPosition;

    // Where is the mouse in relation to screen coordinates
    Vector2i mouseScreenPosition;

    // Create and hold our map on the heap
    srand((int)time(0) * 10);

    // Create first Labyrinth
    grid* g = new grid(resolution.x, resolution.y, scale);


    // Create an instance of the Player class
    Player* player = new Player();

    //Load font file
    Font font;
    font.loadFromFile("Resources/fonts/KOMIKAP_.ttf");

    Text startText;
    Text finish;

    { // Text Stuff
    //Set font for messages
    menuTitle.setFont(font);
    option1.setFont(font);
    option2.setFont(font);
    option3.setFont(font);
    startText.setFont(font);
    finish.setFont(font);

    //Make it big
    menuTitle.setCharacterSize(resolution.y/8);
    option1.setCharacterSize(resolution.y/13);
    option2.setCharacterSize(resolution.y/13);
    option3.setCharacterSize(resolution.y/13);
    startText.setCharacterSize(scale/4);
    finish.setCharacterSize(scale/4);

    //Choose Color
    menuTitle.setFillColor(Color::Black);
    option1.setFillColor(Color::Yellow);
    option2.setFillColor(Color::Black);
    option3.setFillColor(Color::Black);
    startText.setFillColor(Color::Black);
    finish.setFillColor(Color::Black);

    startText.setString("start");
    finish.setString("finish");

    // Position
    FloatRect textRect = startText.getLocalBounds();
    startText.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    startText.setPosition(scale/2, scale/2);

    textRect = finish.getLocalBounds();
    finish.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    finish.setPosition(g->getHW().x * scale - scale/2, g->getHW().y * scale - scale/2);

    // Current Level
    currentLevel.setFont(font);
    currentLevel.setCharacterSize(resolution.y/13);
    currentLevel.setFillColor(Color::Black);
    currentLevel.setString("LEVEL "+std::to_string(level));
    currentLevel.setOrigin(Vector2f(currentLevel.getGlobalBounds().width/2, currentLevel.getGlobalBounds().height/2));
    currentLevel.setPosition(resolution.x/9, resolution.y/26);
    }

    // Start with the START_MENU state
    State state = State::START_MENU;
    toSTART_MENU();
    positionText(resolution);



    // Prepare the sounds
    SoundBuffer cheerBuffer;
    cheerBuffer.loadFromFile("Resources/sound/cheer.wav");
    Sound cheer;
    cheer.setBuffer(cheerBuffer);

    while (window.isOpen()){

        /**************************************************************
        **               Handle Player's Input                       **
        **************************************************************/

        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                switch (state){
                case State::PLAYING:

                    // Pause game while playing
                    if (event.key.code == Keyboard::Return) {
                        state = State::PAUSED;
                        toPAUSED();
                        positionText(resolution);
                    }
                    break;

                case State::PAUSED:
                    // Restart while paused
                    if (event.key.code == Keyboard::Return) {
                        switch(option){
                        case Option::OPTION1: //Resume
                            state = State::PLAYING;
                            // Reset the clock so there isn't a frame jump
                            clock.restart();
                            break;
                        case Option::OPTION2: //Main Menu
                            state = State::START_MENU;
                            toSTART_MENU();
                            positionText(resolution);
                            break;
                        case Option::OPTION3: //Quit
                            window.close();
                            break;
                        default:
                            break;

                        }
                    }

                    // Handle WS keys to change selected option
                    changeOption(option);

                    break;

                case State::START_MENU:

                    // Start a new game while in START_MENU state
                    if (event.key.code == Keyboard::Return) {
                        switch(option){
                        case Option::OPTION1: //Play
                            player->spawn(scale, resolution);
                            state = State::PLAYING;
                            // Reset the clock so there isn't a frame jump
                            clock.restart();
                            break;
                        case Option::OPTION2: //Options
                            //state = State::OPTIONS;
                            //toOPTIONS();
                            //positionText(resolution);
                            break;
                        case Option::OPTION3: //Quit
                            window.close();
                            break;
                        default:
                            break;

                        }

                    }

                    // Handle WS keys to change selected option
                    changeOption(option);

                    break;

                case State::LEVELING_UP:
                    //if you just won
                    if(player->m_Win){

                        //Respawn
                        player->spawn(scale, resolution);

                        // Generate new map, 40% wider and 40% higher
                        delete g;
                        g = new grid(resolution.x * (1 + 0.4 * level), resolution.y * (1 + 0.4 * level), scale);

                        // "Finish" text needs to change position
                        finish.setPosition(g->getHW().x * scale - scale/2, g->getHW().y * scale - scale/2);


                        // New level!
                        level++;
                        currentLevel.setString("LEVEL "+std::to_string(level));

                    }

                    if (event.key.code == Keyboard::Return) {

                        player->upgradeSpeed();

                        state = State::PLAYING;

                        // Reset the clock so there isn't a frame jump
                        clock.restart();
                    }


                    break;
                default:
                    break;

                } //End switch(state)
            } //End if (event.type == Event::KeyPressed)
        }// End event polling


        // Handle the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // Handle controls while playing
        if (state == State::PLAYING) {

            // Handle the pressing and releasing of the WASD keys

            player->m_Speed.y = 0;
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                player->m_Speed.y -= 1;
            }

            if (Keyboard::isKeyPressed(Keyboard::S)/* and g->checkColision(player->getPosition())->down*/) {
                player->m_Speed.y += 1;
            }

            player->m_Speed.x = 0;
            if (Keyboard::isKeyPressed(Keyboard::A)/* and g->checkColision(player->getPosition())->left*/) {
                player->m_Speed.x -= 1;
            }

            if (Keyboard::isKeyPressed(Keyboard::D)/* and g->checkColision(player->getPosition())->right*/) {
                player->m_Speed.x += 1;
            }

        }// End WASD while playing




        /**************************************************************
        **                       Update Scene                        **
        **************************************************************/
        //Measure Time
        Time dt = clock.getElapsedTime();
        clock.restart();

        switch (state){
            case State::PLAYING:

                mouseScreenPosition = Mouse().getPosition();
                player->update(dt.asSeconds(), Vector2i(window.mapPixelToCoords(mouseScreenPosition)), g);

                // Make the view center around the player
                mainView.setCenter(player->getCenter());

                if (player->m_Win){
                    state = State::LEVELING_UP;
                    toLEVELING_UP(level, Vector2i(g->getHW().x*(1 + 0.4*level), g->getHW().y * (1 + 0.4*level)));
                    cheer.play();
                    positionText(resolution);
                }

                break;
            default:
                break;
        }


        /**************************************************************
        **               Draw Everything onto Screen                 **
        **************************************************************/
        window.clear();
        sf::RectangleShape rectangle;

        switch (state) {
        case State::PLAYING:

            window.setView(mainView);

            //Labyrinth
            g->draw(window);

            //Texts
            window.draw(startText);
            window.draw(finish);

            //Player
            window.draw(player->getSprite());

            //Draw HUD
            window.setView(hudView);

            rectangle.setSize(Vector2f(currentLevel.getGlobalBounds().width*1.3, currentLevel.getGlobalBounds().height*2));
            rectangle.setOrigin(rectangle.getSize()/float(2.));
            rectangle.setFillColor(Color::Red);
            rectangle.setPosition(currentLevel.getPosition());
            window.draw(rectangle);
            window.draw(currentLevel);


            break;

        case State::PAUSED:

            window.setView(hudView);

            //Red Background (temporary)
            rectangle.setSize(resolution); //full screen
            rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
            window.draw(menuTitle);
            window.draw(option1);
            window.draw(option2);
            window.draw(option3);
            break;

        case State::START_MENU:

            window.setView(hudView);

            //Red Background (temporary)
            rectangle.setSize(resolution); //full screen
            rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
            window.draw(menuTitle);
            window.draw(option1);
            window.draw(option2);
            window.draw(option3);

            break;

        case State::LEVELING_UP:

            window.setView(hudView);

            //Red Background (temporary)
            rectangle.setSize(resolution); //full screen
            rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
            window.draw(menuTitle);
            window.draw(option1);
            window.draw(option2);
            window.draw(option3);

            break;

        default:
            break;

        } //End switch(state)
        window.display();
    }

    return 0;
}


void positionText(Vector2f resolution){//Position text

    FloatRect textRect = menuTitle.getLocalBounds();
    menuTitle.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    menuTitle.setPosition(resolution.x/2, 0.3*resolution.y);
    //std::cout << "menuTitle: (" << resolution.x/2 << ", " << 0.3*resolution.y << ")" << std::endl;

    textRect = option1.getLocalBounds();
    option1.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    option1.setPosition(menuTitle.getPosition().x, menuTitle.getPosition().y + 2*textRect.height);
    //std::cout << "option1: (" << menuTitle.getPosition().x << ", " << menuTitle.getPosition().y + 2*textRect.height << ")" << std::endl;

    textRect = option2.getLocalBounds();
    option2.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    option2.setPosition(menuTitle.getPosition().x, menuTitle.getPosition().y + 3.5*textRect.height);

    textRect = option3.getLocalBounds();
    option3.setOrigin(
        textRect.left + textRect.width/2,
        textRect.top + textRect.height/2);
    option3.setPosition(menuTitle.getPosition().x, menuTitle.getPosition().y + 5*textRect.height);

}
