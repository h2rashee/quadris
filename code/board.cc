#include <fstream>
#include "include/board.h"

using namespace std;

// Board constructor
Board::Board(int s, bool graphics)
{
	level = isMoveMadeLegal = 1;						// Game is started at level 1, makes the previous move made legal (this 
												// is just to ensure the drawing of the current block)
												
	shadowX = shadowY = -1;

	beenToLevel0 = false;							// we haven't been to level 0 yet (since we start at level 1) 
	
	noBlocksLeft = currScore = cheatMode = 0;			// Current score is zero, cheat mode should be deactivated and noBlocksLeft
												// is false initially (since we start at Level 1) 

	highScore = 0;									// High score is initially set to 0
	
	seed = s;										// The seed is set appropriately

	graphicsMode = graphics;							// Graphics mode is set if applicable

	if (graphicsMode)								// Make additional initialisations for the variables associated with graphics
	{
		initBlockColors();
		gameWindow = new Xwindow(500, 600);
		nextBlockWindow = new Xwindow(300, 300);
		initGrid();
	}

	charSequence = NULL;							// Nothing in character sequence from sequence.txt
	
	prng = new PRNG;								// Pseudo Random Number Generator class is reset

	if(seed)										// If a seed is set by the user
	{
		prng->seed(seed);							// then set it
	}

	currBlock = new Block(genNextType(level, *prng), 0, 0, level);	// Initialises the current block hovering on the board
	nextBlock = new Block(genNextType(level, *prng), 0, 0, level);	// Initialises the next block that will be on the board after input

	for (int i = 0; i < MAXBLOCKS; i++)				// For every block				 
	{
		usedID[i] = 0;								// Initially none of the block ID's are used
		blockLevelGen[i] = -1;						// And no blocks are generated, so set the level generated to -1
	}

	// Cycles through the entire board
	for (int i = 0; i < ROWS+3; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			grid[i][j].type = (char)32;					//Sets empty "cell" spaces to have a block type of SPACE
			grid[i][j].levelGen = grid[i][j].blockID = -1;	//Sets the grid location to have a block ID of -1 because it is empty
		}
	}
}

// Board Destructor
Board::~Board()
{
	delete [] charSequence;
	delete currBlock;
	delete nextBlock;
	delete gameWindow;
	delete nextBlockWindow;
	delete prng;
}

// Called when the game is restarted or if the previous game was lost (i.e. Game Over was hit)
// Resets all the variables and deletes and stored information from the previous game
// (except for high score)
void Board::restartGame()
{
	isMoveMadeLegal = 1;								// sets the last move made legal

	beenToLevel0 = noBlocksLeft = cheatMode = 0;				// resets the variables associated with level 0
													// and cheat mode is deactivated
	
	delete [] charSequence;								// reset the charSequence array

	charSequence = NULL;

	if (graphicsMode)									// If we are in graphics mode then reset the board graphically
	{	
		for (int i = 0; i < usedCells.size(); i++)			// undraw all the cells on the board
		{
			usedCells[i].second.undrawCell(gameWindow, usedCells[i].first.second, usedCells[i].first.first);
		}

		// Undraw the current and next blocks
		undrawCurrBlock();							
		nextBlock->undrawBlock(nextBlockWindow);			
			
		// Redraw the string displaying the level if required
		if (level != 1)								
		{
			gameWindow->fillRectangle(70, 20, 50, 20, Xwindow::White);
			gameWindow->drawString(15, 30, "             1");
		}

		// Redrwa the string displaying the current score if required
		if (currScore != 0)							
		{	
			gameWindow->fillRectangle(70, 50, 50, 20, Xwindow::White);
			gameWindow->drawString(15, 60, "             0");
		}
		currScore = 0;									// the current score is initially 0
	}

	level = 1;										// A restarted game always starts at level one

	// delete all the cells stored in our 'usedCells' vector
	while(usedCells.size())
	{	
		usedCells.erase(usedCells.begin()+0);
	}

	// resets the used ID's and the level each block is generated in
	for (int i = 0; i < MAXBLOCKS; i++)					
	{
		usedID[i] = 0;								
		blockLevelGen[i] = -1;							
	}

	for (int i = 0; i < ROWS+3; i++)						// For every cell on the board
	{
		for (int j = 0; j < COLS; j++)
		{
			// reset the type and level generated of the cell
			grid[i][j].type = (char)32;					
			grid[i][j].levelGen = grid[i][j].blockID = -1;    
		}
	}

	delete currBlock;
	delete nextBlock;
	delete prng;

	prng = new PRNG;
	if (seed)
	{
		prng->seed(seed);		// if the user specificed a seed, seed the random number generator with the given seed
	}

	// Generate the current and next block based on the given seed
	currBlock = new Block(genNextType(level, *prng), 0, 0, level);
	nextBlock = new Block(genNextType(level, *prng), 0, 0, level);
}

