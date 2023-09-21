#include "environment.h"

Environment* Environment::environment{}; //radi static pointera

Environment::Environment(): currentEntryLocation(-1, -1), currentExitLocation(-1, -1)
{
//moraju nam entry i exit lokacije biti validne, pa cemo ih ovako ubacivati na pocetku
}

void Environment::initializeEnvironment()
{

    if (environment == nullptr) {
        environment = new Environment();
    } else {
        throw std::logic_error("Environment access already available!");
    }

    std::vector<std::pair<int, int>> obstacleLocations = std::vector<std::pair<int, int>>();
}

Environment* Environment::getInstance()
{
    return environment;
}

bool Environment::isObstacle(int row, int col)
{
    auto newPair = std::pair<int, int>(row, col);
    return (std::count(obstacleLocations.begin(), obstacleLocations.end(), newPair));
    //ako bude broj veci od 0, onda jeste obstacle, pa je true
}

void Environment::setEntryLocation(int row, int col)
{
    if(!this->isObstacle(row, col)) {
        currentEntryLocation = std::pair<int, int>(row, col); //ovdje mozda treba neki exception baciti
    }
    else throw std::logic_error("Chosen entry location is obstacle!");
}

void Environment::setExitLocation(int row, int col)
{
    if(!this->isObstacle(row, col)) {
        currentExitLocation = std::pair<int, int>(row, col);
        emit locationsSet(); //pretpostavljeno da su se obje lokacije poslale
    }
    else throw std::logic_error("Chosen exit location is obstacle!");
}

std::vector<std::pair<int, int> > Environment::getObstacleLocations() const
{
    return obstacleLocations;
}

std::pair<int, int> Environment::getCurrentEntryLocation() const
{
    return currentEntryLocation;
}

int Environment::getCurrentEntryRow() const
{
    return currentEntryLocation.first;
}

int Environment::getCurrentEntryCol() const
{
    return currentEntryLocation.second;
}

int Environment::getCurrentExitRow() const
{
    return currentExitLocation.first;
}

int Environment::getCurrentExitCol() const
{
    return currentExitLocation.second;
}

void Environment::setObstacleLocations(std::vector<std::pair<int, int> > obstacleLocations)
{
    this->obstacleLocations = obstacleLocations;
}

void Environment::initializeObstacleLocations()
{
    emit requestObstacleLocations();
}

std::pair<int, int> Environment::getCurrentExitLocation() const
{
    return currentExitLocation;
}


