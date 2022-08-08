#include <iostream>
#include <SFML/Graphics.hpp>
#include "RNG.h"
#include <vector>
#include <unordered_map>
#include "TPiece.h"
#include "Piece.h"
#include "JPiece.h"
#include "ZPiece.h"
#include "Square.h"
#include "SPiece.h"
#include <queue>

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

//Recursive function that shifts all the lines in the grid down after a line has been cleared
void downShift(vector<vector<int>>& grid, int clearedRow)
{
    if (clearedRow == -1)
    {
        return;
    }

    vector<int> temp = grid[clearedRow];
    grid[clearedRow + 1] = temp;
    downShift(grid, clearedRow - 1);

}

//Function that determines when a line needs to be cleared.
void clearLines(vector<vector<int>>& grid)
{
    vector<int> emptyRow = { 0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0; i < grid.size(); i++)
    {
        int counter = 0;

        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] != 0)
            {
                counter++;
            }
        }

        if (counter == 10)
        {
            grid[i] = emptyRow;
            downShift(grid, i-1);
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

    unordered_map<int, IntRect> previewPieces;
    previewPieces[1] = IntRect(240, 688, 23, 15);
    previewPieces[2] = IntRect(240, 728, 23, 15);
    previewPieces[3] = IntRect(240, 768, 23, 15);
    previewPieces[4] = IntRect(244, 808, 15, 15);
    previewPieces[5] = IntRect(240, 848, 23, 15);
    previewPieces[6] = IntRect(240, 888, 23, 15);
    previewPieces[7] = IntRect(248, 920, 7, 31);

    RectangleShape previewSquare;
    previewSquare.setSize(Vector2f(150, 150));
    previewSquare.setFillColor(Color::Black);
    previewSquare.setPosition(600, 300);




    Piece* activePiece = nullptr;

    int firstPiece = rng.randomInt(1, 6);
    int nextPiece = rng.randomInt(1, 6);

    if (firstPiece == 1)
    {
        TPiece temp(gameArt);
        activePiece = &temp;
    }

    else if (firstPiece == 2)
    {
        JPiece temp(gameArt);
        activePiece = &temp;
    }

    else if (firstPiece == 3)
    {
        ZPiece temp(gameArt);
        activePiece = &temp;
    }
    
    else if (firstPiece == 4)
    {
        Square temp(gameArt);
        activePiece = &temp;
    }

    else if (firstPiece == 5)
    {
        SPiece temp(gameArt);
        activePiece = &temp;
    }

    if (activePiece == nullptr)
    {
        TPiece temp(gameArt);
        activePiece = &temp;
    }

    queue<int> pieceLine; //queue of size 2, front is the active piece, back is the next piece in line that will be displayed in the preview
    pieceLine.push(firstPiece);
    pieceLine.push(nextPiece);

    Sprite previewArt;
    previewArt.setTexture(gameArt);
    previewArt.setTextureRect(previewPieces[nextPiece]);
    previewArt.setPosition(617.5, 350);
    previewArt.setScale(5, 5);

    Clock clock;
    Clock inputTracker;
    int movementFactor = 300;

    while (window.isOpen())
    {
        Time time1 = clock.getElapsedTime();
        Time time2 = clock.getElapsedTime();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (time1.asMilliseconds()  >= 100 && time1.asMilliseconds() < 1000 && activePiece->canMoveDown(grid))
        {
            activePiece->fall();
            clock.restart();
        }
        

        
        //*********************** USER INPUT *************************************88
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !xPressed)
        {

            activePiece->rotate("right", grid);
            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        }
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !zPressed)
        {

            activePiece->rotate("left", grid);
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && activePiece->canMoveLeft(grid) && !leftPressed)
        {

            activePiece->move("left");
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);

        }
        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && activePiece->canMoveLeft(grid))
        {
            if (time2.asMilliseconds() % movementFactor == 0)
            {
                activePiece->move("left");
                leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            }

        }
        */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && activePiece->canMoveRight(grid) && !rightPressed)
        {
            activePiece->move("right");
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

        }
        /*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && activePiece->canMoveRight(grid))
        {    
            if (time2.asMilliseconds() % movementFactor == 0)
            {
                activePiece->move("right");
                rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
            }

        }
        */




        xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
        
        if (!activePiece->canMoveDown(grid))
        {
            pieceLine.pop();
            int previewPiece = rng.randomInt(1, 6);
            pieceLine.push(previewPiece);
            int generatedPiece = pieceLine.front();
            previewArt.setTextureRect(previewPieces[previewPiece]);

            activePiece->updateGrid(grid);
            clearLines(grid);

            if (generatedPiece == 1)
            {
                TPiece temp2(gameArt);
                activePiece = &temp2;
            }

            else if (generatedPiece == 2)
            {
                JPiece temp2(gameArt);
                activePiece = &temp2;
            }

            else if (generatedPiece == 3)
            {
                ZPiece temp2(gameArt);
                activePiece = &temp2;
            }

            else if (generatedPiece == 4)
            {
                Square temp2(gameArt);
                activePiece = &temp2;
            }

            else if (generatedPiece == 5)
            {
                SPiece temp2(gameArt);
                activePiece = &temp2;
            }

        }

        
        //*********************************** DRAWING *************************************************************
        window.clear(Color(0,0,255));

        
        window.draw(gameBackground);
        drawGrid(window, grid, gameArt);
        vector<Sprite> v = activePiece->getSprites();
        for (int i = 0; i < v.size(); i++)
        {
            window.draw(v[i]);
        }
        window.draw(previewSquare);
        window.draw(previewArt);
        window.display();
    }

    

    return 0;
}


