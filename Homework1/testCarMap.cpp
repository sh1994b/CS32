#include "CarMap.h"
#include <iostream>
using namespace std;

int main()
{
	CarMap cm;
	cout << cm.fleetSize() << endl;
	cm.addCar("7STG306");
	cm.addCar("33OPB85");
	cout << "initial amount of gas: " << cm.gas("7STG306") << endl;
	cm.addGas("7STG306", 9);
	cout << "new amount of gas: " << cm.gas("7STG306") << endl;
	cm.useGas("7STG306", 3);
	cout << "new amount of gas: " << cm.gas("7STG306") << endl;
	cout << cm.fleetSize() << endl;
	cout << endl;
	cm.print();

}
