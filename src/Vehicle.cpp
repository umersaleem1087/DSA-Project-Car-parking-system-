#include "Vehicle.h"
Vehicle::Vehicle(int vID, int pZone) : vehicleID(vID), preferredZone(pZone) {}
int Vehicle::getID() const { return vehicleID; }
int Vehicle::getPreferredZone() const { return preferredZone; }