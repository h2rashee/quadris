#include <iostream>
#include <string>
#include "include/parser.h"

using namespace std;

// Parser Constructor
Parser::Parser()
{
	activeCommands = new TrieNode;			// Stores active commands because cheat mode determines whether cheats are active
	
	numCheats = 3;							// Three cheats are built-in
	cheatCommands = new string[numCheats];

	cheatCommands[0] = "clear";				// Cheat commands
	cheatCommands[1] = "boom";
	cheatCommands[2] = "up";

	numCommands = 13;
	allCommands = new string[numCommands+numCheats];

	allCommands[0] = "left";					// Regular standard commands
	allCommands[1] = "right";
	allCommands[2] = "down";
	allCommands[3] = "clockwise";
	allCommands[4] = "counterclockwise";
	allCommands[5] = "drop";
	allCommands[6] = "levelup";
	allCommands[7] = "leveldown";
	allCommands[8] = "restart";
	allCommands[9] = "cheats";
	allCommands[10] = "rename";
	allCommands[11] = "help";
	allCommands[12] = "man";
	allCommands[13] = "clear";
	allCommands[14] = "boom";
	allCommands[15] = "up";

	for (int i = 0; i < numCommands; i++)		// Inserts active commands into the trie so that they can be processed for short prefixes
	{
		activeCommands = insert(activeCommands, allCommands[i]);
	}
}

// Paresr Destructor
Parser::~Parser()
{
	delete [] allCommands;
	delete [] cheatCommands;
	delete activeCommands;
}

// Converts a string to an integer
int stringToNum(string s)
{
	int num = 0;

	for (int i = 0; i < s.length(); i++)		//Goes through each digit in the string to convert it
	{
		num = num*10 + (s[i] - '0');
	}

	return num;
}

// Lists all the active commands in the directory
void Parser::listCommands()
{
	cout << "List of all available commands:" << endl;

	for (int i = 0; i < numCommands; i++)
	{
		cout << "- " << allCommands[i] << endl;
	}

	cout << endl;
	cout << "Multipliers can be enjoined to commands as prefixes ";
	cout << "to perform the same action multiple times." << endl;

	cout << endl;
	cout << "The 'man [COMMANDNAME]' can be used to find out more ";
	cout << "about a specific command name" << endl;
	cout << endl;
}

