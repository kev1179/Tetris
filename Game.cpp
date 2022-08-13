#pragma once
#include <iostream>
#include "Menu.h"
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
#include "LPiece.h"
#include "Line.h"
#include <queue>
#include <fstream>

using namespace std;
using namespace sf;

    //Initializes empty 10x20 grid
    void Game::initGrid(vector<vector<int>>& grid)
    {
        vector<int> row = { 0,0,0,0,0,0,0,0,0,0 };

        for (int i = 0; i < 20; i++)
        {
            grid.push_back(row);
        }
    }

    //Helper function to help print current grid for debugging purposes
    void Game::printGrid(vector<vector<int>>& grid)
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
    void Game::drawGrid(RenderWindow& window, vector<vector<int>>& grid, Texture& gameArt)
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
                square.setPosition(j * 7 * scaleFactor + 225, i * 7 * scaleFactor + 50);
                square.setScale(scaleFactor, scaleFactor);
                if (grid[i][j] != 0)
                {
                    window.draw(square);
                }
            }
        }
    }

    //Recursive function that shifts all the lines in the grid down after a line has been cleared
    void Game::downShift(vector<vector<int>>& grid, int clearedRow)
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
    void Game::clearLines(vector<vector<int>>& grid, int level, int& score, int& totalLinesCleared)
    {
        vector<int> emptyRow = { 0,0,0,0,0,0,0,0,0,0 };
        int linesCleared = 0;
        unordered_map<int, int> pointValues;
        pointValues[0] = 0;
        pointValues[1] = 40;
        pointValues[2] = 100;
        pointValues[3] = 300;
        pointValues[4] = 1200;

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
                downShift(grid, i - 1);
                linesCleared++;
            }

        }
        score += pointValues[linesCleared] * (level + 1);
        totalLinesCleared += linesCleared;

        if (score > 999999)
        {
            score = 999999;
        }
    }

    string Game::numAsString(int score, int length)
    {
        string returnString = to_string(score);
        int numLeadingZeroes = returnString.size();

        for (int i = 0; i < length - numLeadingZeroes; i++)
        {
            returnString.insert(returnString.begin(), '0');
        }

        return returnString;
    }

    bool Game::isGameOver(vector<vector<int>>& grid)
    {
        vector<int> topRow = grid[0];

        for (int i = 0; i < topRow.size(); i++)
        {
            if (topRow[i] != 0)
            {
                return true;
            }
        }
        return false;
    }

    void Game::readHighScores(ifstream& file, vector<int>& scores)
    {
        string line;
        while (getline(file, line))
        {
            scores.push_back(stoi(line));
        }
    }

    void Game::updateHighScore(vector<int>& scores, int score)
    {
        ofstream file("high_scores.txt");
        bool done = false;
        scores.push_back(score);
        std::sort(scores.begin(), scores.end());
        scores.erase(scores.begin());
        std::sort(scores.begin(), scores.end(), greater<>());

        for (int i = 0; i < scores.size(); i++)
        {
            file << scores[i] << endl;
        }
        file.close();
    }

    Game::Game(int level, RenderWindow& window)
    {
        this->level = level;
        this->window = &window;
    }

    void Game::runGame()
    {
        RNG rng;
        vector<vector<int>> grid;
        initGrid(grid);

        int score = 0;
        int totalLinesCleared = 0;
        vector<int> highestScores;
        unordered_map<int, int> statTracker;
        statTracker[1] = 0;
        statTracker[2] = 0;
        statTracker[3] = 0;
        statTracker[4] = 0;
        statTracker[5] = 0;
        statTracker[6] = 0;
        statTracker[7] = 0;

        unordered_map<int,int> levelSpeed;
        int tracker = 1;
        for (int i = 19; i >= 0; i--)
        {
            levelSpeed[i] = 20 * tracker;
            tracker++;
        }

        ifstream highScores("high_scores.txt");
        readHighScores(highScores, highestScores);
        highScores.close();

        Texture gameArt;
        gameArt.loadFromFile("sprites.png");
        Texture background;
        background.loadFromFile("background.jpg");

        bool xPressed = false;
        bool zPressed = false;
        bool leftPressed = false;
        bool rightPressed = false;
        int keyDelayCounter = 0;
        int keyDelay = 70;

        RectangleShape gameBackground;
        gameBackground.setSize(Vector2f(350, 700));
        gameBackground.setFillColor(Color::Black);
        gameBackground.setPosition(225, 50);

        Sprite* backgroundImage = new Sprite;
        backgroundImage->setTexture(background);
        backgroundImage->setTextureRect(IntRect(0, 0, 800, 750));

        unordered_map<int, IntRect> previewPieces;
        previewPieces[1] = IntRect(240, 688, 23, 15);
        previewPieces[2] = IntRect(240, 728, 23, 15);
        previewPieces[3] = IntRect(240, 768, 23, 15);
        previewPieces[4] = IntRect(244, 808, 15, 15);
        previewPieces[5] = IntRect(240, 848, 23, 15);
        previewPieces[6] = IntRect(240, 888, 23, 15);
        previewPieces[7] = IntRect(276, 932, 31, 7);

        RectangleShape previewSquare;
        previewSquare.setSize(Vector2f(180, 180));
        previewSquare.setFillColor(Color::Black);
        previewSquare.setPosition(600, 300);

        Piece* activePiece = nullptr;

        int firstPiece = rng.randomInt(1, 8);
        int nextPiece = rng.randomInt(1, 8);

        if (firstPiece == 1)
        {
            TPiece* temp = new TPiece(gameArt);
            activePiece = temp;
            statTracker[1]++;
        }

        else if (firstPiece == 2)
        {
            JPiece* temp = new JPiece(gameArt);
            activePiece = temp;
            statTracker[2]++;
        }

        else if (firstPiece == 3)
        {
            ZPiece* temp = new ZPiece(gameArt);
            activePiece = temp;
            statTracker[3]++;
        }

        else if (firstPiece == 4)
        {
            Square* temp = new Square(gameArt);
            activePiece = temp;
            statTracker[4]++;
        }

        else if (firstPiece == 5)
        {
            SPiece* temp = new SPiece(gameArt);
            activePiece = temp;
            statTracker[5]++;
        }

        else if (firstPiece == 6)
        {
            LPiece* temp = new LPiece(gameArt);
            activePiece = temp;
            statTracker[6]++;
        }

        else if (firstPiece == 7)
        {
            Line* temp = new Line(gameArt);
            activePiece = temp;
            statTracker[7]++;
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
        previewArt.setPosition(642.5 - 17.5, 385 - 17.5);
        previewArt.setScale(5, 5);

        Clock clock;
        Clock inputTracker;
        int movementFactor = 300;

        Font textFont;
        textFont.loadFromFile("text_font.ttf");

        RectangleShape scoreBox;
        scoreBox.setSize(Vector2f(180, 180));
        scoreBox.setFillColor(Color::Black);
        scoreBox.setPosition(600, 30);

        Text scoreDisplay;
        scoreDisplay.setFont(textFont);
        scoreDisplay.setString(numAsString(score, 6));
        scoreDisplay.setCharacterSize(50);
        scoreDisplay.setPosition(642.5 - 35, 150);

        Text scoreText;
        scoreText.setFont(textFont);
        scoreText.setString("SCORE");
        scoreText.setCharacterSize(50);
        scoreText.setPosition(642.5 - 22.75, 110);

        Text topScoreDisplay;
        topScoreDisplay.setFont(textFont);
        topScoreDisplay.setString(numAsString(highestScores[0], 6));
        topScoreDisplay.setCharacterSize(50);
        topScoreDisplay.setPosition(642.5 - 35, 70);

        Text topScoreText;
        topScoreText.setFont(textFont);
        topScoreText.setString("TOP");
        topScoreText.setCharacterSize(50);
        topScoreText.setPosition(642.5, 30);

        Text lineText;
        lineText.setFont(textFont);
        lineText.setString("LINES " + numAsString(totalLinesCleared, 3));
        lineText.setCharacterSize(50);
        lineText.setPosition(300, -10);

        vector<Sprite> statPieces;
        int difference = 30;
        for (int i = 1; i <= 7; i++)
        {
            Sprite temp;
            temp.setTexture(gameArt);
            temp.setTextureRect(previewPieces[i]);

            if (i == 7)
            {
                temp.setPosition(60 - difference, 100 + i * 75);
            }

            else
            {
                temp.setPosition(80 - difference, 100 + i * 75);
            }

            temp.setScale(3, 3);
            statPieces.push_back(temp);
        }

        RectangleShape statBox;
        statBox.setSize(Vector2f(190, 525));
        statBox.setFillColor(Color::Black);
        statBox.setPosition(50 - difference, 150);

        vector<Text> statDisplay;

        for (int i = 0; i < 7; i++)
        {
            Text temp;
            temp.setFont(textFont);
            temp.setString(numAsString(0, 3));
            temp.setCharacterSize(50);
            temp.setPosition(642.5, 30);
            temp.setPosition(170 - difference, 160 + i * 75);
            temp.setScale(0.75, 0.75);
            temp.setFillColor(Color(255, 0, 0));

            statDisplay.push_back(temp);
        }

        while (window->isOpen())
        {


            for (int i = 0; i < statDisplay.size(); i++)
            {
                statDisplay[i].setString(numAsString(statTracker[i + 1], 3));
            }

            Time time1 = clock.getElapsedTime();
            Time time2 = clock.getElapsedTime();

            Event event;
            while (window->pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window->close();
            }

            if (time1.asMilliseconds() >= levelSpeed[level] && time1.asMilliseconds() < 1000 && activePiece->canMoveDown(grid))
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


            /*
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && activePiece->canMoveLeft(grid) && !leftPressed)
            {

                activePiece->move("left");
                leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);

            }
             */
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && activePiece->canMoveLeft(grid))
            {
                keyDelayCounter++;

                if (keyDelayCounter == keyDelay)
                {
                    activePiece->move("left");
                    leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
                    keyDelayCounter = 0;
                }

            }
            /*
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && activePiece->canMoveRight(grid) && !rightPressed)
            {
                activePiece->move("right");
                rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

            }
            */
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && activePiece->canMoveRight(grid))
            {
                keyDelayCounter++;

                if (keyDelayCounter == keyDelay)
                {
                    activePiece->move("right");
                    rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
                    keyDelayCounter = 0;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && activePiece->canMoveDown(grid))
            {
                keyDelayCounter++;

                if (keyDelayCounter == keyDelay)
                {
                    activePiece->fall();
                    leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
                    keyDelayCounter = 0;
                }

            }

            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

            if (!activePiece->canMoveDown(grid) && !isGameOver(grid))
            {
                pieceLine.pop();
                int previewPiece = rng.randomInt(1, 8);
                pieceLine.push(previewPiece);
                int generatedPiece = pieceLine.front();
                previewArt.setTextureRect(previewPieces[previewPiece]);

                activePiece->updateGrid(grid);
                clearLines(grid, level, score, totalLinesCleared);
                scoreDisplay.setString(numAsString(score, 6));
                lineText.setString("LINES " + numAsString(totalLinesCleared, 3));

                delete activePiece;

                if (generatedPiece == 1)
                {
                    TPiece* temp2 = new TPiece(gameArt);
                    activePiece = temp2;
                    statTracker[1]++;
                }

                else if (generatedPiece == 2)
                {
                    JPiece* temp2 = new JPiece(gameArt);
                    activePiece = temp2;
                    statTracker[2]++;
                }

                else if (generatedPiece == 3)
                {
                    ZPiece* temp2 = new ZPiece(gameArt);
                    activePiece = temp2;
                    statTracker[3]++;
                }

                else if (generatedPiece == 4)
                {
                    Square* temp2 = new Square(gameArt);
                    activePiece = temp2;
                    statTracker[4]++;
                }

                else if (generatedPiece == 5)
                {
                    SPiece* temp2 = new SPiece(gameArt);
                    activePiece = temp2;
                    statTracker[5]++;
                }

                else if (generatedPiece == 6)
                {
                    LPiece* temp2 = new LPiece(gameArt);
                    activePiece = temp2;
                    statTracker[6]++;
                }

                else if (generatedPiece == 7)
                {
                    Line* temp2 = new Line(gameArt);
                    activePiece = temp2;
                    statTracker[7]++;
                }

            }

            else if (isGameOver(grid))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    ofstream writeHighScore("high_scores.txt");
                    updateHighScore(highestScores, score);

                    for (int i = 0; i < grid.size(); i++)
                    {
                        for (int j = 0; j < grid[i].size(); j++)
                        {
                            grid[i][j] = 0;
                        }
                    }
                    score = 0;
                    totalLinesCleared = 0;
                    scoreDisplay.setString(numAsString(0, 6));
                    lineText.setString("LINES " + numAsString(totalLinesCleared, 3));
                    statTracker[1] = 0;
                    statTracker[2] = 0;
                    statTracker[3] = 0;
                    statTracker[4] = 0;
                    statTracker[5] = 0;
                    statTracker[6] = 0;
                    statTracker[7] = 0;

                    for (int i = 0; i < statDisplay.size(); i++)
                    {
                        statDisplay[i].setString(numAsString(0, 3));
                    }
                    clock.restart();

                    Menu menu(*window);
                    menu.displayMenu();
                    window->close();
                    break;
                }

            }

            //*********************************** DRAWING *************************************************************
            window->clear(Color(0, 0, 255));

            window->draw(*backgroundImage);
            window->draw(gameBackground);
            drawGrid(*window, grid, gameArt);
            vector<Sprite> v = activePiece->getSprites();
            for (int i = 0; i < v.size(); i++)
            {
                window->draw(v[i]);
            }
            window->draw(previewSquare);
            window->draw(previewArt);
            window->draw(scoreBox);
            window->draw(scoreDisplay);
            window->draw(scoreText);
            window->draw(topScoreDisplay);
            window->draw(topScoreText);
            window->draw(lineText);

            window->draw(statBox);
            for (int i = 0; i < statPieces.size(); i++)
            {
                window->draw(statPieces[i]);
            }

            for (int i = 0; i < statDisplay.size(); i++)
            {
                window->draw(statDisplay[i]);
            }

            window->display();

        }

        delete backgroundImage;

    }
