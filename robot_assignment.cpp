#include "robot_assignment.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

RobotPool::RobotPool(int size) {
    capacity = size;
    robots = new Robot*[capacity];
    for (int i = 0; i < capacity; i++) {
        robots[i] = nullptr;
    }
    count = 0;
    front = 0;
    rear = 0;
}

RobotPool::~RobotPool() {
    for (int i = 0; i < capacity; i++) {
        if (robots[i] != nullptr) {
            delete robots[i];
        }
    }
    delete[] robots;
}

// find the robot by id
int RobotPool::findRobotIndex(int robotID) {
    for (int i = 0; i < capacity; i++) {
        if (robots[i] != nullptr && robots[i]->robotID == robotID) {
            return i;
        }
    }
    return -1;
}

// add robot into pool
bool RobotPool::addRobot(int robotID) {
    if (count >= capacity) {
        cout << "[ERROR] Robot pool is full (max " << capacity
             << "). Cannot add robot #" << robotID << ".\n";
        return false;
    }

    if (findRobotIndex(robotID) != -1) {
        cout << "[ERROR] Robot #" << robotID << " already exists in the pool.\n";
        return false;
    }

    Robot* newRobot = new Robot(robotID);
    robots[rear] = newRobot;

    rear = (rear + 1) % capacity;
    count++;
    cout << "[SUCCESS] Robot #" << robotID << " added to the pool.\n";
    return true;
}

// get the available robot to assign task
Robot* RobotPool::getNextAvailable() {
    if (count == 0) {
        cout << "[ERROR] No robots in the pool.\n";
        return nullptr;
    }

    for (int i = 0; i < count; i++) {
        int idx = (front + i) % capacity;

        if (robots[idx] != nullptr && robots[idx]->status == "AVAILABLE") {
            front = (idx + 1) % capacity;
            return robots[idx];
        }
    }

    cout << "[WARNING] All robots are currently busy or under maintenance.\n";
    return nullptr;
}

// assign robot to task
bool RobotPool::assignTask(int robotID, int orderID) {
    int idx = findRobotIndex(robotID);
    if (idx == -1) {
        cout << "[ERROR] Robot #" << robotID << " not found in the pool.\n";
        return false;
    }

    if (robots[idx]->status != "AVAILABLE") {
        cout << "[ERROR] Robot #" << robotID << " is currently "
             << robots[idx]->status << ".\n";
        return false;
    }

    robots[idx]->status = "BUSY";
    robots[idx]->currentOrderID = orderID;
    cout << "[SUCCESS] Robot #" << robotID << " assigned to Order #"
         << orderID << ".\n";
    return true;
}

// mark the robot's task as completed, then set robot to available
bool RobotPool::completeTask(int robotID) {
    int idx = findRobotIndex(robotID);
    if (idx == -1) {
        cout << "[ERROR] Robot #" << robotID << " not found in the pool.\n";
        return false;
    }

    if (robots[idx]->status != "BUSY") {
        cout << "[WARNING] Robot #" << robotID
             << " is not currently assigned to a task.\n";
        return false;
    }

    int finishedOrder = robots[idx]->currentOrderID;
    robots[idx]->status = "AVAILABLE";
    robots[idx]->currentOrderID = -1;
    cout << "[SUCCESS] Robot #" << robotID << " completed Order #"
         << finishedOrder << " and is now available.\n";
    return true;
}

// set robot to maintenance mode
bool RobotPool::setMaintenance(int robotID) {
    int idx = findRobotIndex(robotID);
    if (idx == -1) {
        cout << "[ERROR] Robot #" << robotID << " not found in the pool.\n";
        return false;
    }

    robots[idx]->status = "MAINTENANCE";
    robots[idx]->currentOrderID = -1;
    cout << "[INFO] Robot #" << robotID << " set to MAINTENANCE.\n";
    return true;
}

// set robot to available mode
bool RobotPool::setAvailable(int robotID) {
    int idx = findRobotIndex(robotID);
    if (idx == -1) {
        cout << "[ERROR] Robot #" << robotID << " not found in the pool.\n";
        return false;
    }

    robots[idx]->status = "AVAILABLE";
    robots[idx]->currentOrderID = -1;
    cout << "[INFO] Robot #" << robotID << " is now AVAILABLE.\n";
    return true;
}

// display all robots status
void RobotPool::displayStatus() {
    cout << "\n";
    cout << "========================================\n";
    cout << "         ROBOT STATUS OVERVIEW\n";
    cout << "========================================\n";

    if (count == 0) {
        cout << "[INFO] No robots in the pool.\n";
        return;
    }

    cout << left << setw(12) << "Robot ID"
         << setw(16) << "Status"
         << setw(16) << "Current Order" << "\n";
    cout << "========================================\n";

    for (int i = 0; i < capacity; i++) {
        if (robots[i] != nullptr) {
            string orderStr;
            if (robots[i]->currentOrderID == -1) {
                orderStr = "None";
            } else {
                orderStr = "#" + to_string(robots[i]->currentOrderID);
            }

            cout << left << setw(12) << robots[i]->robotID
                 << setw(16) << robots[i]->status
                 << setw(16) << orderStr << "\n";
        }
    }
    cout << "========================================\n";
    cout << "Total robots: " << count << " | Pool capacity: " << capacity << "\n";
}
