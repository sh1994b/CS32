#include <string>
#include <vector>
#include <iostream>
#include "provided.h"
using namespace std;


class TokenizerImpl
{
public:
	TokenizerImpl(string separators);
	vector<string> tokenize(const string& s) const;
private:
	std::vector <char> m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	for (unsigned int i = 0; i < separators.size(); i++)
		m_separators.push_back(separators[i]);
}

vector<string> TokenizerImpl::tokenize(const string& s) const
{
	std::vector<std::string> strings;
	std::string currWord = "";
	char c;
	bool isSeparator;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		isSeparator = 0;
		c = s[i];
		for (unsigned int j = 0; j < m_separators.size(); j++)
			if (c == m_separators[j] || 
				c >= 48 && c <= 57 || c == 9)		//if char is a separator or a digit or a space or a tab
			{
				isSeparator = 1;
				break;
			}
		if (!isSeparator)
			currWord += c;

		else
		{
			if (currWord != "")
				strings.push_back(currWord);
			currWord = "";
		}

	}
	if (currWord != "")
		strings.push_back(currWord);	//add last word in case there was no separator at the end

	return strings;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
	return m_impl->tokenize(s);
}
//****************************************************************************





/*
int main()
{
	//test Tokenizer
	std::string s = "Shab!. Matt    !!!   Poo....";
	Tokenizer t(" !.");
	std::vector <std::string> v = t.tokenize(s);
	cout << "size of v: " << v.size() << endl;
	for (unsigned int i = 0; i < v.size(); i++)
		std::cout << v[i] << std::endl;
}
*/