#include "ServerController.h"
#include "ServerComModule.h"
#include <iostream>
#include <vector>

ServerController::ServerController(): optimalPathFinder() {
	setObstacleLocations();
	ServerComModule::initializeComModule();
}

int ServerController::pickCab(const jsonrpcpp::Parameter& params) { //na osnovu toga da li su busy i na osnovu puta izracunatog sa findPath iz AStarSearcha
						     
        std::pair<int, int> entry;
        std::pair<int, int> exit;
        
        entry.first = params.get<int>("entryLocationRow");
        entry.second = params.get<int>("entryLocationCol");
        exit.first = params.get<int>("exitLocationRow");
        exit.second = params.get<int>("exitLocationCol");
        
	int minTaxiID = -1;
	int minPathLength = 100;
	std::vector<std::pair<int, int>> path;
	
	for (int i=0; i<cabs.size(); i++){
		if (!cabs.at(i).get().isBusy()) {
			path = optimalPathFinder.findPath(cabs.at(i).get().getCurrentLocation(), entry);
			if (path.size()<= minPathLength) {
			minPathLength = path.size();
			optimalPath = path;
			minTaxiID = cabs.at(i).get().getTaxiID();
			}
		}
	}
	if (minTaxiID != -1) {
	 		path = optimalPathFinder.findPath(entry, exit);
	 		optimalPath.insert(optimalPath.end(), path.begin(), path.end());
	 		cabs.at(minTaxiID).get().setBusy(true);
	 		}
	return minTaxiID;
}
void ServerController::run() {
 	ServerComModule::getAccess().registerFunction("initializeCabState",
    	[this](const jsonrpcpp::Id& id, const jsonrpcpp::Parameter& params) {
    		std::pair <int, int> location = initializeCabState(params.get<int>("taxiID"));
    		int coordinates[2] = {location.first, location.second};
    		return std::make_shared<jsonrpcpp::Response>(id, coordinates);
        });
   ServerComModule::getAccess().registerFunction("updateCabState",
    	[this](const jsonrpcpp::Id& id, const jsonrpcpp::Parameter& params) {
    		std::pair <int, int> location = updateCabState(params.get<int>("taxiID"));
    		int coordinates[2] = {location.first, location.second};
    		return std::make_shared<jsonrpcpp::Response>(id, coordinates);
        });
     ServerComModule::getAccess().registerFunction("pickCab",
    	[this](const jsonrpcpp::Id& id, const jsonrpcpp::Parameter& params) {
    		return std::make_shared<jsonrpcpp::Response>(id,
    			pickCab(params));
        });
     ServerComModule::getAccess().registerFunction("setObstacleLocations",
    	[this](const jsonrpcpp::Id& id, const jsonrpcpp::Parameter& params) {
    		int obstacleCoordinates[20];
		int j=0;
		for (int i=0; i<obstacleLocations.size(); i++) {
			obstacleCoordinates[j] = obstacleLocations.at(i).first;
			obstacleCoordinates[j+1] = obstacleLocations.at(i).second;
			j=j+2;
		}
    		return std::make_shared<jsonrpcpp::Response>(id, obstacleCoordinates);
        });
     ServerComModule::getAccess().registerFunction("freeBusyCab",
    	[this](const jsonrpcpp::Parameter& params) {
    		freeBusyCab(params.get<int>("taxiID"));
        }); 
    ServerComModule::getAccess().runServer();
    
}
void ServerController::setObstacleLocations()
{
    int numObstacles = 10;

    while (obstacleLocations.size() < numObstacles) {

        std::pair<int, int> newPair = std::pair<int, int>(rand() % 10, rand() % 10);

        if( (newPair.second != 0 && (newPair.first != 0 && newPair.first != 1 && newPair.first != 2)) &&
            std::count(obstacleLocations.begin(), obstacleLocations.end(), newPair) == 0)  {
            obstacleLocations.push_back(newPair);
        }
    }
    optimalPathFinder.setObstacleLocations(obstacleLocations);
}

std::pair<int, int> ServerController::updateCabState(int taxiID) {
	std::pair<int, int> location(-1,-1);
	if (optimalPath.size() != 0) {
		location = optimalPath.at(0);
		optimalPath.erase(optimalPath.begin());
		cabs.at(taxiID).get().setCurrentLocation(location);
	}
	return location;
}

std::pair<int, int> ServerController::initializeCabState(int taxiID) {
	return cabs.at(taxiID).get().getCurrentLocation();
}

void ServerController::freeBusyCab(int taxiID) { //change to void later
	cabs.at(taxiID).get().setBusy(false);
}

void ServerController::registerCab(CabRef cab) {
    if (cab.get().getTaxiID() != -1) {
        throw std::logic_error("Taxi is already registered!");
    }

    cab.get().setTaxiID(cabs.size());
    cabs.push_back(cab);
}
