#pragma once
#include <QObject>
#include <functional>
#include <utility>

class Cab: public QObject {
    Q_OBJECT

    int cabId;
    bool busy;
    std::pair<int, int> currentLocation;
    int rotation;

public:
    explicit Cab(QObject *parent = nullptr);
    Cab();
    std::pair<int, int> getCurrentLocation() const;
    Q_INVOKABLE int getCurrentRow() const;
    Q_INVOKABLE int getCurrentCol() const; //ne moze primiti par
    int getCabId() const;
    bool isBusy() const;
    void setBusy(bool busy);
    void setCurrentLocation(int row, int col);
    void setCabId(int cabId);
    Q_INVOKABLE int getRotation() const;
    void setRotation(int rotation);

signals:
    void currentLocationChanged();
    void busyChanged();
};
