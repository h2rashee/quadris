#ifndef __BOARD_H__
#define __BOARD_H__
#include "block.h"
#include "cell.h"
#include <map>
#include <vector>

const int MAXBLOCKS = 180;		// a rough upper bound on the number of blocks there will ever be on the board
const int ROWS = 15;			// number of rows of the board
const int COLS = 10;			// number of columns of the board

class Board : public Block, public Cell {

	Block* currBlock;			// the current active block
	Block* nextBlock;			// the next block
	Cell grid[ROWS+3][COLS];		// represents the game board (the first 3 rows are to accomodate initial rotation of the block)
	char* charSequence;			// array of chars storing the sequence of block types from sequence.txt (when in level 0)
	int seed;					// the seed used to generate random numbers for the PRNG class

	//--- Variables dealing with the handling of level 0
	
	int sizeCharSequence;		// the size of the number of block types input from sequence.txt
	int nextChar;				// the next block to be loaded from sequence.txt
	bool noBlocksLeft;			// stores whether we have any blocks left from sequence.txt
	bool beenToLevel0;			// stores whether it is the first time you hit level 0 (to open sequence.txt only the first time)

	//---

	int level;				// the current level
	int highScore;				// the highest score achieved so far
	int currScore;				// the current score
	int blockLevelGen[MAXBLOCKS]; // the level in which blocks with specific ID's were generated
	bool usedID[MAXBLOCKS];		// stores whether a number is used as an ID of a block

	PRNG* prng;				// an instance of the PRNG class (for generating random number)

	public:

	Board(int, bool);		// ctor for Board
	
	~Board();					// dtor for Board	

	void restartGame();			// resets all the variables (with minimal redrawing if in graphics mode

	std::string intToString(int); // converts an integer to a string (i.e. 123 -> "123")

	//--- Variables/Methods associated with graphical output

	Xwindow* gameWindow;		// the window the board is displayed on
	Xwindow* nextBlockWindow;	// the window the next block is displayed on
	
	bool graphicsMode;			// stores whether graphics mode is on or off
	bool isMoveMadeLegal;		// stores whether the last move made was a legal one (to redraw the iff the last move was legal)
	void initBlockColors();		// initializes the colors for each block type and stores it in 'colors' (see below)
	void initGrid();			// draws the grids on the both the above windows
	void drawCurrBlock();		// draws the current block onto the board
	void undrawCurrBlock();		// undraws the current block on the board	

	//--- Variables/Methods associated with the outputting of the shadow of the current block
	
	int shadowX, shadowY;
	void locateShadow(int&,int&);	// determines where the current block's shadow should be
	void drawShadowBlock();		// draws the current block onto the board
	void undrawShadowBlock(bool);	// undraws the current block on the board

	//---
	std::map <char, int> typeColors;		// maps each block type to a specific color
	
	std::vector< std::pair< std::pair<int, int>, Cell> > usedCells;			// stored the cells on the grid which are currently being used
									// (this is to minimize the amount of redrawing on the grid)

	//---
	
	//--- Variables/Methods associated with cheat mode (bonus feature)

	bool cheatMode;			// stores whether cheats have been activated
	void toggleCheats();		// sets cheat mode to 1 (i.e. activates cheat mode)
	bool up(int);				// moves the current block up (returns true iff possible)
	void boom();				// clears the screen
	//---

	void readCharSequence();		// inputs the values for charSequence from sequence.txt

	void clearRow(int, int);		// clears a given row and updates score (also used when the cheat 'clear' is called, but doesn't update score)
	
	void canClear();			// checks if any row is full, and clears it

	bool outOfBounds(int, int);	// checks whether a position is on the board or not

	bool legalMove();			// checks if the currBlock overlaps the pieces already on the board (or if currBlock is out of bounds)

	bool gameOver();			// returns whether the game is over (i.e. if currBlock overlaps the grid upon construction)

	void loadNextBlock();		// reassigns currBlock to nextBlock, and generates a new nextBlock

	void assignBlockID(int, int);	// assigs a block ID to the recently dropped block

	int nextBlockID();			// returns the next unused block ID

	// For {left, right, down, rotate}, To minimize the amount of redrawing, we pass in the number of times the movement has been called before 
	// and if this number is 0 (i.e. if this is the first time the move is being made), then undraw the current block (if in graphicsMode)

	bool left(int);			// moves the currBlock left (returns true iff possible)

	bool right(int);			// moves the currBlock right (returns true iff possible)

	bool down(int);			// moves the currBlock down (returns true iff possible)

	bool drop();				// drops the currBlock
	
	bool rotate(bool, int);		// rotates the block CW or CCW depending on input (1 - CW, 0 - CCW) (returns true iff possible)

	bool levelUp();			// increases level by 1 (returns true iff possible)
			
	bool levelDown();			// decreases level by 1 (returns true iff possible)
	
	void updateScore(int);		// updates score

	void cutAndPasteCurrBlock(int);						// cut, or pastes, currBlock into the grid (for output purposes)

	friend std::ostream& ::operator<<(std::ostream&, Board*);	// outputs the Board
	
};
#endif
