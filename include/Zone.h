#ifndef ZONE_H
#define ZONE_H

#include <vector>
#include "ParkingArea.h"

class Zone {
private:
    int zoneID;
    std::vector<ParkingArea> areas;

public:
    Zone(int id, int numAreas, int slotsPerArea);
    int getZoneID() const;
    ParkingSlot* findFirstAvailableSlot(); // Delegates to Areas
    ParkingSlot* getSlot(int slotID);
};
#endif