// Converts an integer to a string (i.e. 123 -> "123")
string Board::intToString(int num)
{
	if (num == 0)							// Special Case: if the number is 0, return "0" 
	{
		return "0";
	}

	string tmp = "", ret = "";

	while(num)							// while the number isn't 0
	{
		tmp += (char)((num%10) + (int)'0');     // convert units digit to a character and add it to the string 't'
		num /= 10;						// truncate the number by one digit (i.e. remove its units digit)
	}

	for (int i = tmp.length()-1; i >= 0; --i)	// reverse the string (since our string currently stores the 
										// reverse of our number)
	{
		ret += tmp[i];
	}

	return ret;
}

// Initialises the colors of our 7 standard blocks
void Board::initBlockColors()
{
	int color = 1;
	for (int i = 0; i < NUMBLOCKS; i++)		// set block i to color i (specified in the from the window class
										// to be the appropriate classical Tetris colors for each block)
	{
		typeColors[blockTypes[i]] = color++;
	}
}

// Toggles cheat mode
void Board::toggleCheats()
{
	cheatMode ^= 1;						// activates cheat mode when deactivated, and vice versa
}

// Draws the board 
void Board::initGrid()
{
	// Draws 'gridlines' for the board in the main window, where each gridline is a rectangle of width/height 1
	// and are 25 units away from each other (with the first gridline being 125 units away from the edge of the 
	// window)
	
	for (int i = 0; i < 19; i++)	// Draws the vertical gridlines
	{
		gameWindow->fillRectangle (125, 25 * i + 125, 250, 1, Xwindow::Black);
 	}		

	for (int i = 0; i < 11; i++) // Draws the horizontal gridlines
	{
		gameWindow->fillRectangle(25 * i + 125, 125, 1, 450, Xwindow::Black);
	}

	// Draws the strings that displays the current level, score and highscore
	gameWindow->drawString(15, 30, "Level:       " + intToString(level));	
  	gameWindow->drawString(15, 60, "Score:       " + intToString(currScore));
  	gameWindow->drawString(15, 90, "Hi Score:    " + intToString(highScore));
  

	// Each gridline in the window that displays the next block is spaced 40 units away from 
	// each other with the leftmost and topmost lines being 70 units away from window's edge	
	
	for (int i = 0; i < 5; i++) // Draws the vertical/horizontal lines 
	{
		nextBlockWindow->fillRectangle(40 * i + 70, 70, 1, 160, Xwindow::Black);
    		nextBlockWindow->fillRectangle(70, 40 * i + 70, 160, 1, Xwindow::Black); 
  	}

	// Displays "Next Block" in the window that displays the next block
  	nextBlockWindow->drawString(120, 35, "Next Block");
}

// Draws the current block in the main window
void Board::drawCurrBlock()
{
	if (graphicsMode)							// Draw the current block if graphics mode is activated
	{
		// Sets the block's information (x, y coordinates and block color) 
		int cx = currBlock->getX();
		int cy = currBlock->getY();
		int blockColor = typeColors[currBlock->getType()];  

		/*
			Each square of the current block is drawn on the board as
			a square with side length 23 units (since each cell of the grid
			is a square with side length 25 units, this draws each square of
			the current block completely inside a cell of the grid to ensure
			that none of the grid lines is altered when drawing/undrawing each 
			square of the current block)
		*/

		// For each cell in the 4-by-4 grid that stores the configuration of the block
		for (int i = cx; i < cx + 4; i++)
		{		
			for (int j = cy; j < cy + 4; j++)
			{
				if (currBlock->piece[i-cx][j-cy])	// if there is a portion of the block in this particular cell
				{
					// Draw the portion of the block in the main window 
					gameWindow->fillRectangle(25 * j + 125 + 1, 25 * i + 125 + 1, 23, 23, blockColor);
				}
			}
		}
	
		drawShadowBlock();
	}
}

