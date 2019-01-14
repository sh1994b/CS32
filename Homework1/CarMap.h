#ifndef CARMAP_INCLUDED
#define CARMAP_INCLUDED

#include "Map.h"

class CarMap
{
public:
	CarMap();       // Create an empty car map.
	bool addCar(std::string license);
	double gas(std::string license) const;
	bool addGas(std::string license, double gallons);
	bool useGas(std::string license, double gallons);
	int fleetSize() const; 
	void print() const;
private:
	Map m_map;
};

#endif //CARMAP_INCLUDED