#include "mbed.h"
#include "ServerController.h"

int main(int argc, char *argv[]) {
    
    ServerController serverController;
    Cab SamirEmir, CrveniTaxi, ZutiTaxi;
    
    SamirEmir.setCurrentLocation(std::pair<int, int> (0, 0));
    CrveniTaxi.setCurrentLocation(std::pair<int, int> (1, 0));
    ZutiTaxi.setCurrentLocation(std::pair<int, int> (2, 0));
    
    SamirEmir.setBusy(false);
    CrveniTaxi.setBusy(false);
    ZutiTaxi.setBusy(false);
    
    
    serverController.registerCab(SamirEmir);
    serverController.registerCab(CrveniTaxi);
    serverController.registerCab(ZutiTaxi);
    
    serverController.run();

    return 0;
}