void Parser::commandManual(string cmd)
{
	if (cmd == allCommands[0])				//'left' command
	{
		cout << "Default Command Name: left" << endl;
		cout << "Moves the current block one cell to the left. If this is not possible (left " << endl;
		cout << "edge of the board, or block in the way), the command has no effect." << endl;
	}
	else if (cmd == allCommands[1])			//'right' command
	{
		cout << "Default Command Name: right" << endl;
		cout << "Moves the current block one cell to the right. If this is not possible (right " << endl;
		cout << "edge of the board, or block in the way), the command has no effect." << endl;
	}		
	else if (cmd == allCommands[2])			//'down' command
	{
		cout << "Default Command Name: down" << endl;
		cout << "Moves the current block one cell down. If this is not possible (bottom edge " << endl;
		cout << "of the board, or block in the way), the command has no effect." << endl;
	}
	else if (cmd == allCommands[3])			//'clockwise' command
	{	
		cout << "Default Command Name: clockwise" << endl;
		cout << "Rotates the block 90 degrees clockwise about the bottom left corner. If the " << endl;
		cout << "rotation cannot be accomplished without coming into contact with existing " << endl;
		cout << "blocks or going outside the game board, the command has no effect." << endl;
	}
	else if (cmd == allCommands[4])			//'counterclockwise' command
	{
		cout << "Default Command Name: counterclockwise" << endl;
		cout << "Rotates the block 90 degrees counterclockwise about the bottom left corner. ";
		cout << "If the rotation cannot be accomplished without coming into contact with existing blocks ";
		cout << "existing blocks or going outside the game board, the command has no effect." << endl;
	}
	else if (cmd == allCommands[5])			//'drop' command
	{	
		cout << "Default Command Name: drop" << endl;
		cout << "Drops the current block down until it one of its bottom cells rests on an " << endl;
		cout << "existing block or on the bottom edge of the board. The command also triggers " << endl;
		cout << "the next block to appear on the game board and generates a new next block" << endl;
		cout << "which is displayed in the appropriate place." << endl;
	}
	else if (cmd == allCommands[6])			//'levelup' command
	{	
		cout << "Default Command Name: levelup" << endl;
		cout << "Increases the difficulty level of the game by one." << endl;
		cout << "This results in the blocks being generated after the next block to follow " << endl;
		cout << "the selection process associated with the higher level. If there is no" << endl;
		cout << "higher level, this command has no effect." << endl;
	}
	else if (cmd == allCommands[7])			//'leveldown' command
	{
		cout << "Default Command Name: leveldown" << endl;
		cout << "Decreases the difficulty level of the game by one." << endl;
		cout << "This results in the blocks being generated after the next block to follow " << endl;
		cout << "the selection process associated with the lower level. If there is no lower " << endl;
		cout << "level, this command has no effect." << endl;
	}
	else if (cmd == allCommands[8])			//'restart' command
	{
		cout << "Default Command Name: restart" << endl;
		cout << "Resets the board in both graphics mode and text mode such that there are no " << endl;
		cout << "blocks set upon the board. The block appearing on the screen is the current " << endl;
		cout << "block that has to be generated. The current score is reset but the high score " << endl;
		cout << "is retained and a new game is started. The seed is reset so that ir generates " << endl;
		cout << "the same combination of blocks. Multipliers cannot be applied to this command." << endl;
	}
	else if (cmd == allCommands[9])			//'cheats' command
	{
		cout << "Default Command Name: cheats" << endl;
		cout << "Activates/deactivates cheat modes and allows commands that go against the " << endl;
		cout << "spirit of the game Quadris. For the complete listing of commands, activate " << endl;
		cout << "cheat mode, type 'help' and 'man' the specific command you would like to " << endl;
		cout << "know more about." << endl;
	}
	else if (cmd == allCommands[10])			//'rename' command
	{
		cout << "Command Name: rename" << endl;
		cout << "Changes the name of a command to the given specific parameter. This command " << endl;
		cout << "takes in two parameters. The first parameter is the current command name " << endl;
		cout << "and the seocnd parameter is the new desired command name. The new command " << endl;
		cout << "name should not be renamed to a current existing command. Multipliers cannot " << endl;
		cout << "be applied to this command." << endl;
	}
	else if (cmd == allCommands[11])			//'help' command
	{
		cout << "Command Name: help" << endl;
		cout << "Displays all available commands at the time which alters depending on whether " << endl;
		cout << "cheat mode is activated or not. Multipliers cannot be applied ot this command." << endl;
	}
	else if (cmd == cheatCommands[0])			//'clear' command
	{	
		cout << "Default Command Name: clear" << endl;
		cout << "Clears a row's blocks given the first parameter as a number of the row from the " << endl;
		cout << "bottom. This command does not grant the user any points for using this command." << endl;
	}
	else if (cmd == cheatCommands[1])			//'boom' command
	{
		cout << "Default Command Name: boom" << endl;
		cout << "Clears all the blocks that have been set on the screen (all the blocks but the " << endl;
		cout << "current block) but it however retains the current score. This allows users who " << endl;
		cout << "are near a possible gameover scenario to reset the board and keep playing for a" << endl;
		cout << " theoretical infinite amount of time." << endl;
	}
	else if (cmd == cheatCommands[2])			//'up' command
	{	
		cout << "Default Command Name: up" << endl;
		cout << "Moves the current block one cell up." << endl;
		cout << "If this is not possible (top of the board, or block in the way), the command has no effect." << endl;
	}
	else if (cmd == allCommands[12])					//'man' command
	{
		beingSilly();
	}
	else
	{
		cerr << "The command does not exist." << endl;
		cerr << "Please ensure that you use the format man [COMMANDNAME] e.g. man drop" << endl;
	}
	cout << endl;
}

//Parses the multiplier out of the command input
void Parser::parseMultiplier(int &mult, string &s)
{
	int commandPos = 0;			// Position where the actual command starts
	string tmp = "";

	// Loops through the string and 
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			tmp += s[i];		// String concatenates the number to the string
		}
		else
		{
			commandPos = i;	// Sets the command position to be at the ith position
			break;
		}
	}

	// If the entered command (after the multiplier prefix) contains a character
	// that isn't a lower case alphabet
	for (int i = commandPos; i < s.length(); i++)
	{
		if (s[i] < 'a' || s[i] > 'z')
		{
			s = "";			// then set to "" (so that it can be caught later on in startGame
							// as an invalid command)
			return;
		}
	}

	if (tmp == "")			//If there is no multiplier
	{
		return;
	}

	if (commandPos != 0)		// If there was a multiplier at the beginning of the command
	{
		s = s.substr(commandPos, s.length()-commandPos);			//Takes a substring 
		mult = stringToNum(tmp);								//Sets the multiplier to be the number in front of the block
	}
	else
	{
		s = "";
	}
}

