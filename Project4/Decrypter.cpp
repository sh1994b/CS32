#include "provided.h"
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
	DecrypterImpl();
	bool load(string filename);
	vector<string> crack(const string& ciphertext);
private:
	WordList m_wordList;
	Tokenizer m_tokenizer;
	Translator m_translator;
	bool isValid(const string& word) const;
	string getLongestUntranslated(const vector<string>& words) const;
	bool aTransWordNotLegit(string cipherTranslated);
	bool isFullyTranslated(string cipherTranslated);
};

DecrypterImpl::DecrypterImpl()
	:m_tokenizer(", ;:.!()[]{}-\"#$%^&")
{
}

bool DecrypterImpl::load(string filename)
{
	return m_wordList.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	if (!isValid(ciphertext))
		return vector<string>();
	vector<string> results;
	string possibleTranslation;
	vector<string> tokenized;
	tokenized = m_tokenizer.tokenize(ciphertext);
	//get word with most untranslated letters:
	string useThisWord = getLongestUntranslated(tokenized);
	if (useThisWord == "")
		return vector<string>();
	//translate the word using the current mapping:
	string currWordTranslated = m_translator.getTranslation(useThisWord);
	//find candidates for the current word's translation:
	vector<string> currCandidates = m_wordList.findCandidates(useThisWord, currWordTranslated);
	if (currCandidates.empty())
	{
		m_translator.popMapping();
		return currCandidates;
	}

	for (vector<string>::iterator q = currCandidates.begin(); q != currCandidates.end(); q++)	//for each candidate
	{
		//create a temporary mapping table:
		if (m_translator.pushMapping(useThisWord, *q))	//couldn't push mapping for ah
		{
			string cipherTranslated = m_translator.getTranslation(ciphertext);
			if (aTransWordNotLegit(cipherTranslated))	//if there's at least 1 fully translated word that's not in word list
				m_translator.popMapping();
			else if (!isFullyTranslated(cipherTranslated))		//if ciphertext isn't fully translated
			{
				vector<string> recursiveCallResults = crack(ciphertext);
				for (unsigned int i = 0; i < recursiveCallResults.size(); i++)
					results.push_back(recursiveCallResults[i]);		//add results from recurvise call into this call's results
			}
			else
			{
				results.push_back(cipherTranslated);	//add this translation to vector of results
				m_translator.popMapping();
			}
		}
	}
	m_translator.popMapping();
	sort(results.begin(), results.end());
	return results;
}


bool DecrypterImpl::isValid(const string& word) const
{
	unsigned int i;
	for (i = 0; i < word.length(); i++)
		if ((word[i] >= 'A' && word[i] <= 'Z') ||
			(word[i] >= 'a' && word[i] <= 'z') ||
			word[i] == 39 || (word[i] >= 33 && word[i] <= 38)
			|| word[i] == 40 || word[i] == 41 || word[i] == 44
			|| word[i] == 46 || word[i] == 58 || word[i] == 59
			|| word[i] == 45 || word[i] == 94 || word[i] == 91
			|| word[i] == 93 || word[i] == 123 || word[i] == 125)
		{
			;
		}
	if (i == word.length())
		return true;
	return false;
}

//gets a vector of the tokenized ciphertext words
//translates them and returns the cipherword with most number of ?'s
string DecrypterImpl::getLongestUntranslated(const vector<string>& words) const
{
	if (words.size() == 0)
		return "";
	vector<string> translated;
	//get translation for each one of the cipherwords from ciphertext
	for (unsigned int i = 0; i < words.size(); i++)
		translated.push_back(m_translator.getTranslation(words[i]));

	vector<int> numUntranslatedChars;
	for (unsigned int i = 0; i < words.size(); i++)
	{
		numUntranslatedChars.push_back(0);
		for (unsigned int j = 0; j < translated[i].size(); j++)  //look at each character of each word
			if ((translated[i])[j] == '?')
				numUntranslatedChars[i]++;
	}
	int maxUntranslated = numUntranslatedChars[0];
	for (unsigned int i = 0; i < numUntranslatedChars.size(); i++)
		if (numUntranslatedChars[i] > maxUntranslated)
			maxUntranslated = numUntranslatedChars[i];
	for (unsigned int i = 0; i < numUntranslatedChars.size(); i++)
		if (numUntranslatedChars[i] == maxUntranslated)
			return words[i];
	return "";
}

bool DecrypterImpl::aTransWordNotLegit(string cipherTranslated)
{
	vector<string> words = m_tokenizer.tokenize(cipherTranslated);
	unsigned int i;
	for (i = 0; i < words.size(); i++)	//go thru each word in vector
	{
		unsigned int j;
		for (j = 0; j < words[i].size(); j++)	//go thru each letter of word
			if ((words[i])[j] == '?')
				break;
		if (j == words[i].size())	//word is fully translated
			if (!m_wordList.contains(words[i]))
				break;
	}
	if (i == words.size())	//none of translated words were contained in word list
		return false;
	return true;
}

bool DecrypterImpl::isFullyTranslated(string cipherTranslated)
	{
	unsigned int i;
	for (i = 0; i < cipherTranslated.size(); i++)
		if (cipherTranslated[i] == '?')
			break;
	if (i == cipherTranslated.size())
		return true;
	return false;
	}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
	delete m_impl;
}

bool Decrypter::load(string filename)
{
	return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
	return m_impl->crack(ciphertext);
}