#include <iostream>
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include "MISC/RNG.h"
#include <vector>
#include <unordered_map>
#include "Pieces/TPiece.h"
#include "Pieces/Piece.h"
#include "Pieces/JPiece.h"
#include "Pieces/ZPiece.h"
#include "Pieces/Square.h"
#include "Pieces/SPiece.h"
#include "Pieces/LPiece.h"
#include "Pieces/Line.h"
#include <queue>
#include <fstream>

using namespace std;
using namespace sf;

    //Initializes empty 10x20 grid.
    void Game::initGrid(vector<vector<int>>& grid)
    {
        vector<int> row = { 0,0,0,0,0,0,0,0,0,0 };

        for (int i = 0; i < 20; i++)
        {
            grid.push_back(row);
        }
    }

    //Helper function to help print current grid for debugging purposes.
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

    //Function that takes in the grid of pieces and draws the squares that are present at each location.
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

    //Recursive function that shifts all the lines in the grid down after a line has been cleared.
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

    //Helper function that converts a number into a string of a certain length. For example, numAsString(12,6) would return 000012.
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

    //Checks if the game is over by seeing if there are pieces in the top row of the 2D vector.
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

    //Reads the high_scores.txt file and places the top 3 highest scores into a vector for easy access.
    void Game::readHighScores(ifstream& file, vector<int>& scores)
    {
        string line;
        while (getline(file, line))
        {
            scores.push_back(stoi(line));
        }
    }

    //Updates the high scores after the game is over.
    void Game::updateHighScore(vector<int>& scores, int score)
    {
        ofstream file("MISC/high_scores.txt");
        bool done = false;
        scores.push_back(score);

        /*The algorthim here is to first add the current score in question to the end of the vector.We then sort the vector from least to
        greatest. Then, it removes the first element from the sorted vector which basically just removes the smallest element. Then, the
        vector is resorted, this time greatest to least and then this vector is written to the file to have the scores in a top 3 greatest
        to least format.*/

        std::sort(scores.begin(), scores.end());
        scores.erase(scores.begin());
        std::sort(scores.begin(), scores.end(), greater<>());

        for (int i = 0; i < scores.size(); i++)
        {
            file << scores[i] << endl;
        }
        file.close();
    }

    /*Constructor for game object.initalizes the game leveland passes the game window in by referenceand initalizes a pointer that points to
    the game window in memory.*/
    Game::Game(int level, RenderWindow& window)
    {
        this->level = level;
        this->window = &window;
    }

    //Runs the game and displays the graphics to the screen. Performs game logic as well.
    void Game::runGame()
    {
        //Creates object used for random number generation
        RNG rng;

        //Creates the 2D vector used to represent the grid and fills it with zeroes
        vector<vector<int>> grid;
        initGrid(grid);

        //initalizes variables used for tracking the score and total number of lines cleared
        int score = 0;
        int totalLinesCleared = 0;

        //Creates vector to store the top 3 highest scores and then reads them from a file to fill the vector
        vector<int> highestScores;
        ifstream highScores("MISC/high_scores.txt");
        readHighScores(highScores, highestScores);
        highScores.close();

        //Hashmap that keeps track of how many of each type of piece has been generated. 
        /* KEY:
        *  1 - TPiece
        *  2 - JPiece
        *  3 - ZPiece
        *  4 - Square
        *  5 - SPiece
        *  6 - LPiece
        *  7 - Line
        */
        unordered_map<int, int> statTracker;
        statTracker[1] = 0;
        statTracker[2] = 0;
        statTracker[3] = 0;
        statTracker[4] = 0;
        statTracker[5] = 0;
        statTracker[6] = 0;
        statTracker[7] = 0;

        //Hashmap that sets the speed for each level. The formula here is that the base is 20 (lower means less time to spawn aka faster)
        //So it starts at level 19, sets the speed to 20 and it goes up by 20 each iteration down to 0
        unordered_map<int,int> levelSpeed;
        int tracker = 1;
        for (int i = 19; i >= 0; i--)
        {
            levelSpeed[i] = 20 * tracker;
            tracker++;
        }

        //Loads art used in game
        Texture gameArt;
        gameArt.loadFromFile("Assets/sprites.png");
        Texture background;
        background.loadFromFile("Assets/background.jpg");

        //Booleans and tracker variables used to monitor user input
        bool xPressed = false;
        bool zPressed = false;
        bool leftPressed = false;
        bool rightPressed = false;
        int keyDelayCounter = 0;
        int keyDelay = 70;

        //Black background used for playing field
        RectangleShape gameBackground;
        gameBackground.setSize(Vector2f(350, 700));
        gameBackground.setFillColor(Color::Black);
        gameBackground.setPosition(225, 50);

        //Background image in the game
        Sprite* backgroundImage = new Sprite;
        backgroundImage->setTexture(background);
        backgroundImage->setTextureRect(IntRect(0, 0, 800, 750));

        //Hashmap that reads the sprite data and sets the rectangle corresponding to the preview image for each piece
        unordered_map<int, IntRect> previewPieces;
        previewPieces[1] = IntRect(240, 688, 23, 15);
        previewPieces[2] = IntRect(240, 728, 23, 15);
        previewPieces[3] = IntRect(240, 768, 23, 15);
        previewPieces[4] = IntRect(244, 808, 15, 15);
        previewPieces[5] = IntRect(240, 848, 23, 15);
        previewPieces[6] = IntRect(240, 888, 23, 15);
        previewPieces[7] = IntRect(276, 932, 31, 7);

        //Black background for the piece preview
        RectangleShape previewSquare;
        previewSquare.setSize(Vector2f(180, 180));
        previewSquare.setFillColor(Color::Black);
        previewSquare.setPosition(600, 300);

        //This is the activepiece in the game or the one the player currently has control of
        Piece* activePiece = nullptr;

        //This generates the first two pieces
        int firstPiece = rng.randomInt(1, 8);
        int nextPiece = rng.randomInt(1, 8);

        //The following code decides which piece to generate first and also updates the tracker that keeps track how many of each piece has
        //been generated. The different piece types being generated was achieved using polymorphism. The number generated determines what type 
        //of object to generate
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

        //queue of size 2, front is the active piece, back is the next piece in line that will be displayed in the preview
        queue<int> pieceLine; 
        pieceLine.push(firstPiece);
        pieceLine.push(nextPiece);

        //Sprite for the preview image of the next piece
        Sprite previewArt;
        previewArt.setTexture(gameArt);
        previewArt.setTextureRect(previewPieces[nextPiece]);
        previewArt.setPosition(642.5 - 17.5, 385 - 17.5);
        previewArt.setScale(5, 5);

        //Clock object used for timing purposes
        Clock clock;
        Clock inputTracker;
        int movementFactor = 300;

        //Loads up font from file
        Font textFont;
        textFont.loadFromFile("Assets/text_font.ttf");

        //Black background for where score is displayed
        RectangleShape scoreBox;
        scoreBox.setSize(Vector2f(180, 180));
        scoreBox.setFillColor(Color::Black);
        scoreBox.setPosition(600, 30);

        //Displays the score as text on screen
        Text scoreDisplay;
        scoreDisplay.setFont(textFont);
        scoreDisplay.setString(numAsString(score, 6));
        scoreDisplay.setCharacterSize(50);
        scoreDisplay.setPosition(642.5 - 35, 150);

        //Text that says "SCORE"
        Text scoreText;
        scoreText.setFont(textFont);
        scoreText.setString("SCORE");
        scoreText.setCharacterSize(50);
        scoreText.setPosition(642.5 - 22.75, 110);

        //Displays high score as in game text
        Text topScoreDisplay;
        topScoreDisplay.setFont(textFont);
        topScoreDisplay.setString(numAsString(highestScores[0], 6));
        topScoreDisplay.setCharacterSize(50);
        topScoreDisplay.setPosition(642.5 - 35, 70);

        //Displays text that says "TOP" on screen
        Text topScoreText;
        topScoreText.setFont(textFont);
        topScoreText.setString("TOP");
        topScoreText.setCharacterSize(50);
        topScoreText.setPosition(642.5, 30);

        //Text that shows how many lines have been cleared in total during the game
        Text lineText;
        lineText.setFont(textFont);
        lineText.setString("LINES " + numAsString(totalLinesCleared, 3));
        lineText.setCharacterSize(50);
        lineText.setPosition(300, -10);

        //vector that houses the images for the stat section pieces on the left
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

        //Black background on the left
        RectangleShape statBox;
        statBox.setSize(Vector2f(190, 525));
        statBox.setFillColor(Color::Black);
        statBox.setPosition(50 - difference, 150);

        //Populates vector with text for each type of piece displaying how many of each have been generated
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

        //Game Loop
        while (window->isOpen())
        {

            //Sets the text on the left to the appropriate values
            for (int i = 0; i < statDisplay.size(); i++)
            {
                statDisplay[i].setString(numAsString(statTracker[i + 1], 3));
            }

            //Gets the time since last clock reset
            Time time1 = clock.getElapsedTime();
            Time time2 = clock.getElapsedTime();

            Event event;
            while (window->pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window->close();
            }

            //Performs the piece falling, calls the hashmap levelSpeed to determine the speed based off the level
            if (time1.asMilliseconds() >= levelSpeed[level] && time1.asMilliseconds() < 1000 && activePiece->canMoveDown(grid))
            {
                activePiece->fall();
                clock.restart();
            }



            //*********************** USER INPUT *************************************

            //Performs counterclockwise rotation if user presses X key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !xPressed)
            {

                activePiece->rotate("right", grid);
                xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
            }

            //Performs clockwise rotation if user presses Z key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !zPressed)
            {

                activePiece->rotate("left", grid);
                zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
            }

            //Moves piece left if user presses left arrow key
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

            //Moves piece right if user presses right arrow key
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

            //Moves piece down if user presses down arrow key
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

            //Updates state of key press boolean tracker variables
            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

            //Code that executes if a piece lands on the floor or another piece and it is time to generate a new piece
            if (!activePiece->canMoveDown(grid) && !isGameOver(grid) && !activePiece->canMoveLeft(grid) && !activePiece->canMoveRight(grid))
            {
                //Updates the queue and generates a new piece and places it into the back of the queue
                //Also uses this info to display the correct preview piece
                pieceLine.pop();
                int previewPiece = rng.randomInt(1, 8);
                pieceLine.push(previewPiece);
                int generatedPiece = pieceLine.front();
                previewArt.setTextureRect(previewPieces[previewPiece]);

                //Updates the grid upon landing and clears any lines needed as well as updates the score and line counter
                activePiece->updateGrid(grid);
                clearLines(grid, level, score, totalLinesCleared);
                scoreDisplay.setString(numAsString(score, 6));
                lineText.setString("LINES " + numAsString(totalLinesCleared, 3));

                //deallocates the memory used for activePiece before reassigning it
                delete activePiece;

                //Performs piece generation again and assigns that to activePiece
                //Pieces were created on the heap because of memory concerns
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

            //Code that runs if the game is over
            else if (isGameOver(grid))
            {
                //if the user hits enter, take user to menu, update high score if needed, and reset game data
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    ofstream writeHighScore("MISC/high_scores.txt");
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

        //Deallocates memory used for the background image
        delete backgroundImage;

    }