//Inserts the cheat commands in the trie to make them available for use
void Parser::insertCheats()
{
	for (int i = 0; i < numCheats; i++)
	{
		activeCommands = insert(activeCommands, cheatCommands[i]);	//Adds the cheat commands to the trie
	}
	numCommands += numCheats;								//Increases the number of commands value
}

//Removes the cheat commands from the trie to make them unavailable for use
void Parser::removeCheats()
{
	for (int i = 0; i < numCheats; i++)
	{
		activeCommands = remove(activeCommands, cheatCommands[i]);	//Removes the cheat commands from the trie
	}
	numCommands -= numCheats;								//Decreases the number of commands value
}

//Displays a comical message if the user attempts to do something silly
void Parser::beingSilly()
{
	cout << "Now you are just being silly..." << endl;	
}

//Renames a command to a lowercase alphabet consisting string the user specifies
void Parser::renameCommand(string command, string name)
{
	if (command == allCommands[10])		//Trying to rename the rename command
	{
		beingSilly();					//Output a being silly message
		cout << endl;
		return;
	}

	bool found = 0;					//Stores whether the command being renamed exists
	bool nameInUse = 0;					//Stores whether the new command name is already in use

	//Cycles through the commands currently in use
	for (int i = 0; i < numCommands; i++)
	{
		if (allCommands[i] == name)		//Checks if the command's intended new name is already in use
		{
			nameInUse = 1;
		}
	}

	if(nameInUse)						//Trying to rename a command to one that already exists
	{
		cout << name << " is an existing command." << endl;
		cout << "Try renaming " << command << " to a non-existing command" << endl;
		cout << endl;
		return;
	}

	//Cycles through each character in name
	for (int i = 0; i < name.length(); i++)
	{
		if (name[i] < 'a' || name[i] > 'z')	//If the name consists of characters that are non-lowercase alphabets
		{
			cout << "Rename is invalid." << endl;
			cout << "Commands should only consist of lowercase alphabets" << endl;
			cout << endl;
			return;
		}
	}

	string search = find(activeCommands, name, "");

	//Cycles through all the commands
	for (int i = 0; i < numCommands; i++)
	{
		if (allCommands[i] == command)		//If the command being named is found
		{
			found = 1;

			//If the name is not in use, greater than length zero and is unambiguous
			if(!nameInUse && (!search.length() || search[search.length()-1] != ','))
			{
				allCommands[i] = name;		//Renames the current command name to the intended new name
			}
		}
	}

	//The command is being renamed to an ambiguous prefix that clashes with an existing command
	if (search.length() > 0 && search[search.length()-1] == ',')
	{
		cout << name << " is the prefix of an existing command" << endl;
		cout << "Try renaming " << command << " to something that's not a";
		cout << " prefix of an existing command" << endl;
		cout << endl;
		return;
	}

	//If the command attempting to be renamed is not found
	if (!found)
	{
		cout << "Command '" << command << "' is not found" << endl;
		cout << "Remember to type out the full command name" <<  endl;
		cout << endl;
		return;
	}

	//If the user is attempting to rename a command to its current name
	if (command == name)
	{
		beingSilly();					//Output a being silly message
		cout << endl;
		return;
	}

	activeCommands = remove(activeCommands, command);			//Removes the old command name from the Trie
	activeCommands = insert(activeCommands, name);			//Inserts the new command name into the Trie

	cout << command << " successfully renamed to " << name << endl;		//Outputs command succesfully renamed message
	cout << endl;
}

