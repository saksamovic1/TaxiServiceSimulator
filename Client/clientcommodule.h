#include "cab.h"
#include "serialib.h"
#include <string>
#include <vector>


class ClientComModule {
    serialib serialPort;

    ClientComModule(std::string const &devicePort, unsigned int baudRate);
    static ClientComModule* clientComModule;

public:
    bool testConnection();
    static void initializeComModule();
    static ClientComModule& getAccess();
    int pickCab(std::pair<int, int> entry, std::pair<int, int> exit);
    void freeBusyCab(int cabId); //void
    std::pair<int, int> updateCabState(std::reference_wrapper<Cab> cab);
    std::vector<std::pair<int, int>> setObstacleLocations();
    ~ClientComModule();
};
