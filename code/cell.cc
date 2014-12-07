#include "include/cell.h"

// Cell constructor 1:
// This constructor is used for when there is no character in the space
Cell::Cell()
{
	type = (char)32;
	levelGen = blockID = -1;
}

// Cell constructor 2:
// This constructor is used for a space that is occupied by a character
Cell::Cell(char t, int lvl, int ID)
{
	type = t;
	levelGen = lvl;
	blockID = ID;
}

// Cell destructor
Cell::~Cell()
{
}

// Cell draw function
void Cell::drawCell(Xwindow* w, int x, int y, int c)
{
	w->fillRectangle(25 * x + 125 + 1, 25 * y + 125 + 1, 23, 23, c);
	// Draws a cell in the 25x25 celled grid that is situated 125 pixels
	// from the top of the screen as a 23x23 square with the colour
	// represented by the enumeration c
}

//Cell undraw function
void Cell::undrawCell(Xwindow* w, int x, int y)
{
	w->fillRectangle(25 * x + 125 + 1, 25 * y + 125 + 1, 23, 23, 0);
	// Draws a cell in the 25x25 celled grid that is situated 125 pixels
	// from the top of the screen as a 23x23 white square
}
