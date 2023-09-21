#include "Cab.h"

Cab::Cab(): taxiID(-1), busy(false)
{
}

std::pair<int, int> Cab::getCurrentLocation() const
{
    return this->currentLocation;   
}

int Cab::getTaxiID() const
{
    return this->taxiID;
}

bool Cab::isBusy() const
{
    return this->busy;
}

void Cab::setBusy(bool busy)
{
    this->busy = busy;
}

void Cab::setCurrentLocation(std::pair<int, int> currentLocation) {
    this->currentLocation = currentLocation;
}

void Cab::setTaxiID(int taxiID) {
    this->taxiID = taxiID;
}
