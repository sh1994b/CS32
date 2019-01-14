#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;
	
int main()
{

	Map a(1000);   // a can hold at most 1000 key/value pairs
	Map b(5);      // b can hold at most 5 key/value pairs
	Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
	KeyType k[6] = { "al", "shab", "jake", "mo", "su", "matt" };	//= { a list of six distinct values of the appropriate type };
	ValueType v = 12;		// = a value of the appropriate type;

	//to test swap:
	Map mymap;
	Map hismap;
	mymap.insert("Shab", 1994);
	mymap.insert("Billie", 1996);
	hismap.insert("Matt", 1993);
	cout << "mymap initial size: " << mymap.size() << endl;
	cout << "hismap initial size: " << hismap.size() << endl;
	mymap.swap(hismap);
	cout << "mymap final size: " << mymap.size() << endl;
	cout << "hismap final size: " << hismap.size() << endl;
	KeyType keyt = "";
	ValueType valt = -1;
	mymap.get(0, keyt, valt);
	cout << "pair in mymap: " << keyt << " " << valt << endl;
}
