#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;

class Menu
{
private:
    RenderWindow* window;
public:

    Menu(RenderWindow& window);
    void displayMenu();

};