// Undraws the current block
void Board::undrawCurrBlock()
{
	if (graphicsMode)			// Only undraw the current block when graphics mode activated
	{
		// Sets the block's x and y coordinates
		int cx = currBlock->getX();
		int cy = currBlock->getY();  
		
		// For each cell in the 4-by-4 grid that stores the configuration of the current block
		for (int i = cx; i < cx + 4; i++)
		{
			for (int j = cy; j < cy + 4; j++)
			{
				if (currBlock->piece[i-cx][j-cy]) // if there is a portion of the block in this particular cell
				{
					// Undraw (i.e. draw a white square) in the main window
					gameWindow->fillRectangle(25 * j + 125 + 1, 25 * i + 125 + 1, 23, 23, Xwindow::White);
				}
			}
		}
		
		undrawShadowBlock(1);
	}
}

// Determines where the shadow of the current block should be
void Board::locateShadow(int &x, int &y)
{
	int numMoved = 0;		// the number of times current block was moved down

	while(down(1))			// move the current block down until you can't
	{	
		numMoved++;
	}
	
	// Move the current block back up the number of times we moved
	// it down (so that the current block stays unaffected)
	for (int i = 0; i < numMoved; i++)
	{
		up(1);
	}

	// Return the location of the shadow (which is numMoved
	// units below the currentBlock)
	x = currBlock->getX() + numMoved;
	y = currBlock->getY();
}

// Draws the current block's shadow
void Board::drawShadowBlock()
{
	if (graphicsMode)						// Draw the shadow only if graphics mode is activated
	{	
		// set the position of the current block's shadow to shadowX/Y
		locateShadow(shadowX, shadowY);
	
		// if the shadow is incident with the current block, don't draw it
		if (shadowX == currBlock->getX() && shadowY == currBlock->getY())
		{
			return;
		}

		// the shadow color is set to black
		int shadowColor = Xwindow::Black;  

		// For each cell in the 4-by-4 grid that stores the configuration of the current block
		for (int i = shadowX; i < shadowX + 4; i++)
		{		
			for (int j = shadowY; j < shadowY + 4; j++)
			{
				if (currBlock->piece[i-shadowX][j-shadowY])	// if there is a portion of the shadow in this particular cell
				{
					// Draw the portion of the shadow in the main window 
					gameWindow->fillRectangle(25 * j + 125 + 5, 25 * i + 125 + 5, 15, 15, shadowColor);
				}
			}
		}
	}
}

// Undraw the current block's shadow from the board
void Board::undrawShadowBlock(bool update)
{
	if (graphicsMode)					// Undraw the shadow only if graphics mode is activated
	{		 	
		// if the shadow was updated (i.e. if the last move its position)
		if (update)
		{
			// relocate its position
			locateShadow(shadowX, shadowY);
		
			// if the shadow is incident with the current block, don't draw it
			if (shadowX == currBlock->getX() && shadowY == currBlock->getY())
			{
				return;
			}
		}

		// For each cell in the 4-by-4 grid that stores the configuration of the current block
		for (int i = shadowX; i < shadowX + 4; i++)
		{		
			for (int j = shadowY; j < shadowY + 4; j++)
			{
				// if there is a portion of the shadow in this particular cell
				if (currBlock->piece[i-shadowX][j-shadowY])
				{
					// Undraw the portion of the shadow in the main window 
					gameWindow->fillRectangle(25 * j + 125 + 5, 25 * i + 125 + 5, 15, 15, Xwindow::White);
				}
			}
		}
		
		// if the shadow wasn't updated (i.e. if the last move didn't alter its position)
		if (update == 0)
		{
			drawCurrBlock();			// redraw the current block (as undrawing the shadow could have
									// potentially removed parts of the current block if the overlapped)
		}
	}
}

