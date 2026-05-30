#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>
#include <iostream>

struct Order {
    int orderID;
    std::string itemName;
    int itemID;
    std::string location;   // e.g. "D2" or "Aisle-D2"
    std::string status;
    // status enumerate: PENDING, PROCESSING, COMPLETED
    Order* next;
    Order(int id, std::string iName, int iID, std::string loc)
        : orderID(id), itemName(iName), itemID(iID), location(loc), status("PENDING"), next(nullptr) {}
};

struct Step {
    std::string direct;
    Step* nxt;
    Step(std::string dir) {
        direct = dir;
        nxt = nullptr;
    }
};

struct Robot {
    int robotID;
    std::string status;  // AVAILABLE, BUSY, MAINTENANCE
    int currentOrderID;
    Robot(int id)
        : robotID(id), status("AVAILABLE"), currentOrderID(-1) {}
};

#endif