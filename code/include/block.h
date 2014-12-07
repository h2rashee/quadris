#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "PRNG.h"
#include "window.h"
#include <map>

const int NUMBLOCKS = 7;
const char blockTypes[NUMBLOCKS] = {'I', 'J', 'L', 'O', 'T', 'S', 'Z'};

class Block
{	
	char type;				// the type of the block
	int x, y;					// the x and y coordinates of the top left square (used for graphical output)
	int levelGen;				// the level the block was generated in

	public: 

	Block()
	{
		type = (char)32, x = y = levelGen = -1;
	}	// ctor for Block

	Block(char, int, int, int);				// ctor for Block

	~Block();					// dtor for Block

	// --- Methods associated with graphical output

	void drawBlock(Xwindow*, std::map<char, int>);	// draws the block onto the specified window (only used to draw the nextBlock)
	void undrawBlock(Xwindow*);	// undraws the block from the specified window (only used to undraw the nextBlock)

	//---

	bool piece[4][4];			// the block in a 4-by-4 array
	
	void setX(int);			// sets the x coordinate
	int getX();				// returns x coordinate

	void setY(int);			// sets the y coordinate
	int getY();				// returns y coordinate

	void setType(char);			// sets the block type
	char getType();			// returns type

	int getLevel();			// returns levelGen

	void alignBlock();			// moves the block to the bottom left corner of the 4-by-4 grid (following a rotation)

	char genNextType(int, PRNG&);		// generates the next block's type

	friend std::ostream& ::operator<<(std::ostream&, Block*);	// outputs the block
};
#endif
