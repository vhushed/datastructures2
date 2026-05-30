#ifndef ROBOT_ASSIGNMENT_HPP
#define ROBOT_ASSIGNMENT_HPP

#include "structs.hpp"
#include <string>

class RobotPool {
private:
    Robot** robots;
    int capacity;
    int count;
    int front;
    int rear;

    int findRobotIndex(int robotID);

public:
    RobotPool(int size = 10);
    ~RobotPool();

    bool addRobot(int robotID);
    Robot* getNextAvailable();
    bool assignTask(int robotID, int orderID);
    bool completeTask(int robotID);
    bool setMaintenance(int robotID);
    bool setAvailable(int robotID);
    void displayStatus();
};

#endif
