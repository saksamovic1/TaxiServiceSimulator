#include "clientcommodule.h"
#include "controller.h"
#include "environment.h"
#include <iostream>
#include <QTimer>

Controller::Controller() {
    ClientComModule::initializeComModule();
    ClientComModule::getAccess().testConnection(); //poslije zakomentarisati
    Environment::initializeEnvironment();
}

void Controller::createConnections() {

    Environment* envSingleton = Environment::getInstance();

    QObject::connect(envSingleton, &Environment::locationsSet,
                     this, &Controller::locationsSet);
    QObject::connect(envSingleton, &Environment::requestObstacleLocations,
                     this, &Controller::requestObstacleLocations);

    envSingleton->initializeObstacleLocations(); // za slanje signala
}

void Controller::registerCab(std::reference_wrapper<Cab> cab)
{
    if (cab.get().getCabId() != -1) {
        throw std::logic_error("Cab already registered!");
    }

    cab.get().setCabId(cabs.size());
    cabs.push_back(cab);

    auto initialLocation = ClientComModule::getAccess().updateCabState(cab);
    cab.get().setCurrentLocation(initialLocation.first, initialLocation.second);
}

void Controller::registerEntryLocation(int row, int col)
{
    try {
        Environment::getInstance()->setEntryLocation(row, col);
    }
    catch(std::logic_error e) {
        std::cout << e.what();
    }
}

void Controller::registerExitLocation(int row, int col)
{
    try {
        Environment::getInstance()->setExitLocation(row, col);
    }
    catch(std::logic_error e) {
        std::cout << e.what();
    }
}

void Controller::updateEnvironmentState()
{
    auto newLocation = ClientComModule::getAccess().updateCabState(cabs[currentCabId]);
    if(newLocation != std::pair<int, int>(-1, -1)) {
        auto oldLocation =  cabs[currentCabId].get().getCurrentLocation();
        if(oldLocation.second < newLocation.second) {
            cabs[currentCabId].get().setRotation(0); //dole
        }
        else if(oldLocation.second > newLocation.second) {
            cabs[currentCabId].get().setRotation(1); //gore
        }
        else if(oldLocation.first < newLocation.first) {
            cabs[currentCabId].get().setRotation(2); //lijevo
        }
        else {
            cabs[currentCabId].get().setRotation(3); //desno
        }

        cabs[currentCabId].get().setCurrentLocation(newLocation.first, newLocation.second);
    }
    else {
        timer->stop();
        cabs[currentCabId].get().setBusy(false);
        ClientComModule::getAccess().freeBusyCab(currentCabId);
    }
}

void Controller::locationsSet()
{

    currentCabId = ClientComModule::getAccess().pickCab(Environment::getInstance()->getCurrentEntryLocation(),
                                                        Environment::getInstance()->getCurrentExitLocation());

    std::cout << "currentCabId: " << currentCabId;

    if(currentCabId == -1) {
        throw std::logic_error("No cabs currently available!");
    }
    else {
        timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, this, &Controller::updateEnvironmentState);
        timer->start(1000);
        cabs[currentCabId].get().setBusy(true);
    }
}

void Controller::requestObstacleLocations()
{
    auto obstacleLocations = ClientComModule::getAccess().setObstacleLocations();
    Environment::getInstance()->setObstacleLocations(obstacleLocations);
}