// Determines whether a given position is out of the board
bool Board::outOfBounds(int x, int y)
{
	return (x < 0 || x >= ROWS+3 || y < 0 || y >= COLS);
}

// Determines whether the last move made was legal by making sure that the 
// current block doesn't overlap with any used cell on the board and isn't out of bounds
bool Board::legalMove()
{
	// Sets block's x and y coordinates
	int cx = currBlock->getX();
	int cy = currBlock->getY();
	
	// For every cell in the 4-by-4 grid that stores the configuration of the current block
	for (int i = cx; i < cx + 4; i++)
	{
		for (int j = cy; j < cy + 4; j++)
		{
			if (currBlock->piece[i-cx][j-cy]) // if there is a portion of the block in this particular cell
			{
				// if this portion is out of bounds or if there is a used cell on the same position of the board
				if (outOfBounds(i, j) || grid[i][j].type != (char)32)
				{
					return false; // the last move made wasn't a legal one
				}
			}
		}
	}
	
	return true;	// otherwise the last move made was legal
}

// Inputs the block types for level 0 from sequence.txt 
void Board::readCharSequence()
{
	ifstream fin("sequence.txt");				// open sequence.txt the first time to determine the # of block 
										// types in the file
	string s;

	sizeCharSequence = nextChar = 0;			// stores the # of block types in the file and the next character
										// respectively

	while(fin >> s)
	{
		sizeCharSequence++;			// count the # of block type in the file
	}

	fin.close();

	fin.open("sequence.txt");		 		// open sequence.txt for input		
	charSequence = new char [sizeCharSequence];

	for (int i = 0; i < sizeCharSequence; i++)	// input all the block types in the file
	{
		fin >> s;
		
		// if the input string isn't a standard block type
		if (s.length() != 1 || !(s == "L" || s == "T" || s == "S" || s == "I" || s == "O" || s == "J" || s == "Z")) 
		{
			charSequence[i] = '*'; // store '*'
		}
		else
		{
			charSequence[i] = s[0]; // store the input block type
		}
	}
}

// Determines whether the game is over
bool Board::gameOver()
{
	return noBlocksLeft||!legalMove(); // if there are no blocks left in sequence (for level 0) or if the
								// current block overlaps with an existing block on the board 
}

/*
	In the functions that actually move the current block,
	to avoid redrawing the current block multiple times, 
	we only undraw the current block if it is the first time
	ths move is being called (e.g. if 5left is the entered command,
	rather than undrawing and redrawing the current block after moving 
	left each of the 5 times, we only undraw it the first time it 
	is moved left)			
*/

// Moves the current block one cell to the left
bool Board::left(int timesCalled)
{
	currBlock->setY(currBlock->getY()-1);			// Move the block left
	
	// if this isn't legal
	if (!legalMove())
	{
		currBlock->setY(currBlock->getY()+1);		// Move the block back right
		return false;
	}

	// if this is the first time left is called
	if (timesCalled == 0 && graphicsMode) 
	{
		currBlock->setY(currBlock->getY()+1);		// Move the block back right (to undraw it)
	
		undrawCurrBlock();

		currBlock->setY(currBlock->getY()-1);		// Move the block left
	}

	return true;
}

// Moves the current block one cell to the right
bool Board::right(int timesCalled)
{	
	currBlock->setY(currBlock->getY()+1);			// Move the block right

	// if this isn't legal 
	if (!legalMove())
	{
		currBlock->setY(currBlock->getY()-1);		// Move the block back left
		return false;
	}		

	// if this is the first time right is called
	if (timesCalled == 0 && graphicsMode)	
	{
		currBlock->setY(currBlock->getY()-1);		// Move the block back left (to undraw it)

		undrawCurrBlock();
		
		currBlock->setY(currBlock->getY()+1);		// Move the block right
	}

	return true;
}

// (Only for cheat mode) Moves the current block one cell up
bool Board::up(int timesCalled)
{	
	currBlock->setX(currBlock->getX()-1);			// Move the block up
	
	// if this isn't legal
	if (!legalMove())
	{
		currBlock->setX(currBlock->getX()+1);		// Move the block back down
		return false;
	}
	
	// if times is the first time up is called
	if (timesCalled == 0 && graphicsMode)
	{
		currBlock->setX(currBlock->getX()+1);		// Move the block back down (to undraw it)

		undrawCurrBlock();	
		
		currBlock->setX(currBlock->getX()-1);		// Move the block up
	}
	
	return true;
}

