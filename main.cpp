#include <iostream>
#include <SFML/Graphics.hpp>
#include "RNG.h"
#include <vector>
#include <unordered_map>
#include "TPiece.h"

using namespace std;
using namespace sf;

//Initializes empty 10x20 grid
void initGrid(vector<vector<int>>& grid)
{
    vector<int> row = {0,0,0,0,0,0,0,0,0,0};

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
    unordered_map<int, IntRect> squares;
    squares[1] = IntRect(240, 688, 7, 7);
    squares[2] = IntRect(240, 728, 7, 7);
    squares[3] = IntRect(240, 768, 7, 7);
    squares[4] = IntRect(240, 688, 7, 7);
    squares[5] = IntRect(240, 728, 7, 7);
    squares[6] = IntRect(240, 768, 7, 7);
    squares[7] = IntRect(240, 688, 7, 7);

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            Sprite square;
            square.setTexture(gameArt);
            square.setTextureRect(squares[grid[i][j]]);
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



    bool xPressed = false;
    bool zPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;

    RectangleShape gameBackground;
    gameBackground.setSize(Vector2f(350, 700));
    gameBackground.setFillColor(Color::Black);
    gameBackground.setPosition(225, 50);

    TPiece activePiece(gameArt);
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

        if (time1.asMilliseconds()  >= 300 && time1.asMilliseconds() < 1000 && activePiece.canMoveDown())
        {
            activePiece.fall();
            clock.restart();
        }
        

        
        //*********************** USER INPUT *************************************88
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !xPressed)
        {

            activePiece.rotate("right");
            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        }
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !zPressed)
        {

            activePiece.rotate("left");
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !leftPressed && activePiece.canMoveLeft())
        {

            activePiece.move("left");
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !rightPressed && activePiece.canMoveRight())
        {

            activePiece.move("right");
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
        }

        xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
        //*********************************** DRAWING *************************************************************
        window.clear(Color(0,0,255));

        
        window.draw(gameBackground);
        drawGrid(window, grid, gameArt);
        vector<Sprite> v = activePiece.getSprites();
        for (int i = 0; i < v.size(); i++)
        {
            window.draw(v[i]);
        }
        //window.draw(TPiece);
        window.display();
    }

    

    return 0;
}


