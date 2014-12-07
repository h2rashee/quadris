#include <iostream>
#include <fstream>
#include <string>
#include "include/trie.h"

using namespace std;

// checks whether a word is stored in the trie
bool hasWord (TrieNode& node)
{
	if (node.isWord) 		// you have found your word, so return true
	{
		return true;
	}
						// recursively search through each child
						// and check if either of them have a word
	for (int i = 0; i < 26; i++)
	{
		if (node.letters[i] != NULL && hasWord(*(node.letters[i])))
		{
			return true;
		}
	}
	return false;
}

// insert a word to your trie
TrieNode* insert (TrieNode* node, const string &word)
{
	if (!word.length())									 // if you've reached the end of your word
													 // make isWord true
	{
		node->isWord = true;
		return node;
	}
													// Otherwise insert everything from your 2nd 
													// letter on into the pointer to your first letter
	const string next = word.substr(1, word.length()-1);

	if (node->letters[(int)word[0]-(int)'a'] == NULL)
	{
		node->letters[(int)word[0]-(int)'a'] = new TrieNode;  // allocate space for the pointer to the first letter if its NULL
	}
	node->letters[(int)word[0]-(int)'a'] =  insert(node->letters[(int)word[0]-(int)'a'], next);
	return node;
}

// Remove a word from your trie by traversing through the given
//word in the trie and setting isWord to false in your last letter
TrieNode* remove (TrieNode* node, const string &word)
{
	if (!word.length())							 // if you've reached the end of your word
											 // set isWord to false (i.e. remove it)
	{
		node->isWord = false;
		return node;
	}
	if (node->letters[(int)word[0]-(int)'a'] != NULL) // if the pointer to your first letter isn't NULL
										     // recursively traverse down your word in the trie
	{
		node->letters[(int)word[0]-(int)'a'] = remove(node->letters[(int)word[0]-(int)'a'], word.substr(1, word.length()-1));
		return node;
	}
	else 									// otherwise return the trie itself
	{
		return node;
	}
}

// Searches for words that start with a given prefix in your trie
// by traversing through your prefix in trie and then returning all
// words in the trie from then on
string find (TrieNode* node, const string &word, string ans)
{
	if (!word.length())					 // if you have already traversed through the prefix
	{
		string ret2 = "";
									// if you are at the end of a word then 
									// add the word to your answer
		if (node->isWord)
		{
			ret2 = ",";
			bool done = 1;
			for (int i = 0; i < 26; i++) // if any of your pointers has a word 
								    // then add the current word to your answer
			{
				if (node->letters[i] != NULL && hasWord(*(node->letters[i])))
				{
					ret2 += ans;
					done = 0;
					break;
				}
			}
								    // If all the pointers are NULL then we have reached
								    // a leaf so return your answer
			if (done)
			{
				return ret2;
			}
		}
		// Recursively add all the words that can be 
		// completed from this point on in the trie
		for (int i = 0; i < 26; i++)
		{
			if (node->letters[i] != NULL && hasWord(*(node->letters[i]))) // Only recurse to letters that aren't NULL 
														    	  // and thath have a word somewhere down the trie
			{
				if (ret2[ret2.length()-1] == ',')
				{
					ret2 += ans;
				}
				ret2 += (char)(i+(int)'a') + find(node->letters[i], word, ans + (char)(i+(int)'a'));
			}
		}
		return ret2;
	}
	else if (node->letters[(int)word[0]-(int)'a'] == NULL) // if your next letter's pointer is NULL
												// then return "*" (to mark that you can't 
												// find words with the given prefix) 
	{
		return "*";
	}
	else 										// if we haven't traversed through the prefix yet
												// go to the next letter of the prefix
	{
		return word[0] + find(node->letters[(int)word[0]-(int)'a'], word.substr(1, word.length()-1), ans + word[0]);
	}
}
