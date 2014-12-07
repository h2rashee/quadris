/*

Authors: Amlesh Jayakumar & Harris Rasheed
Date: Monday 5th December 2011
CS 246 Assignment 5 Project
Quadris

*/

#include <iostream>
#include "include/parser.h"

using namespace std;

//Main function
int main(int argv, char* argc[])
{
	int seed = 0;							//The default seed is zero for the program
	bool graphicsMode = 1;					//By default, the program runs with graphics
	string s;								//Stores the current command

	for (int i = 1; i < argv; i++)			//Cycles through the options passed into the program
	{
		s = argc[i];

		if (s == "-seed")					//If the -seed option is passed
		{
			
			if(argv <= (i+1) || argc[i+1] == "-text")		//If two few arguments are supplied or no seed value is given
			{
				cerr << "A seed value was not passed" << endl;
				return 0;
			}

			else
			{
				seed = stringToNum(argc[i+1]);	//Converts the given seed to a string
			}
		}

		else if (s == "-text")				//If the -text option is passed
		{
			graphicsMode = 0;				//Disables the graphics flag
		}
	}

	Parser *p = new Parser;					//Allocates space for the Parser class
	p->startGame(seed, graphicsMode);			//Starts the Quadris game
	delete p;								//Deallocates the Parser class from memory

	return 0;
}