// Moves the current block one cell down
bool Board::down(int timesCalled)
{	
	currBlock->setX(currBlock->getX()+1);			// Move the block down

	// if this isn't legal
	if (!legalMove())
	{
		currBlock->setX(currBlock->getX()-1);		// Move the block back up
		return false;
	}
	
	// if times is the first time down is called
	if (timesCalled == 0 && graphicsMode)
	{
		currBlock->setX(currBlock->getX()-1);		// Move the block back up (to undraw it)
		
		undrawCurrBlock();

		currBlock->setX(currBlock->getX()+1);		// Move the block down
	}
	
	return true;
}

// Drops the current block on the board
bool Board::drop()
{
	int numDown = 0;

	while(down(numDown++));							// Continuously move the current block down
												// (down() will return false when you can't move down)

	cutAndPasteCurrBlock(0);							// Paste the current block onto the board

	assignBlockID(nextBlockID(), currBlock->getLevel());	// Sets the block ID and the level generated for the
												// dropped block

	canClear();									// clears any rows that have filled up

	loadNextBlock();								// loads the next block

	if (gameOver() || (level == 0 && noBlocksLeft))
	{
		return false;
	}
	else
	{
		return true;
	}
}

// (Only for cheat mode) Clears the board
void Board::boom()
{	
	if (graphicsMode)				  // Undraw the used cells if in graphics mode
	{	
		for (int i = 0; i < usedCells.size(); i++)
		{
			usedCells[i].second.undrawCell(gameWindow, usedCells[i].first.second, usedCells[i].first.first);
		}
		
		locateShadow(shadowX, shadowY); // locates where the current block's shadow should be
	}

	while(usedCells.size())		  	  // resets the used cells vector
	{
		usedCells.erase(usedCells.begin()+0);
	}

	// For every cell in the board
	for (int i = 0; i < ROWS+3; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			// Reset the type, level generated and block ID
			grid[i][j].type = (char)32;
			grid[i][j].levelGen = grid[i][j].blockID = -1;
		}
	}

	// if the shadow is not on the first row (i.e. if the 4-by-4 grid storing shadow is above the 4th last row)
	if (shadowX < (ROWS+3) - 4)
	{
		undrawShadowBlock(0);      // then it has to be redrawn (as all the used cells were cleared)
	}

	// No block ID is being used as all the filled cells are cleared
	for (int i = 0; i < MAXBLOCKS; i++)
	{
		usedID[i] = 0;
	}
}

// Determines what the next block ID should be
int Board::nextBlockID()
{
	// Return the first unused block ID
	for (int i = 0; i < MAXBLOCKS; i++)
	{
		if (!usedID[i]) 	// if the ith block ID isn't used
		{
			usedID[i] = 1;	// set it to used
			return i;	
		}
	}
}

// Assigns a block ID to the current block
void Board::assignBlockID(int ID, int lvlGen)
{
	blockLevelGen[ID] = lvlGen;						// the block with the given ID is generated in the given level
	
	// Get the block's information
	int cx = currBlock->getX(), cy = currBlock->getY();
	
	// For every cell in the 4-by-4 grid that stores the current block's configuration
	for (int i = cx; i < cx + 4; i++)
	{
		for (int j = cy; j < cy + 4; j++)
		{
			if (currBlock->piece[i-cx][j-cy])			// if this cell stores a portion of the block
			{
				// sets the ID and level generated of the this cell to the given values 
				grid[i][j].blockID = ID;
				grid[i][j].levelGen = lvlGen;
			
				if (graphicsMode) 					// if graphics mode is activated
				{
					// add the current cell into the used celld vector and draw the cell onto the board
					usedCells.push_back(make_pair(make_pair(i, j), grid[i][j]));
					usedCells[usedCells.size()-1].second.drawCell(	gameWindow, 
															usedCells[usedCells.size()-1].first.second, 
															usedCells[usedCells.size()-1].first.first, 
															typeColors[usedCells[usedCells.size()-1].second.type]);
				}
			}
		}
	}
}

