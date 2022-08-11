#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Menu
{
private:

public:
	void displayMenu()
	{
        sf::RenderWindow window(sf::VideoMode(800, 750), "Tetris");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
            window.display();
        }
	}
};