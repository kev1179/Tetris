#include <iostream>
#include <SFML/Graphics.hpp>
#include "RNG.h"
#include <vector>

using namespace std;
using namespace sf;

int main()
{
    RNG rng;
    vector<vector<bool>> grid;
    RenderWindow window(VideoMode(800, 800), "Tetris");
    Texture gameArt;
    gameArt.loadFromFile("sprites.png");

    Sprite TPiece;
    TPiece.setTexture(gameArt);
    TPiece.setTextureRect(sf::IntRect(202, 688, 32, 32));
    TPiece.setScale(5,5);
    TPiece.setPosition(400, 0);

    bool xPressed = false;
    bool zPressed = false;

    Clock clock;
    while (window.isOpen())
    {
        Time time1 = clock.getElapsedTime();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (time1.asMilliseconds()  >= 80 && time1.asMilliseconds() < 1000)
        {
            TPiece.setPosition(TPiece.getPosition().x, TPiece.getPosition().y + 9);
            clock.restart();
        }
        

        
        //*********************** USER INPUT *************************************88
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !xPressed)
        {

            TPiece.setRotation(TPiece.getRotation() + 90);
            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);

        }
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !zPressed)
        {

            TPiece.setRotation(TPiece.getRotation() - 90);
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);

        }
        xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);

        //*********************************** DRAWING *************************************************************
        window.clear();

        window.draw(TPiece);

        window.display();
    }

    

    return 0;
}


