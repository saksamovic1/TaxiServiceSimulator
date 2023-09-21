#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <QObject>

class Environment: public QObject // singleton - zelimo samo 1 zasticen environment, ova klasa je nesto kao pristup bazi

{
    Q_OBJECT

    std::vector<std::pair<int, int>> obstacleLocations;
    std::pair<int, int> currentEntryLocation;
    std::pair<int, int> currentExitLocation;
    static Environment* environment;
    Environment();

public:
    static void initializeEnvironment();
    static Environment* getInstance();
    Q_INVOKABLE bool isObstacle(int row, int col);
    Q_INVOKABLE void setEntryLocation(int row, int col);
    Q_INVOKABLE void setExitLocation(int row, int col);
    std::vector<std::pair<int, int>> getObstacleLocations() const;
    std::pair<int, int> getCurrentEntryLocation() const;
    Q_INVOKABLE int getCurrentEntryRow() const;
    Q_INVOKABLE int getCurrentEntryCol() const;
    std::pair<int, int> getCurrentExitLocation() const;
    Q_INVOKABLE int getCurrentExitRow() const;
    Q_INVOKABLE int getCurrentExitCol() const;
    void setObstacleLocations(std::vector<std::pair<int, int>> obstacleLocations);
    void initializeObstacleLocations();

signals:
    void requestObstacleLocations();
    void locationsSet();
};

#endif // ENVIRONMENT_H
