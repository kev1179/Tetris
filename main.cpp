#include "Screens/Game.h"
#include "Screens/Menu.h"

using namespace std;
using namespace sf;

//Creates the window for the game. Starts the game at the menu screen which is defined in the Menu class.
int main()
{
	RenderWindow window(VideoMode(800, 750), "Tetris");
	Menu menu(window);
	menu.displayMenu();
}


