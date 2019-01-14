#include "provided.h"
#include "MyHash.h"
#include <string>
#include <iostream>
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	bool pushMapping(string ciphertext, string plaintext);
	bool popMapping();
	string getTranslation(const string& ciphertext) const;
	~TranslatorImpl();
private:
	struct Node
	{
		char m_mapping[26];
		Node* m_next;
	};
	Node* m_head;
	void push(char mapping[]);
	void pop();
	int index(const char& c);
	int popMapCalled;
	int pushMapCalled;
	char currMapping[26];
};

TranslatorImpl::TranslatorImpl()
	:popMapCalled(0), pushMapCalled(0)
{
	m_head = nullptr;
	for (int i = 0; i < 26; i++)
		currMapping[i] = '?';
	push(currMapping);      //****should I do this? will it mess with pushMapCalled?
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	if (ciphertext.length() != plaintext.length())
		return false;
	bool isInvalid = 0;
	char tempMapping[26] = {};

	for (unsigned int j = 0; j < ciphertext.length(); j++)
	{
		ciphertext[j] = toupper(ciphertext[j]);
		plaintext[j] = toupper(plaintext[j]);
		if (ciphertext[j]<'A' || ciphertext[j]>'Z'
			|| plaintext[j] < 'A' || plaintext[j] > 'Z')  //if non letter
		{
			isInvalid = 1;
			break;
		}
		int charIndex = index(ciphertext[j]);
		//if incosistent with the current mapping:
		if (currMapping[charIndex] != '?' && currMapping[charIndex] != plaintext[j])
		{
			isInvalid = 1;
			break;
		}
		//if more than one letters map to same letter
		for (int k = 0; k < 26; k++)
		{
			if (plaintext[j] == currMapping[k] && plaintext[j] != currMapping[ciphertext[j]-'A'])
			{
				isInvalid = 1;
				break;
			}
		}
		tempMapping[charIndex] = plaintext[j];  //add mapping to the map
	}
	if (isInvalid)
		return false;
	for (int i = 0; i < 26; i++)
	{
		if (tempMapping[i] >= 'A' && tempMapping[i] <= 'Z')
			currMapping[i] = tempMapping[i];
	}
	push(currMapping);
	pushMapCalled++;

	return true;
}

bool TranslatorImpl::popMapping()
{
	if (pushMapCalled == popMapCalled)
		return false;
	pop();
	for (int i = 0; i < 26; i++)
		currMapping[i] = m_head->m_mapping[i];
	popMapCalled++;
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string translated = "";
	for (unsigned int i = 0; i < ciphertext.size(); i++)
	{
		if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') //if character is a lowrcase letter
			translated += tolower(currMapping[ciphertext[i] - 'a']);
		else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z')    //if character is a uppercase letter
			translated += currMapping[ciphertext[i] - 'A'];
		else    //character isn't a letter
			translated += ciphertext[i];
	}
	return translated;
}

void TranslatorImpl::push(char mapping[])
{
	Node* newNode = new Node;
	for (int i = 0; i < 26; i++)
		newNode->m_mapping[i] = mapping[i];
	newNode->m_next = m_head;
	m_head = newNode;
}

void TranslatorImpl::pop()
{
	Node* p = m_head;
	if (p != nullptr)
	{
		m_head = p->m_next;
		delete p;
	}
}

TranslatorImpl::~TranslatorImpl()
{
	Node* p = m_head;

	while (p != nullptr)
	{
		m_head = p->m_next;
		delete p;
		p = m_head;
	}

}


int TranslatorImpl::index(const char& c)
{
	return c - 'A';
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
	m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
	delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
	return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
	return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
	return m_impl->getTranslation(ciphertext);
}