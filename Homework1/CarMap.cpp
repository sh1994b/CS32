#include "CarMap.h"

#include <iostream>
using namespace std;

CarMap::CarMap()
{
	//empty since upon making m_map, its constructor is automatically
	//called, leaving an empty map.
}

bool CarMap::addCar(std::string license)
{
	m_map.insert(license, 0);
	return m_map.insert(license, 0);
}

double CarMap::gas(std::string license) const
{
	double gas = -1;
	m_map.get(license, gas);
	if (m_map.get(license, gas))
		return gas;
	else
		return -1;

}
// If a car with the given license plate is in the map, return the
// number of gallons of gas in its tank; otherwise, return -1.


bool CarMap::addGas(std::string license, double gallons)
{
	//first get the amount of gas in that license then use update function to update gas + gallons there
	double gas = -1;
	m_map.get(license, gas);
	if (!m_map.get(license, gas) || gallons < 0)
		return false;
	else
	{
		gas = gas + gallons;
		m_map.update(license, gas);
		return true;
	}
}

bool CarMap::useGas(std::string license, double gallons)
{
	if (gas(license) == -1 || gallons < 0 || gallons > gas(license))
		return false;
	else
	{
		double newgas = gas(license) - gallons;
		m_map.update(license, newgas);
		return true;
	}

}
// If no car with the given license plate is in the map or if
// gallons is negative or if gallons > gas(), make no change to the
// map and return false.  Otherwise, decrease the number of gallons
// of gas in the indicated car by the gallons parameter and return
// true.

int CarMap::fleetSize() const
{
	return m_map.size();
}
// Return the number of cars in the CarMap.

void CarMap::print() const
{
	std::string license;
	double gas;
	for (int i = 0; i < fleetSize(); i++)
	{
		m_map.get(i, license, gas);
		cout << license << " " << gas << endl;
	}
}
// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of gallons in that car's tank.  Write
// no other text.

