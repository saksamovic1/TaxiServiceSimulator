#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "cab.h"

#include <QObject>
#include <QTimer>

class Controller: public QObject
{
    Q_OBJECT

    std::vector<std::reference_wrapper<Cab>> cabs; //da li je ovdje okej da budu smjesteni cabs
    QTimer* timer;
    int currentCabId;

public:
    Controller();
    void registerCab(std::reference_wrapper<Cab> cab);
    void registerEntryLocation(int row, int col);
    void registerExitLocation(int row, int col);
    void createConnections();

public slots:
    void locationsSet();
    void requestObstacleLocations();
    void updateEnvironmentState();

};

#endif // CONTROLLER_H
