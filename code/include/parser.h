#ifndef __PARSER_H__
#define __PARSER_H__
#include "trie.h"
#include "board.h"
class Parser
{
	std::string* allCommands;					// contains all the Quadris commands (doesn't include help commands)
	
	std::string* cheatCommands;					// contains all the cheat commands 

	int numCommands;							// stores the number of commands stored by allCommands

	int numCheats;								// stores the number of commands stored by cheatCommands

	TrieNode* activeCommands;					// stores our commands

	public:

	Parser();									// ctor for Parser

	~Parser();								// dtor for Parser

	void parseMultiplier(int&, std::string&);		// breaks off the number (if any) from the beginning of a command

	void startGame(int, bool);					// starts the game

	void renameCommand (std::string, std::string);	// renames a built-in command to a user specified name (bonus feature)

	void listCommands(); 						// lists all the available commands (called when 'help' is entered)

	void commandManual(std::string);				// details the functionality of a given command (if it exists)

	void insertCheats();						// inserts the cheat commands into our trie 'activeCommands'

	void removeCheats();						// removes the cheat commands form our trie 'activeCommands'

	void beingSilly();							// outpus 'Now you're just being silly...' (called when user tries to
											// call redundant commands)
};

int stringToNum(std::string);						// converts a given string to a number (e.g. "123" -> 123)

#endif
