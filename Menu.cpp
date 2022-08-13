#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;


    Menu::Menu(RenderWindow& window)
    {
        this->window = &window;
    }

    void Menu::displayMenu()
    {
        //RenderWindow window(sf::VideoMode(800, 750), "Tetris");
        Texture blockArt;
        blockArt.loadFromFile("menu_background.jpg");

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

            window->display();
        }
    }
