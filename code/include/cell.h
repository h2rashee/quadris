#ifndef __CELL_H__
#define __CELL_H__
#include "window.h"
class Cell
{
	public:

	char type;							// Sets the type of the cell to be of the character it represents
	int levelGen;							// The level that the block was generated in
	int blockID;							// The ID number of the block that the cell represents

	Cell();								// Cell constructor
	Cell(char, int, int);					// Cell constructor
	~Cell();								// Cell destructor

	void drawCell(Xwindow*, int, int, int);		// draws the cell
	void undrawCell(Xwindow*, int, int);	 	// undraws the cell
};
#endif
