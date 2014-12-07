#include "include/block.h"

using namespace std;

// Block Constructor
Block::Block(char t, int xx, int yy, int lvl)
{
	type = t, x = xx, y = yy, levelGen = lvl;		// Initialises variables

	for(int i = 0; i < 4; i++)					// Initialises grid spots to be false i.e. nothing exists there
	{
		for (int j = 0; j < 4; j++)
		{
			piece[i][j] = 0;
		}
	}

	if (type == 'I')							// I-block configuration
	{
		/*
			----
			----
			----
			IIII
		*/

		for (int i = 0; i < 4; i++)				// Sets the grid's locations as used depending on the block configuration and with below
		{
			piece[3][i] = 1;
		}
	}

	else if (type == 'J')						// J-block configuration
	{
		/*
			----
			----
			J---
			JJJ-
		*/

		piece[2][0] = piece[3][0] = piece[3][1] = piece[3][2] = 1;
	}
	else if (type == 'L')						// L-block configuration
	{
		/*
			----
			----
			--L-
			LLL-
		*/

		piece[2][2] = piece[3][0] = piece[3][1] = piece[3][2] = 1;
	}

	else if (type == 'O')						// O-block configuration
	{
		/*
			----
			----
			OO--
			OO--
		*/

		piece[2][0] = piece[2][1] = piece[3][0] = piece[3][1] = 1;
	}
	else if (type == 'S')						// S-block configuration
	{
		/*
			----
			----
			-SS-
			SS--
		*/

		piece[3][0] = piece[3][1] = piece[2][1] = piece[2][2] = 1;
	}
	else if (type == 'Z')						// Z-block configuration
	{
		/*
			----
			----
			ZZ--
			-ZZ-
		*/

		piece[2][0] = piece[2][1] = piece[3][1] = piece[3][2] = 1;
	}
	else if (type == 'T')						// T-block configuration
	{
		/*
			----
			----
			TTT-
			-T--
		*/

		piece[2][0] = piece[2][1] = piece[2][2] = piece[3][1] = 1;
	}
}

// Block destructor
Block::~Block()
{
}

// Getter method for the x co-ordinate of the Block
int Block::getX()
{
	return x;
}

// Getter method for the y co-ordinate of the Block
int Block::getY()
{
	return y;
}

// Getter method for the type of the Block
char Block::getType()
{
	return type;
}

// Getter method for the level that the Block was generated in
int Block::getLevel()
{
	return levelGen;
}

// Setter method for the x co-ordinate of the Block
void Block::setX(int x1)
{
	x = x1;
}

// Setter method for the y co-ordinate of the Block
void Block::setY(int y1)
{
	y = y1;
}

// Setter method for the type of the Block
void Block::setType(char t)
{
	type = t;
} 

// Aligns the Block to the bottom left corner of the 4x4 Block grid following a rotation
void Block::alignBlock()
{
	bool isEmpty = 1;

	// Loops through the grid to align the Block to the left
	while(1)
	{
		isEmpty = 1;					// Resets the flag

		// Checks if the first column is empty
		for(int i = 0; i < 4; i++)
		{
			if (piece[i][0])
			{
				isEmpty = 0;			// Sets the flag if the column is empty
				break;
			}
		}

		// If the first column is not empty and the block is now aligned to the left
		if (!isEmpty)
		{
			break;
		}

		// Shifts all the cells in the grid, one column to the left
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				piece[i][j] = piece[i][j+1];
			}
		}

		// Sets the fourth column as being unused
		for (int i = 0; i < 4; i++)
		{
			piece[i][3] = 0;
		}
	}

	// Loops through the grid to align the Block to the bottom
	while(1)
	{
		isEmpty = 1;					// Resets the flag

		// Checks if the fourth row is empty
		for (int i = 0; i < 4; i++)
		{
			if (piece[3][i])
			{
				isEmpty = 0;			// Sets the flag if the row is empty
				break;
			}
		}
		
		// If the fourth row is not empty and the block is now aligned to the bottom
		if (!isEmpty)
		{
			break;
		}

		// Shifts all the cells in the grid, one row down
		for (int i = 3; i > 0; --i)
		{
			for (int j = 0; j < 4; j++)
			{
				piece[i][j] = piece[i-1][j];
			}
		}

		// Sets the first row as being unused
		for (int i = 0; i < 4; i++)
		{
			piece[0][i] = 0;
		}
	}
}

// Generates the next Block Type so that it can be displayed to the user
char Block::genNextType(int lvl, PRNG& prng)
{
	int rand;
	char t;
	
	if (lvl == 1)						// Level 1 Block Selection
	{
		rand = prng(11);

		if (rand == 0)					// 1/12 probability for the S-block
		{
			t = 'S';
		}
		else if (rand == 1)				// 1/12 probability for the Z-block
		{
			t = 'Z';
		}
		else							// 1/6 probability for all other blocks
		{	
			rand /= 2;
			--rand;
			t = blockTypes[rand];
		}
	}

	else if (lvl == 2)					// Level 2 Block Selection
	{
		t = blockTypes[prng(6)];			// An equal probability of 1/7 for each block
	}

	else if (lvl == 3)					// Level 3 Block Selection
	{
		rand = prng(8);

		if (rand == 0 || rand == 1)			// 2/9 probability for the S-block
		{
			t = 'S';
		}
		else if (rand == 2 || rand == 3)		// 2/9 probability for the Z-block
		{
			t = 'Z';
		}
		else								// 1/9 probability for all other blocks
		{
			rand -= 4;
			t = blockTypes[rand];
		}
	}
	return t;
}

// Draws the given Block to the given window
void Block::drawBlock(Xwindow* w, map<char, int> c)
{
	for (int i = 0; i < 4; i++)			// For every spot in the grid that contains a Block cell
	{
		for (int j = 0; j < 4; j++)
		{
			if (piece[i][j])
			{
				w->fillRectangle(40 * j + 70 + 1, 40 * i + 70 + 1, 38, 38, c[type]);
				// Fill the 40x40 cell situated 70 pixels from the top of the 
				// window with the type's colour in a 38x38 square
			}
		}
	}
}

//Removes any drawing in the given window
void Block::undrawBlock(Xwindow* w)
{
	for (int i = 0; i < 4; i++)			// For every spot in the grid that contains a Block cell
	{
		for (int j = 0; j < 4; j++)
		{
			if (piece[i][j])
			{
				w->fillRectangle(40 * j + 70 + 1, 40 * i + 70 + 1, 38, 38, Xwindow::White);
				// Fill the 40x40 cell situated 70 pixels from the 
				// top of the window with a white 38x38 square
			}
		}
	}
}

// Operator overload for the output operator for Block
ostream& operator<<(ostream& out, Block* bl)
{
	for (int i = 0; i < 4; i++)
	{
		// If the current row is empty, don't output it
		for (int j = 0; j < 4; j++)
		{
			if (bl->piece[i][j])	// if there is a piece in this row
			{
				goto next;		// continue with outputting this row
			}
		}
		continue;					// otherwise go to the next row
		next:;
		for (int j = 0; j < 4; j++)
		{
			if(bl->piece[i][j])		// If the cell is occupied
			{
				out << bl->type;	// Output the character
			}
			else
			{
				out << (char)32;	// Otherwise a space
			}
		}
		out << endl;				// Output blank line
	}
	
	out << endl;

	return out;
}
