#pragma once
#include "Cab.h"
#include "AStarSearch.h"
#include <vector>

class ServerController {
	using CabRef = std::reference_wrapper<Cab>;
	
	std::vector<CabRef> cabs;
	AStarSearch optimalPathFinder; 
	std::vector<std::pair<int, int>> optimalPath;
	std::vector<std::pair<int, int>> obstacleLocations;
	
	int pickCab(const jsonrpcpp::Parameter& params); 
	void setObstacleLocations();
	void createObstacles(const jsonrpcpp::Parameter& params); //should be back to void
	std::pair<int, int> initializeCabState(int taxiID);
	std::pair<int, int> updateCabState(int taxiID);
	void freeBusyCab(int taxiID);
public:
	ServerController();
	void registerCab(CabRef cab);
	void run();
};
