#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
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
	KeyType k = "";
	ValueType v = -1;
	mymap.get(0, k, v);
	cout << "pair in mymap: " << k << " " << v << endl;
}