// Loads the next block
void Board::loadNextBlock()
{
	// sets the current block to be the next block
	delete currBlock;
	currBlock = new Block(nextBlock->getType(), nextBlock->getX(), nextBlock->getY(), nextBlock->getLevel());	

	char nextType;
	
	// If we are in level 0
	if (level == 0)
	{
		// Keep skipping the next character in charSequence if it isn't standard
		while(charSequence[nextChar] == '*')
		{
			cerr << "Given block type is invalid. Skipping..." << endl;
			nextChar++;
		}

		nextType = charSequence[nextChar]; 
	}
	else
	{
		nextType = nextBlock->genNextType(level, *prng);	// the type of the next block is decided based on the current level
	}

	// if the new next block's type is different from the old next block's type (to ensure minimal redrawing)
	// and if graphics mode is activated
	if (graphicsMode && nextBlock->getType() != nextType)
	{
		nextBlock->undrawBlock(nextBlockWindow);		// undraw the old next block
	}

	delete nextBlock;

	// if we are in level 0 and we are (possibly) at the last letter of the charSequence 
	if (level == 0 && (nextChar >= sizeCharSequence))	
	{
		nextBlock = new Block();						// the next block window will be empty (as there isn't one)
		
		// if we used all the types in charSequence
		if (nextChar >= sizeCharSequence)
		{
			noBlocksLeft = 1;						// there are no blocks left in sequence.txt
		}

		nextChar++;
	}
	else
	{
		// generate the new block (based on what level we are currently on)
		nextBlock = new Block((level==0?charSequence[nextChar++]:nextType), 0, 0, level);
	}
}

// Rotates the current block in the given direction
bool Board::rotate(bool CW, int timesCalled)
{
	bool tmp[4][4], tmp2[4][4];					// tmp stores the rotated piece, tmp2 stores the original piece 
											// (in case we want to restore the piece if the rotation is illegal)
	
	// creates a copy of the 4-by-4 grid that stores the configuration of the current block
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp2[i][j] = currBlock->piece[i][j];
		}
	}

	/*
		To rotate the block, we created a bijective function that maps coordinates
		between two 4-by-4 grids. If we want to rotate clockwise, we map (i, j) ->
		(3 - j, i). If we want to rotate counterclockwise, we map (i, j) -> (j, 3 - i)
	*/

	if (CW)							// if we want to rotate the piece CW, then do so
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				tmp[i][j] = currBlock->piece[3-j][i];
			}
		}
	}
	else					 			// otherwise, rotate it CCW
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				tmp[i][j] = currBlock->piece[j][3-i];
			}
		}
	}

	// set the current block to the rotated temporary grid
	for (int i = 0; i < 4; i++)			
	{
		for (int j = 0; j < 4; j++)
		{
			currBlock->piece[i][j] = tmp[i][j];
		}
	}

	currBlock->alignBlock();				// aligns the block to the bottom left corner in the 4-by-4 grid that contains it

	// if the move was legal
	if (legalMove())
	{
		// if this is the first time the piece is being rotated
		if (timesCalled == 0)			
		{
			for (int i = 0; i < 4; i++)			// restore the original current block
			{
				for (int j = 0; j < 4; j++)
				{
					currBlock->piece[i][j] = tmp2[i][j];
				}
			}
			undrawCurrBlock();					// undraw the current block
		}
		
		for (int i = 0; i < 4; i++)			// restore the rotated piece
		{
			for (int j = 0; j < 4; j++)
			{
				currBlock->piece[i][j] = tmp[i][j];
			}
		}

		currBlock->alignBlock();			// aligns the block to the bottom left corner in the 4-by-4 rectangle that contains it	
		return true;
	}

	for (int i = 0; i < 4; i++)			// restore the original current block
	{
		for (int j = 0; j < 4; j++)
		{
			currBlock->piece[i][j] = tmp2[i][j];
		}
	}

	return false;
}