//Starts the game with the parameters seed for PRNG and whether the game is to be in graphics mode
void Parser::startGame(int seed, bool graphicsMode)
{
	string input, command, name, cmdFound;
	int mult = 1;
	bool isMoveMade, lastMoveLegal;

	Board *quadris = new Board(seed, graphicsMode);	//Initialises a Board

	cout << quadris;

	//While there are commands in the input stream
	while(cin >> input)
	{
		parseMultiplier(mult, input);			//Parses the multiplier out of the command name if there is one
		
		if (input != "")								//If there is a non-empty command
		{
			cmdFound = find(activeCommands, input, "");		//Search the trie
		}

		//If the command is empty, the command is not found or the command is ambiguous
		if (input == "" || cmdFound.length() == 0 || cmdFound[cmdFound.length()-1] != ',')
		{
			cout << "Command is not found." << endl;
			cout << "Use the 'help' command for a list of available commands." << endl;
			cout << endl;
			continue;
		}

		cmdFound = cmdFound.substr(0, cmdFound.length()-1);		//Removes the comma at the end of the string

		if (cmdFound.find(',') != string::npos)					//If there are less than one commands
		{
			cout << "Command is ambiguous" << endl;
			cout << "Here are the matching commands:" << endl;
			cout << cmdFound << endl;
			cout << "Use 'man [COMMANDNAME]' command to find out more about a specific command." << endl;
			cout << endl;
		}
		else
		{
			isMoveMade = 0;									// Was a move made in the last command
			for (int i = 0; i < mult; i++)
			{
				lastMoveLegal = 0;								// Was the last command legal

				// lastMoveLegal is bitwise 'or'd with any move command (e.g. left) to check
				// if that move was legal

				if (cmdFound == allCommands[0])					//Left command
				{
					lastMoveLegal |= quadris->left(i);
				}
				else if (cmdFound == allCommands[1])				//Right command
				{
					lastMoveLegal |= quadris->right(i);
				}
				else if (cmdFound == allCommands[2])				//Down command
				{
					lastMoveLegal |= quadris->down(i);
				}
				else if (cmdFound == allCommands[3])				//Clockwise command
				{
					lastMoveLegal |= quadris->rotate(1, i);
				}
				else if (cmdFound == allCommands[4])				//Counterclockwise command
				{
					lastMoveLegal |= quadris->rotate(0, i);
				}
				else if (cmdFound == allCommands[5])				//Drop command
				{
					lastMoveLegal |= quadris->drop();
				}
				else if (cmdFound == allCommands[6])				//Level Up command
				{
					lastMoveLegal |= quadris->levelUp();
				}
				else if (cmdFound == allCommands[7])				//Level Down command
				{
					lastMoveLegal |= quadris->levelDown();
				}
				else if (cmdFound == allCommands[8])				//Restart command
				{
					if (quadris->cheatMode)
					{
						removeCheats();
					}

					quadris->restartGame();

					isMoveMade = 1;

					break;
				}
				else if (cmdFound == allCommands[9])				//Cheats command
				{
					quadris->toggleCheats();

					if (quadris->cheatMode)
					{
						cout << "Cheat mode activated" << endl;
						insertCheats();
					}
					else
					{
						cout << "Cheat mode deactivated" << endl;
						removeCheats();
					}
					
					cout << endl;

					goto next;
				}
				else if (cmdFound == allCommands[10])				// Rename command
				{
					cin >> command >> name;
					renameCommand(command, name);
					goto next;
				}
				else if (cmdFound == allCommands[11])				// Help command
				{
					listCommands();
					goto next;
				}
				else if (cmdFound == allCommands[12])				// Man command
				{
					cin >> command;
					commandManual(command);
					goto next;
				}
				if (numCommands > 13)									//If cheat mode is activated
				{
					if (cmdFound == allCommands[13])						//Clear command
					{
						int r;
						cin >> r;										//Read in row number to clear
						quadris->clearRow(18-r, 1);
					}
					else if (cmdFound == allCommands[14])					//Boom command
					{
						quadris->boom();
					}
					else if (cmdFound == allCommands[15])					//Up command
					{
						lastMoveLegal |= quadris->up(i);		
					}
				}

				isMoveMade = isMoveMade | lastMoveLegal;					// If last move was legal, then a move was made

				if (!lastMoveLegal)										// If the last move was not legal
				{
					break;								//Break out of the loop as future iterations are not legal
				}
			}

			quadris->isMoveMadeLegal = isMoveMade;		// Sets whether the last move was legal

			if (quadris->gameOver())					//If it is a game over situation
			{
				cout << "GAME OVER" << endl;			//Output game over
				cout << endl;
				quadris->restartGame();				//Restart game
			}
			cout << quadris;

			next:;

			mult = 1;							//The multiplier is reset to one
		}
	}

	delete quadris;		//Deletes the Board when the input stream is exhausted
}
