#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle {
private:
    int vehicleID;
    int preferredZone;

public:
    Vehicle(int vID, int pZone);
    int getID() const;
    int getPreferredZone() const;
};
#endif