SFML := -lsfml-graphics -lsfml-window -lsfml-system

tetris: main.o Game.o Menu.o main.cpp Screens/Game.cpp Screens/Menu.cpp
	g++ -c main.cpp Screens/Game.cpp Screens/Menu.cpp -g
	g++ main.o Game.o Menu.o -o tetris $(SFML)