// Increases the current level by 1 (iff possible)
bool Board::levelUp()
{
	// if we can go up one level
	if (level < 3)
	{
		level++;													// increase the current level by 1

		// if graphics mode is activated
		if (graphicsMode)		
		{	
			// redraw the string that displays the current level
			gameWindow->fillRectangle(70, 20, 50, 20, Xwindow::White);
			gameWindow->drawString(15, 30, "             " + intToString(level));
		}
		return true;
	}
	return false;
}

// Decreases the current level by 1 (iff possible)
bool Board::levelDown()
{
	// if we can go down one level
	if (level > 0)
	{
		--level;				// decrease the current level by 1

		// if the level we moved down to is 0 and we haven't been to level 0 before
		if (level == 0 && !beenToLevel0)
		{
			beenToLevel0 = 1;	// now we have been to level 0
			readCharSequence();	// input the block types from sequence.txt
		}
		
		// if graphics mode is activated
		if (graphicsMode)
		{	
			// redraw the string that displays the current level
			gameWindow->fillRectangle(70, 20, 50, 20, Xwindow::White);
			gameWindow->drawString(15, 30, "             " + intToString(level));
		}
		return true;
	}
	return false;
}

// Clears rows that are filled up
void Board::canClear()
{
	// For every row 
	for (int i = 3; i < ROWS+3; i++)
	{
		bool shouldClear = 1;				// stores whether the current rows should be cleared
		for (int j = 0; j < COLS; j++)
		{
			// if there is a single non filled up cell in the current row
			if (grid[i][j].type == (char)32)
			{
				shouldClear = 0;			// we shouldn't clear the current row
				break;
			}
		}

		// if we should clear the current row
		if (shouldClear)
		{
			clearRow(i, 0);				// clear the current row
		}
	}	
}

// Updates the current score
void Board::updateScore(int lvl)
{
	cout << lvl << endl; 
	currScore += (lvl + 1)*(lvl + 1);			// increases the current score by the square of the current level + 1

	// if the current score is higher than our current high score
	if (currScore > highScore)
	{
		highScore = currScore;				// set the high score to be the current score

		// if graphics mode is activated
		if (graphicsMode)	
		{	
			// redraw the string that displays the high score
			gameWindow->fillRectangle(70, 80, 50, 20, Xwindow::White);
			gameWindow->drawString(15, 90, "             " + intToString(highScore));
		}
	}

	// if graphics mode is activated
	if (graphicsMode)
	{	
		// redraw the string that displays the current score
		gameWindow->fillRectangle(70, 50, 50, 20, Xwindow::White);
		gameWindow->drawString(15, 60, "             " + intToString(currScore));
	}
}

