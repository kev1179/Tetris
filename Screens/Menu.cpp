#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;

    //Reads the high_scores.txt file to get the top three highest scores and place them into a vector for storage.
    void Menu::getHighScores(vector<int>& scores, ifstream& file)
    {
        string line;
        while (getline(file, line))
        {
            scores.push_back(stoi(line));
        }
    }

    //Helper function that converts a number into a string of a certain length. For example, numAsString(12,6) would return 000012.
    string Menu::numAsString(int score, int length)
    {
        string returnString = to_string(score);
        int numLeadingZeroes = returnString.size();

        for (int i = 0; i < length - numLeadingZeroes; i++)
        {
            returnString.insert(returnString.begin(), '0');
        }

        return returnString;
    }

    /*Constructor that passes the game window by reference.This prevents a new window from being created when the screen changes.It then
    initalizes the class variable window which is a pointer to the game window. */
    Menu::Menu(RenderWindow& window)
    {
        this->window = &window;
    }

    //Code that displays the menu screen.
    void Menu::displayMenu()
    {
        //Loads image for the menu background
        Texture blockArt;
        blockArt.loadFromFile("Assets/menu_background.jpg");

        //Loads up the high scores file and then places them into a vector 
        vector<int> topScores;
        ifstream file("MISC/high_scores.txt");
        getHighScores(topScores, file);
        file.close();

        //Sets the portion of the background image that is displayed
        Sprite background;
        background.setTexture(blockArt);
        background.setTextureRect(IntRect(645, 1035 - 750, 800, 750));

        //Displays the black background for the level select portion
        float bx = 125;
        float by = 200;
        RectangleShape levelSelectBG;
        levelSelectBG.setSize(Vector2f(550, 350));
        levelSelectBG.setFillColor(Color::Black);
        levelSelectBG.setPosition(bx, by);

        //Vector to keep all the text for all 19 of the levels displayed
        vector<Text> levelText;
        Font font;
        font.loadFromFile("Assets/text_font.ttf");

        //This is the code that populates the above vector so it can be drawn to the screen later
        float rowShift = 0;
        float x = 0;
        for (int i = 0; i <= 19; i++)
        {
            if (i != 0 && i % 5 == 0)
            {
                rowShift += 100;
                x = 0;
            }
            //This is the text for each level number and the position and number is modified upon each iteration
            Text temp;
            temp.setFont(font);
            temp.setString(to_string(i));
            temp.setPosition((20 + bx) + 120 * x, by + rowShift);
            temp.setFillColor(Color(255, 255, 255));
            temp.setScale(1.3, 1.3);
            x++;

            levelText.push_back(temp);
        }
        //This line makes level 0 red and the one currently selected when the program boots up
        levelText[0].setFillColor(Color(255, 0, 0));

        //booleans to keep track of keyboard input
        bool leftPressed = false;
        bool rightPressed = false;
        //The current level selected in the menu
        int level = 0;

        //This uses the above vector we made containing the top 3 scores and displays them as in game text
        vector<Text> highScoreText;
        for (int i = 0; i < 3; i++)
        {
            Text temp;
            temp.setFont(font);
            temp.setString(numAsString(topScores[i],6));
            temp.setPosition(300, 590 + 50 * i);
            temp.setScale(2, 2);
            
            highScoreText.push_back(temp);
        }

        //Text that says "Top 3 Scores"
        Text scoreHeader;
        scoreHeader.setFont(font);
        scoreHeader.setString("Top 3 Scores");
        scoreHeader.setPosition(300, 570);
        scoreHeader.setScale(1.1, 1.1);
        highScoreText.push_back(scoreHeader);

        //Black background for the high scores
        RectangleShape highScoreBG;
        highScoreBG.setSize(Vector2f(240, 200));
        highScoreBG.setFillColor(Color::Black);
        highScoreBG.setPosition(280, 570);

        //Text that says "Select Level"
        Text levelHeader;
        levelHeader.setFont(font);
        levelHeader.setString("Select Level");
        levelHeader.setPosition(300, 100);
        levelHeader.setScale(1.1, 1.1);

        //Black background for the levels
        RectangleShape levelHeaderBG;
        levelHeaderBG.setSize(Vector2f(240, 50));
        levelHeaderBG.setFillColor(Color::Black);
        levelHeaderBG.setPosition(280, 100);

        //Game Loop that displays menu screen
        while (window->isOpen())
        {
            Event event;
            while (window->pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window->close();
            }

            //Code for changing the currently selected level
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !leftPressed)
            {
                levelText[level].setFillColor(Color(255, 255, 255));
                if (level != 0)
                {
                    level--;
                }

                else
                {
                    level = 19;
                }

                levelText[level].setFillColor(Color(255, 0, 0));
                leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            }

            //Same but if they press right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !rightPressed)
            {
                levelText[level].setFillColor(Color(255, 255, 255));
                if (level != 19)
                {
                    level++;
                }

                else
                {
                    level = 0;
                }

                levelText[level].setFillColor(Color(255, 0, 0));
                rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
            }

            //Starts the game if they press enter
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                break;
            }

            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

            //************************************************* DRAWING ***********************************************
            window->clear();
            window->draw(background);
            window->draw(levelSelectBG);

            for (int i = 0; i < levelText.size(); i++)
            {
                window->draw(levelText[i]);
            }

            window->draw(highScoreBG);

            for (int i = 0; i < highScoreText.size(); i++)
            {
                window->draw(highScoreText[i]);
            }

            window->draw(levelHeaderBG);
            window->draw(levelHeader);

            window->display();
        }
                Game game(level, *window);
                game.runGame();
    }
