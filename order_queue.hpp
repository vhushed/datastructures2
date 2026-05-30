#ifndef ORDER_QUEUE_HPP
#define ORDER_QUEUE_HPP

#include "structs.hpp"
#include <string>

class OrderQueue {
private:
    Order* front;
    Order* rear;
    Order* completedHead;
    Order* completedRear;
    int pendingCount;
    int completedCount;

public:
    OrderQueue();
    ~OrderQueue();

    void enqueue(int orderID, std::string itemName, int itemID, std::string location);
    Order* dequeue();
    bool isEmpty();
    void addCompleted(Order* order);
    void displayPending();
    void displayCompleted();
    int getPendingCount();
};

#endif
