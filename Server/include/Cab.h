#pragma once 
#include "jsonrpcpp.hpp"
#include <functional>
#include <utility>

class Cab {
	int taxiID;
	bool busy;
	std::pair<int, int> currentLocation; //x i y koordinate trenutne lokacije
public:
	Cab();
	std::pair<int, int> getCurrentLocation() const;
	int getTaxiID() const;
	bool isBusy() const;
	void setBusy(bool busy);
	void setCurrentLocation(std::pair<int, int> currentLocation);
	void setTaxiID(int taxiID);
};
