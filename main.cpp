#include <iostream>
#include <SFML/Graphics.hpp>
#include "RNG.h"
#include <vector>

using namespace std;
using namespace sf;

//Initializes empty 10x20 grid
void initGrid(vector<vector<int>>& grid)
{
    vector<int> row = {1,0,0,0,0,0,0,0,0,1};

    for (int i = 0; i < 20; i++)
    {
        grid.push_back(row);
    }
}

//Helper function to help print current grid for debugging purposes
void printGrid(vector<vector<int>> &grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

//Function that takes in the grid of pieces and draws the squares that are present at each location
void drawGrid(RenderWindow& window, vector<vector<int>>& grid, Texture& gameArt)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            Sprite square;
            square.setTexture(gameArt);
            square.setTextureRect(IntRect(240, 688, 7, 7));
            float scaleFactor = 5;
            square.setPosition(j*7*scaleFactor + 225, i*7*scaleFactor + 50);
            square.setScale(scaleFactor,scaleFactor);
            if (grid[i][j] != 0)
            {
                window.draw(square);
            }
        }
    }
}

int main()
{
    RNG rng;
    vector<vector<int>> grid;
    initGrid(grid);

    RenderWindow window(VideoMode(800, 750), "Tetris");
    Texture gameArt;
    gameArt.loadFromFile("sprites.png");

    Sprite TPiece;
    TPiece.setTexture(gameArt);
    TPiece.setTextureRect(sf::IntRect(202, 688, 32, 32));
    TPiece.setScale(5,5);
    TPiece.setPosition(400, 0);

    bool xPressed = false;
    bool zPressed = false;

    RectangleShape gameBackground;
    gameBackground.setSize(Vector2f(350, 700));
    gameBackground.setFillColor(Color::Black);
    gameBackground.setPosition(225, 50);

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
        window.clear(Color(0,0,255));

        
        window.draw(gameBackground);
        drawGrid(window, grid, gameArt);
        window.draw(TPiece);
        window.display();
    }

    

    return 0;
}


