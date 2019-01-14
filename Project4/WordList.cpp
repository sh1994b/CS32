#include "provided.h"
#include <string>
#include <vector>
#include "MyHash.h"
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
	~WordListImpl();
private:
	MyHash<string, vector<string>> m_hashtable;
	MyHash<string, int> m_wordsHash;
	string generatePattern(const string& word) const;
	bool wordIsValid(string& word, char otherChar = 'A') const;
	void convertToLower(string& word);
};

bool WordListImpl::loadWordList(string dictFilename)
{
	m_hashtable.reset();    //discard old contents
	string word = "";
	ifstream infile(dictFilename);
	if (!infile)    //if opening the file failed
		return false;
	else
	{
		while (getline(infile, word))
		{
			//make sure word doesn't include non-letter and non-':
			if (wordIsValid(word))
			{
				convertToLower(word);       //make word be all lowercase
				m_wordsHash.associate(word, 0);     //add word with dummy value to m_wordHash
													//to add pattern and word in its vector to m_hashtable:
				string pattern = generatePattern(word);
				vector <string>* wordsVector = m_hashtable.find(pattern); //get a pointer to vector of matching words
				if (wordsVector != nullptr)     //node including pattern exists
					wordsVector->push_back(word);
				else    //pattern doesn't exist in hashtable
				{
					vector <string> newVector;
					newVector.push_back(word);
					m_hashtable.associate(pattern, newVector);
				}
			}
			word = "";
		}
		return true;
	}
}

bool WordListImpl::contains(string word) const
{
	for (unsigned int i = 0; i < word.length(); i++)
		word[i] = tolower(word[i]);     //first convert to lowercase
	const int* valPtr = m_wordsHash.find(word);
	if (valPtr == nullptr)      //word doesn't exist
		return false;
	return true;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	//check for validity
	if (!wordIsValid(cipherWord) || !wordIsValid(currTranslation, '?'))
		return vector<string>();
	if (cipherWord.length() != currTranslation.length())
		return vector<string>();
	for (unsigned int i = 0; i < cipherWord.length(); i++)
		cipherWord[i] = tolower(cipherWord[i]);     //first convert cipherword to lowercase
	string pattern = generatePattern(cipherWord);   //get pattern of cipherWord
	const vector <string>* vPtr = m_hashtable.find(pattern);  //get a pointer to vector of matching words
	if (vPtr == nullptr)        //pattern doesn't exist
		return vector<string>();

	vector <string> candidates;
	bool goodWord = 1;
	vector <string> wordsVector = *vPtr;
	for (vector <string>::iterator p = wordsVector.begin(); p != wordsVector.end(); p++)  //for each word with that pattern
	{
		goodWord = 1;
		string w = *p;
		for (unsigned int j = 0; j < currTranslation.length(); j++)
		{
			currTranslation[j] = tolower(currTranslation[j]);
			if (currTranslation[j] >= 'a' && currTranslation[j] <= 'z')       //is a letter
			{
				if (cipherWord[j] < 'a' || cipherWord[j] > 'z')
					return vector<string>();
				if (w[j] != currTranslation[j])
				{
					goodWord = 0;
					break;
				}
			}
			if (currTranslation[j] == '?')
			{
				if (cipherWord[j] < 'a' || cipherWord[j] > 'z')
					return vector<string>();
				if (w[j] < 'a' || w[j] > 'z')
				{
					goodWord = 0;
					break;
				}
			}
			if (currTranslation[j] == 39)
			{
				if (cipherWord[j] != 39)
					return vector<string>();
				if (w[j] != 39)
				{
					goodWord = 0;
					break;
				}
			}
		}
		if (goodWord)
			candidates.push_back(w);
	}
	return candidates;
}


WordListImpl::~WordListImpl()
{
	//calling delete on MyHash nodes will take care of
	//deleting dynamically allocated vectors as values
	//=> don't need to implement destructor anymore
}

//takes a word and generates a pattern for it
string WordListImpl::generatePattern(const string& word) const
{
	if (word.size() == 0)
		return word;
	string pattern = "a";
	char patternMaker = 'b';
	bool charsEqual;

	for (unsigned int i = 1; i < word.size(); i++)
	{
		if (word[i] == 39)		//if ' just add it
			pattern += 39;
		else
		{
			charsEqual = false;
			for (unsigned int j = 0; j < i; j++)
			{
				if (word[i] == word[j])
				{
					charsEqual = true;
					pattern += pattern[j];
					break;
				}
			}
			if (!charsEqual)
			{
				pattern += patternMaker;
				patternMaker++;
			}
		}
	}
	return pattern;
}

//if a character is not a letter or an apostrophe it returns false
//otherwise the word is valid and it returns true
bool WordListImpl::wordIsValid(string& word, char otherChar) const
{
	unsigned int i;
	for (i = 0; i < word.length(); i++)
		if ((word[i] >= 'A' && word[i] <= 'Z') ||
			(word[i] >= 'a' && word[i] <= 'z') ||
			word[i] == 39 || word[i] == otherChar)
		{
			;
		}
	if (i == word.length())
		return true;
	return false;

}

//takes a word and turns it into all lowercase letters
void WordListImpl::convertToLower(string& word)
{
	for (unsigned int i = 0; i < word.length(); i++)
		word[i] = tolower(word[i]);
}






//***** hash functions for string, int, and char *****

unsigned int myHash(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int myHash(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int myHash(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
	m_impl = new WordListImpl;
}

WordList::~WordList()
{
	delete m_impl;
}

bool WordList::loadWordList(string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
	return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
	return m_impl->findCandidates(cipherWord, currTranslation);
}