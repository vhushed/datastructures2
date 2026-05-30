#include "order_queue.hpp"
#include "robot_assignment.hpp"
#include "path_stack.hpp"
#include "warehouse_layout.hpp"
#include "binary_tree_search_items.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

struct ActiveTask {
    Order* order;
    int robotID;
    PathStack* path;
};

int main()
{
    OrderQueue orderQueue;
    RobotPool robotPool(10);
    W_itemBTS warehouse;
    WarehouseLayout layout;

    // create 5 robots first
    for (int i = 1; i <= 5; i++) {
        robotPool.addRobot(i);
    }

    int nextOrderID = 1001;

    // get all active tasks
    const int MAX_TASKS = 10;
    ActiveTask* activeTasks[MAX_TASKS];
    for (int i = 0; i < MAX_TASKS; i++) {
        activeTasks[i] = nullptr;
    }
    int activeTaskCount = 0;

    while (true)
    {
        cout << "\n\n";
        cout << "====================================================================\n";
        cout << "       WAREHOUSE ROBOT NAVIGATION & ORDER MANAGEMENT SYSTEM\n";
        cout << "====================================================================\n";
        cout << "  [1]  Add New Order\n";
        cout << "  [2]  View Pending Orders\n";
        cout << "  [3]  Process Next Order (Assign Robot & Navigate)\n";
        cout << "  [4]  Complete a Task (Return Robot)\n";
        cout << "  [5]  View Robot Status\n";
        cout << "  [6]  Add Robot to Pool\n";
        cout << "  [7]  Manage Robot Status\n";
        cout << "  [8]  Warehouse Item Search & Management\n";
        cout << "  [9]  View System Overview\n";
        cout << "  [0]  Exit System\n";
        cout << "====================================================================\n";
        cout << "Active tasks: " << activeTaskCount << " | Pending orders: "
             << orderQueue.getPendingCount() << "\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cout << "[ERROR] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // 1. add new order
        if (choice == 1)
        {
            cout << "\n--- ADD NEW ORDER ---\n";
            cout << "Item name: ";
            string itemName;
            cin >> itemName;
            cout << "Item ID (numeric): ";
            int itemID;
            cin >> itemID;

            if (cin.fail())
            {
                cout << "[ERROR] Invalid item ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cout << "Shelf location (e.g., D2, Aisle-C3, a1): ";
            string location;
            cin >> location;

            // Validate the location
            int testX, testY;
            if (!layout.parseLocation(location, testX, testY))
            {
                cout << "[ERROR] Invalid location. Use format like D2, Aisle-C3, a1.\n";
                cout << "[INFO] Valid zones: A-E, shelves: 1-5.\n";
                continue;
            }

            orderQueue.enqueue(nextOrderID, itemName, itemID, location);
            cout << "[INFO] Order #" << nextOrderID << " created with status PENDING.\n";
            nextOrderID++;
        }

        // 2. show pending orders
        else if (choice == 2)
        {
            orderQueue.displayPending();
        }

        // 3. process next order by assign a robot and navigate to item
        else if (choice == 3)
        {
            if (orderQueue.isEmpty())
            {
                cout << "[INFO] No pending orders to process.\n";
                continue;
            }

            Order* order = orderQueue.dequeue();
            if (order == nullptr) continue;

            Robot* assigned = robotPool.getNextAvailable();
            if (assigned == nullptr)
            {
                cout << "[INFO] No robots available right now.\n";
                cout << "[INFO] Try again when a robot becomes available (Option 4).\n";
                delete order;
                continue;
            }

            robotPool.assignTask(assigned->robotID, order->orderID);

            cout << "\n[INFO] Robot #" << assigned->robotID << " searching for item: "
                 << order->itemName << " (ID: " << order->itemID << ")\n";

            cout << "\n[NAVIGATION] Robot #" << assigned->robotID
                 << " deployed -- heading to: " << order->itemName
                 << " at " << order->location << "\n";

            int destX, destY;
            PathStack* path = new PathStack();
            
            layout.parseLocation(order->location, destX, destY);
            layout.generatePath(destX, destY, *path, assigned->robotID, order->itemName);

            ActiveTask* task = new ActiveTask();
            task->order = order;
            task->robotID = assigned->robotID;
            task->path = path;

            bool stored = false;
            for (int i = 0; i < MAX_TASKS; i++) {
                if (activeTasks[i] == nullptr) {
                    activeTasks[i] = task;
                    activeTaskCount++;
                    stored = true;
                    break;
                }
            }

            if (!stored) {
                cout << "[ERROR] Task slots full. Cannot start new task.\n";
                delete path;
                delete task;
                // order must stay alive — put it back via addCompleted? No,
                // the order is already dequeued. Simplest: mark it lost.
                // This path is unreachable while MAX_TASKS >= robot count.
                delete order;
                continue;
            }

            cout << "\n[INFO] Task active: Order #" << order->orderID
                 << " | Robot #" << assigned->robotID
                 << " | " << activeTaskCount << " task(s) now in progress.\n";
        }

        // 4. complete a task, return the robot to base with reverse path
        else if (choice == 4)
        {
            if (activeTaskCount == 0)
            {
                cout << "[INFO] No tasks are currently in progress.\n";
                continue;
            }

            cout << "\n";
            cout << "========================================\n";
            cout << "       ACTIVE TASKS (" << activeTaskCount << ")\n";
            cout << "========================================\n";
            cout << left << setw(6)  << "No."
                 << setw(12) << "Order ID"
                 << setw(10) << "Robot ID"
                 << setw(16) << "Item" << "\n";
            cout << "========================================\n";

            int displayNum = 1;
            int taskIndexMap[MAX_TASKS];

            for (int i = 0; i < MAX_TASKS; i++) {
                if (activeTasks[i] != nullptr) {
                    cout << left << setw(6)  << displayNum
                         << setw(12) << activeTasks[i]->order->orderID
                         << setw(10) << activeTasks[i]->robotID
                         << setw(16) << activeTasks[i]->order->itemName << "\n";
                    taskIndexMap[displayNum - 1] = i;
                    displayNum++;
                }
            }
            cout << "========================================\n";
            cout << "Enter task number to complete (0 to cancel): ";

            int taskChoice;
            cin >> taskChoice;

            if (cin.fail() || taskChoice < 0 || taskChoice > activeTaskCount)
            {
                cout << "[ERROR] Invalid selection.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (taskChoice == 0)
            {
                continue;
            }

            int arrIdx = taskIndexMap[taskChoice - 1];
            ActiveTask* task = activeTasks[arrIdx];

            cout << "\n[NAVIGATION] Robot #" << task->robotID
                 << " returning to base...\n";
            task->path->retracePath();
            delete task->path;

            robotPool.completeTask(task->robotID);

            orderQueue.addCompleted(task->order);

            cout << "[SUCCESS] Task completed. Robot #" << task->robotID
                 << " is back at base, Order #" << task->order->orderID
                 << " is done.\n";

            delete task;
            activeTasks[arrIdx] = nullptr;
            activeTaskCount--;
        }

        // 5. show robot status
        else if (choice == 5)
        {
            robotPool.displayStatus();
        }

        // 6. add new robot to pool
        else if (choice == 6)
        {
            cout << "\n--- ADD ROBOT TO POOL ---\n";
            cout << "Robot ID: ";
            int robotID;
            cin >> robotID;

            if (cin.fail())
            {
                cout << "[ERROR] Invalid robot ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            robotPool.addRobot(robotID);
        }

        // 7. change robot status
        else if (choice == 7)
        {
            cout << "\n--- MANAGE ROBOT STATUS ---\n";
            cout << "  [a] Set robot to MAINTENANCE\n";
            cout << "  [b] Set robot to AVAILABLE\n";
            cout << "Enter choice: ";
            char sub;
            cin >> sub;

            cout << "Robot ID: ";
            int robotID;
            cin >> robotID;

            if (cin.fail())
            {
                cout << "[ERROR] Invalid input.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (sub == 'a')
            {
                robotPool.setMaintenance(robotID);
            }
            else if (sub == 'b')
            {
                robotPool.setAvailable(robotID);
            }
            else
            {
                cout << "[ERROR] Invalid choice.\n";
            }
        }

        // 8. item search and management
        else if (choice == 8)
        {
            while (true)
            {
                cout << "\n";
                cout << "========================================\n";
                cout << "   WAREHOUSE INVENTORY MANAGEMENT\n";
                cout << "========================================\n";
                cout << "  [1] Add new item\n";
                cout << "  [2] Search for item\n";
                cout << "  [3] Update item\n";
                cout << "  [4] Delete item\n";
                cout << "  [5] Display all items\n";
                cout << "  [6] Return to main menu\n";
                cout << "========================================\n";
                cout << "Enter choice: ";

                int wChoice;
                cin >> wChoice;

                if (cin.fail())
                {
                    cout << "[ERROR] Invalid input. Please enter a number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                if (wChoice == 1)
                {
                    cout << "\nEnter item details:\n";
                    cout << "Item name: ";
                    string itemName;
                    cin >> itemName;
                    cout << "Item location (e.g., Aisle-A1): ";
                    string itemLocation;
                    cin >> itemLocation;
                    cout << "Item quantity: ";
                    int itemQuantity;
                    cin >> itemQuantity;

                    if (cin.fail())
                    {
                        cout << "[ERROR] Invalid quantity.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    warehouse.insertW_item(itemName, itemLocation, itemQuantity);
                }
                else if (wChoice == 2)
                {
                    warehouse.searchItem();
                }
                else if (wChoice == 3)
                {
                    warehouse.updateW_item();
                }
                else if (wChoice == 4)
                {
                    warehouse.deleteW_item();
                }
                else if (wChoice == 5)
                {
                    warehouse.displayW_item();
                }
                else if (wChoice == 6)
                {
                    break;
                }
                else
                {
                    cout << "[ERROR] Invalid choice. Please enter a number between 1 and 6.\n";
                }
            }
        }

        // 9. system overview
        else if (choice == 9)
        {
            cout << "\n";
            cout << "====================================================================\n";
            cout << "                     SYSTEM OVERVIEW\n";
            cout << "====================================================================\n";

            // order summary
            cout << "\n[ORDERS]\n";
            cout << "  Pending orders : " << orderQueue.getPendingCount() << "\n";

            // shows active tasks
            cout << "  Active tasks (" << activeTaskCount << "):\n";
            if (activeTaskCount == 0)
            {
                cout << "    None\n";
            }
            else
            {
                for (int i = 0; i < MAX_TASKS; i++)
                {
                    if (activeTasks[i] != nullptr)
                    {
                        cout << "    Order #" << activeTasks[i]->order->orderID
                             << " (" << activeTasks[i]->order->itemName
                             << ") -- Robot #" << activeTasks[i]->robotID << "\n";
                    }
                }
            }
            orderQueue.displayCompleted();

            // robot summary
            robotPool.displayStatus();

            // warehouse layout
            layout.displayGrid();
        }

        // 0. exit
        else if (choice == 0)
        {
            // clean up active tasks
            if (activeTaskCount > 0)
            {
                cout << "[WARNING] " << activeTaskCount
                     << " active task(s) still in progress:\n";
                for (int i = 0; i < MAX_TASKS; i++)
                {
                    if (activeTasks[i] != nullptr)
                    {
                        cout << "  Order #" << activeTasks[i]->order->orderID
                             << " — Robot #" << activeTasks[i]->robotID << "\n";
                        delete activeTasks[i]->path;
                        delete activeTasks[i]->order;
                        delete activeTasks[i];
                        activeTasks[i] = nullptr;
                    }
                }
                cout << "[WARNING] These tasks have been discarded.\n";
            }

            cout << "\n[INFO] Shutting down Warehouse Robot Navigation System. Goodbye!\n";
            break;
        }

        else
        {
            cout << "[ERROR] Invalid choice. Please enter a number between 0 and 9.\n";
        }
    }

    return 0;
}
