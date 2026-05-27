#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <string>
#include <iostream>

struct Order {
    int orderID;
    std::string itemName;
    int itemID;
    std::string status;  
    // status enumerate: PENDING, PROCESSING, COMPLETED 
    Order* next;    
    Order(int id, std::string iName, int iID) 
        : orderID(id), itemName(iName), itemID(iID), status("PENDING"), next(nullptr) {}
};

struct Step {
    std::string direct; 
    Step* nxt; 
    Step(std::string dir) {
        direct = dir;
        nxt = nullptr;
    }
};

#endif