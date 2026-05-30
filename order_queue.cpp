#include "order_queue.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

OrderQueue::OrderQueue() {
    front = nullptr;
    rear = nullptr;
    completedHead = nullptr;
    completedRear = nullptr;
    pendingCount = 0;
    completedCount = 0;
}

// clean up all orders in list
OrderQueue::~OrderQueue() {
    while (!isEmpty()) {
        Order* temp = front;
        front = front->next;
        delete temp;
    }
    while (completedHead != nullptr) {
        Order* temp = completedHead;
        completedHead = completedHead->next;
        delete temp;
    }
}

bool OrderQueue::isEmpty() {
    return front == nullptr;
}

// add new order into the rear of queue
void OrderQueue::enqueue(int orderID, std::string itemName, int itemID, std::string location) {
    Order* newOrder = new Order(orderID, itemName, itemID, location);

    if (isEmpty()) {
        front = newOrder;
        rear = newOrder;
    } else {
        rear->next = newOrder;
        rear = newOrder;
    }
    pendingCount++;
    cout << "[SUCCESS] Order #" << orderID << " (" << itemName
         << ") at location " << location << " added to queue.\n";
}

// remove the first order in the queue and process it
Order* OrderQueue::dequeue() {
    if (isEmpty()) {
        cout << "[ERROR] No pending orders to process.\n";
        return nullptr;
    }

    Order* removed = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    removed->next = nullptr;
    removed->status = "PROCESSING";
    pendingCount--;
    cout << "[INFO] Order #" << removed->orderID << " (" << removed->itemName << ") is now processing.\n";
    return removed;
}

// add completed order into list
void OrderQueue::addCompleted(Order* order) {
    if (order == nullptr) return;

    order->status = "COMPLETED";
    order->next = nullptr;

    if (completedHead == nullptr) {
        // First completed order
        completedHead = order;
        completedRear = order;
    } else {
        // Append to the end of the completed list
        completedRear->next = order;
        completedRear = order;
    }
    completedCount++;
    cout << "[SUCCESS] Order #" << order->orderID << " (" << order->itemName << ") completed.\n";
}

int OrderQueue::getPendingCount() {
    return pendingCount;
}

// show all pending orders
void OrderQueue::displayPending() {
    cout << "\n";
    cout << "========================================\n";
    cout << "         PENDING ORDERS (" << pendingCount << ")\n";
    cout << "========================================\n";

    if (isEmpty()) {
        cout << "[INFO] No pending orders.\n";
        return;
    }

    cout << left << setw(12) << "Order ID"
         << setw(18) << "Item Name"
         << setw(10) << "Item ID"
         << setw(10) << "Location"
         << setw(12) << "Status" << "\n";
    cout << "==============================================================\n";

    Order* curr = front;
    while (curr != nullptr) {
        cout << left << setw(12) << curr->orderID
             << setw(18) << curr->itemName
             << setw(10) << curr->itemID
             << setw(10) << curr->location
             << setw(12) << curr->status << "\n";
        curr = curr->next;
    }
    cout << "==============================================================\n";
}

// show all completed orders
void OrderQueue::displayCompleted() {
    cout << "\n";
    cout << "========================================\n";
    cout << "       COMPLETED ORDERS (" << completedCount << ")\n";
    cout << "========================================\n";

    if (completedHead == nullptr) {
        cout << "[INFO] No completed orders yet.\n";
        return;
    }

    cout << left << setw(12) << "Order ID"
         << setw(18) << "Item Name"
         << setw(10) << "Item ID"
         << setw(10) << "Location"
         << setw(12) << "Status" << "\n";
    cout << "==============================================================\n";

    Order* curr = completedHead;
    while (curr != nullptr) {
        cout << left << setw(12) << curr->orderID
             << setw(18) << curr->itemName
             << setw(10) << curr->itemID
             << setw(10) << curr->location
             << setw(12) << curr->status << "\n";
        curr = curr->next;
    }
    cout << "==============================================================\n";
}
