#pragma once
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

class Game
{
private:
        
        RenderWindow* window;
        int level;

        void initGrid(vector<vector<int>>& grid);
        void printGrid(vector<vector<int>>& grid);
        void drawGrid(RenderWindow& window, vector<vector<int>>& grid, Texture& gameArt);
        void downShift(vector<vector<int>>& grid, int clearedRow);
        void clearLines(vector<vector<int>>& grid, int level, int& score, int& totalLinesCleared);
        string numAsString(int score, int length);
        bool isGameOver(vector<vector<int>>& grid);
        void readHighScores(ifstream& file, vector<int>& scores);
        void updateHighScore(vector<int>& scores, int score);

public:

        Game(int level, RenderWindow& window);
        void runGame();

};



