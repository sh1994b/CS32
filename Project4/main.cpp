//************haven't tested findCandidates
#include <iostream>
#include "provided.h"
#include "MyHash.h"
using namespace std;

int main()
{
	Decrypter d;
	string shoo = "Lzdkgd	dyrmjls	shcg	xdggkud	fpm	xd!!";
	if (d.load("wordlist.txt"))
	{
		vector<string> translation = d.crack(shoo);
		for (unsigned int i = 0; i < translation.size(); i++)
			cout << translation[i] << endl;
	}

}
