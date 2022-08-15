#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;

    void Menu::getHighScores(vector<int>& scores, ifstream& file)
    {
        string line;
        while (getline(file, line))
        {
            scores.push_back(stoi(line));
        }
    }

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

    Menu::Menu(RenderWindow& window)
    {
        this->window = &window;
    }

    void Menu::displayMenu()
    {
        //RenderWindow window(sf::VideoMode(800, 750), "Tetris");
        Texture blockArt;
        blockArt.loadFromFile("menu_background.jpg");
        vector<int> topScores;
        ifstream file("high_scores.txt");
        getHighScores(topScores, file);
        file.close();

        Sprite background;
        background.setTexture(blockArt);
        background.setTextureRect(IntRect(645, 1035 - 750, 800, 750));

        float bx = 125;
        float by = 200;
        RectangleShape levelSelectBG;
        levelSelectBG.setSize(Vector2f(550, 350));
        levelSelectBG.setFillColor(Color::Black);
        levelSelectBG.setPosition(bx, by);

        vector<Text> levelText;
        Font font;
        font.loadFromFile("text_font.ttf");

        float rowShift = 0;
        float x = 0;
        for (int i = 0; i <= 19; i++)
        {
            if (i != 0 && i % 5 == 0)
            {
                rowShift += 100;
                x = 0;
            }
            Text temp;
            temp.setFont(font);
            temp.setString(to_string(i));
            temp.setPosition((20 + bx) + 120 * x, by + rowShift);
            temp.setFillColor(Color(255, 255, 255));
            temp.setScale(1.3, 1.3);
            x++;

            levelText.push_back(temp);
        }
        levelText[0].setFillColor(Color(255, 0, 0));

        bool leftPressed = false;
        bool rightPressed = false;
        int level = 0;

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
        Text scoreHeader;
        scoreHeader.setFont(font);
        scoreHeader.setString("Top 3 Scores");
        scoreHeader.setPosition(300, 570);
        scoreHeader.setScale(1.1, 1.1);
        highScoreText.push_back(scoreHeader);

        RectangleShape highScoreBG;
        highScoreBG.setSize(Vector2f(240, 200));
        highScoreBG.setFillColor(Color::Black);
        highScoreBG.setPosition(280, 570);

        Text levelHeader;
        levelHeader.setFont(font);
        levelHeader.setString("Select Level");
        levelHeader.setPosition(300, 100);
        levelHeader.setScale(1.1, 1.1);

        RectangleShape levelHeaderBG;
        levelHeaderBG.setSize(Vector2f(240, 50));
        levelHeaderBG.setFillColor(Color::Black);
        levelHeaderBG.setPosition(280, 100);

        while (window->isOpen())
        {
            Event event;
            while (window->pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window->close();
            }

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                Game game(level, *window);
                game.runGame();
                break;
            }
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

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
    }