// Clears the given row
void Board::clearRow(int r, int cheat)
{
	// if we aren't using the cheat 'clear'
	if (!cheat)
	{
		updateScore(level);					// update the current score
	}
	
	locateShadow(shadowX, shadowY); // locates where the current block's shadow should be

	// For every row above the deleted row
	for (int i = r; i >= 3; --i)
	{
		for (int j = 0; j < COLS; j++)
		{
			// Set its information to that of the cell right above it (i.e. bump this row down)
			grid[i][j].type = grid[i-1][j].type;
			grid[i][j].levelGen = grid[i-1][j].levelGen;
			grid[i][j].blockID = grid[i-1][j].blockID;
		}
	}
	
	if (graphicsMode)						// if graphics mode is activated
	{
		int i;
		for (i = 0; i < usedCells.size(); i++)	// for every cell in the used cells vector
		{
			// if the cell is in a row that is above the deleted row
			if (usedCells[i].first.first < r)	
			{
				// undraw the cell
				usedCells[i].second.undrawCell(gameWindow, usedCells[i].first.second, usedCells[i].first.first); 
				usedCells[i].first.first++;	// bump the cell on row down
			}
			// if the cell is on the deleted row
			else if (usedCells[i].first.first == r)
			{
				// undraw the cell
				usedCells[i].second.undrawCell(gameWindow, usedCells[i].first.second, usedCells[i].first.first);
				usedCells.erase(usedCells.begin()+i);	// remove is from the used cells vector
				--i;
			}
		}
		
		// For every used cell
		for (int i = 0; i < usedCells.size(); i++)
		{
			// if the cell (after having been bumped down) is on or above the deleted row
			if (usedCells[i].first.first <= r)
			{
				// redraw the cell
				usedCells[i].second.drawCell(	gameWindow, 
										usedCells[i].first.second, 
										usedCells[i].first.first, 
										typeColors[usedCells[i].second.type]);
			}
		}
		
		// if the shadow is on or above the deleted row (i.e. the 4-by-4 grid containing the shadow is above the row)
		if (shadowX <= r - 3 && cheat)
		{
			undrawShadowBlock(0);      // then it has to be redrawn (as all the used cells were cleared)
		}
	}
	
	int tmpUsedID[MAXBLOCKS];		 // a temporary array storing the block ID's of the blocks after the row was deleted

	// Initialises the temporary block ID array to 0
	for (int i = 0; i < MAXBLOCKS; i++) 
	{
		tmpUsedID[i] = 0; 
	}

	// For every cell in our grid
	for (int i = 3; i < ROWS+3; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			// if the cell isn't empty
			if (grid[i][j].type != (char)32)
			{
				tmpUsedID[grid[i][j].blockID] = 1;	// the block ID is used
			}
		}
	}
	
	// For every block ID
	for (int i = 0; i < MAXBLOCKS; i++)
	{
		// if the block ID was used before deleting the row and if it isn't used
		// after deleting the row
		if (usedID[i] && !tmpUsedID[i])
		{
			// This means that the block with ID 'i' was completely deleted after the 
			// current row was deleted

			// if cheat mode isn't activated
			if (!cheatMode)
			{	
				updateScore(blockLevelGen[i]);	// update the current score
			}
			blockLevelGen[i] = -1;				// reset the level the block was generated
		}
		usedID[i] = tmpUsedID[i];				// updates the used ID array
	}
}

// Adds/removes the current block from/to the board
void Board::cutAndPasteCurrBlock(int mode)				// when mode = 0 we are pasting the current block onto the grid
												// when mode = 1 we are 'cutting' (i.e. removing) the current block from the grid
{
	// Get the current block's x and y coordinates
	int cx = currBlock->getX(), cy = currBlock->getY();

	if (mode == 0 && isMoveMadeLegal)					// redraws the current block iff the last move made
												// was a legal one, to ensure minimal redrawing
	{
		drawCurrBlock();
	}

	// For every cell in the 4-by-4 grid that stores the current block's configuration
	for (int i = cx; i < cx + 4; i++)
	{
		for (int j = cy; j < cy + 4; j++)
		{
			int px = i - cx, py = j - cy;

			// if this cell contains a portion of the block
			if (currBlock->piece[px][py])
			{
				// if we are pasting (i.e. adding) the current block to the board
				if (mode == 0)			
				{
					grid[i][j].type = currBlock->getType();	// set the cell's type to that of the block's
				}
				else
				{
					grid[i][j].type = (char)32;			// reset the cell's type to ' '
				}
			}
		}
	}
}

// Outputs the board onto the screen
ostream& operator<<(ostream &out, Board* b)
{
	// Output the game information onto to screen
	out << "Level:   \t" << b->level << endl;
	out << "Score:   \t" << b->currScore << endl;
	out << "Hi Score:\t" << b->highScore << endl;
	out << "----------" << endl;

	// paste the current block onto the board
	b->cutAndPasteCurrBlock(0);

	// For each cell on the board
	for (int i = 0; i < ROWS+3; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			out << (b->grid[i][j]).type;							// outputs the cell onto the screen
		}
		out << endl;
	}

	// cuts the current block from the board
	b->cutAndPasteCurrBlock(1);
	
	if (b->level != 0 && b->nextBlock->getType() == (char)32)			// if the next block is empty
	{
		b->nextBlock = new Block(b->genNextType(b->level, *(b->prng)), 0, 0, b->level);	// load next block
	}

	// Outputs the next block onto the screen
	out << "----------" << endl;
	out << "Next:" << endl;
	out << b->nextBlock;
	
	if (b->graphicsMode)										// if graphics mode is activated
	{
		b->nextBlock->drawBlock(b->nextBlockWindow, b->typeColors);		// draw the next block onto the window the displays the 
															// next block
	}
	return out;
}
