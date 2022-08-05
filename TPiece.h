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
	int state;

	void updateState(int& state, string type)
	{
		if (type.compare("left") == 0)
		{
			state++;

			if (state == 5)
			{
				state = 1;
			}
		}

		if (type.compare("right") == 0)
		{
			state--;

			if (state == 0)
			{
				state = 4;
			}
		}
	}

	void updateSprites(vector<Sprite>& sprites)
	{
		sprites = { block1, block2, block3, block4 };
	}

public:
	
	TPiece(Texture& gameArt)
	{
		scaleFactor = 5;
		state = 1;

		block1.setTexture(gameArt);
		block2.setTexture(gameArt);
		block3.setTexture(gameArt);
		block4.setTexture(gameArt);

		block1.setTextureRect(IntRect(240, 688, 7, 7));
		block2.setTextureRect(IntRect(240, 688, 7, 7));
		block3.setTextureRect(IntRect(240, 688, 7, 7));
		block4.setTextureRect(IntRect(240, 688, 7, 7));

		block1.setPosition(330, 50);
		block2.setPosition(365, 50);
		block3.setPosition(400, 50);
		block4.setPosition(365, 85);

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

	void rotate(string type)
	{
		updateState(state, type);

		//NOTE: We are using block2 as the pivot
		float pivot_x = block2.getPosition().x;
		float pivot_y = block2.getPosition().y;
		
		if (state == 1)
		{
			block1.setPosition(pivot_x - 35, pivot_y);
			block3.setPosition(pivot_x + 35, pivot_y);
			block4.setPosition(pivot_x, pivot_y + 35);

		}

		if (state == 2)
		{
			block1.setPosition(pivot_x, pivot_y - 35);
			block3.setPosition(pivot_x, pivot_y + 35);
			block4.setPosition(pivot_x - 35, pivot_y);
		}

		if (state == 3)
		{
			block1.setPosition(pivot_x - 35, pivot_y);
			block3.setPosition(pivot_x + 35, pivot_y);
			block4.setPosition(pivot_x, pivot_y - 35);
		}

		if (state == 4)
		{
			block1.setPosition(pivot_x, pivot_y - 35);
			block3.setPosition(pivot_x, pivot_y + 35);
			block4.setPosition(pivot_x + 35, pivot_y);
		}
		updateSprites(sprites);
	}

	void fall()
	{
		block1.setPosition(block1.getPosition().x, block1.getPosition().y + 35);
		block2.setPosition(block2.getPosition().x, block2.getPosition().y + 35);
		block3.setPosition(block3.getPosition().x, block3.getPosition().y + 35);
		block4.setPosition(block4.getPosition().x, block4.getPosition().y + 35);

		updateSprites(sprites);
	}

	void move(string type)
	{
		if (type.compare("left") == 0)
		{
			block1.setPosition(block1.getPosition().x - 35, block1.getPosition().y);
			block2.setPosition(block2.getPosition().x - 35, block2.getPosition().y);
			block3.setPosition(block3.getPosition().x - 35, block3.getPosition().y);
			block4.setPosition(block4.getPosition().x - 35, block4.getPosition().y);
		}

		if (type.compare("right") == 0)
		{
			block1.setPosition(block1.getPosition().x + 35, block1.getPosition().y);
			block2.setPosition(block2.getPosition().x + 35, block2.getPosition().y);
			block3.setPosition(block3.getPosition().x + 35, block3.getPosition().y);
			block4.setPosition(block4.getPosition().x + 35, block4.getPosition().y);
		}

		updateSprites(sprites);
	}

	bool canMoveLeft()
	{
		for (int i = 0; i < sprites.size(); i++)
		{
			if (sprites[i].getPosition().x == 225)
			{
				return false;
			}
		}

		if (!canMoveDown())
		{
			return false;
		}

		return true;
	}

	bool canMoveRight()
	{
		for (int i = 0; i < sprites.size(); i++)
		{
			if (sprites[i].getPosition().x == 225+350-35)
			{
				return false;
			}
		}

		if (!canMoveDown())
		{
			return false;
		}

		return true;
	}

	bool canMoveDown()
	{
		for (int i = 0; i < sprites.size(); i++)
		{
			if (sprites[i].getPosition().y == 715)
			{
				return false;
			}
		}
		return true;
	}
};