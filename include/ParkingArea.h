#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <vector>
#include "ParkingSlot.h"

class ParkingArea {
private:
    int areaID;
    int zoneID;
    std::vector<ParkingSlot> slots;

public:
    ParkingArea(int aID, int zID, int capacity);
    int getAreaID() const;
    ParkingSlot* findFreeSlot();
    ParkingSlot* getSlotById(int slotID);
    int getAvailableCount() const;
};
#endif