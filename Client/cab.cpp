#include "cab.h"

Cab::Cab(QObject *parent): busy(false), cabId(-1), currentLocation(-1, -1)
{

}

Cab::Cab(): busy(false), cabId(-1), currentLocation(-1, -1)
{

}

std::pair<int, int> Cab::getCurrentLocation() const
{
    return this->currentLocation;
}

int Cab::getCurrentRow() const {
    return this->currentLocation.first;
}

int Cab::getCurrentCol() const {
    return this->currentLocation.second;
}

int Cab::getCabId() const
{
    return this->cabId;
}

bool Cab::isBusy() const
{
    return this->busy;
}

void Cab::setBusy(bool busy)
{
    this->busy = busy;
    emit busyChanged();
}

void Cab::setCurrentLocation(int row, int col) {
    this->currentLocation = std::pair<int, int>(row, col);
    emit currentLocationChanged();
}

void Cab::setCabId(int cabId) {
    this->cabId = cabId;
}

int Cab::getRotation() const
{
    return this->rotation;
}

void Cab::setRotation(int rotation) {
    this->rotation = rotation;
}
