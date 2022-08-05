#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class TPiece {

private:

	Sprite block1;
	Sprite block2;
	Sprite block3;
	Sprite block4;
	vector<Sprite> sprites;
	float scaleFactor;
public:
	
	TPiece(Texture& gameArt)
	{
		scaleFactor = 5;

		block1.setTexture(gameArt);
		block2.setTexture(gameArt);
		block3.setTexture(gameArt);
		block4.setTexture(gameArt);

		block1.setTextureRect(IntRect(240, 688, 7, 7));
		block2.setTextureRect(IntRect(240, 688, 7, 7));
		block3.setTextureRect(IntRect(240, 688, 7, 7));
		block4.setTextureRect(IntRect(240, 688, 7, 7));

		block1.setPosition(400, 400);
		block2.setPosition(435, 400);
		block3.setPosition(470, 400);
		block4.setPosition(435, 430);

		block1.setScale(scaleFactor, scaleFactor);
		block2.setScale(scaleFactor, scaleFactor);
		block3.setScale(scaleFactor, scaleFactor);
		block4.setScale(scaleFactor, scaleFactor);

		sprites = { block1, block2, block3, block4 };
	}

	vector<Sprite> getSprites()
	{
		return sprites;
	}
};