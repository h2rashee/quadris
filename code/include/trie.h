#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>

struct TrieNode
{
    enum { NumChars = 26 };

    bool isWord;
    
    TrieNode *letters[NumChars];
    
    TrieNode()			// TrieNode constructor
    {
		isWord = false;
    
    		for ( int i = 0; i < NumChars; i += 1 )
	    	{
	          letters[i] = NULL;
   		}
    }
    
    ~TrieNode()  	    // TrieNode destructor
    				    // deletes trie by recursively deleting letters
  	{
  	   for (int i = 0; i < NumChars; i++)
	   {
	   	   delete letters[i];
	   }
    	}
}; 

bool hasWord (TrieNode&);								// checks whether the given trie contains a word			

TrieNode* insert (TrieNode*, const std::string&);				// inserts a word into the given trie

TrieNode* remove (TrieNode*, const std::string&);				// removes a word from the given trie

std::string find (TrieNode*, const std::string&, std::string);	// autocompletes a given prefix with words from
													// the given trie
#endif

