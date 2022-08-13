#include "Game.h"
#include "Menu.h"

using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(800, 750), "Tetris");
	Menu menu(window);
	menu.displayMenu();
}


