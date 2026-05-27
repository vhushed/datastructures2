#include "path_stack.hpp"
#include <iostream>
#include <string>
#include <limits>
#include "binary_tree_search_items.hpp"
using namespace std;


int main()
{
    PathStack robotPath;

    std::cout << "Robot navigating to item...\n";
    robotPath.push("FORWARD");
    robotPath.push("FORWARD");
    robotPath.push("LEFT");
    robotPath.push("FORWARD");
    robotPath.push("RIGHT");

    robotPath.retracePath();

    return 0;





    // Warehouse item search and management module - joash
    W_itemBTS Witem;


    while (true)
    {
        cout << "\n" << endl;
        cout << "Welcome to the Warehouse Inventory Management System!" << endl;
        cout << "This system allows you to manage your warehouse inventory efficiently." << endl;
        cout << "Here are some of the features you can use:" << endl;
        cout << "1. Add new items to the warehouse." << endl;
        cout << "2. Search for items in the warehouse." << endl;
        cout << "3. Update item information." << endl;
        cout << "4. Remove items from the warehouse." << endl;
        cout << "5. Display all items in the warehouse." << endl;
        cout << "6. Exit the system." << endl;
        int WarehouseChoice;
        cin >> WarehouseChoice;

        if (cin.fail())
        {
            cout << "\nIncorrect format. Please enter a number ." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (WarehouseChoice == 1)
        {
            cout << "You have chosen to add a new item to the warehouse." << endl;
            cout << "Please enter the item details as below:" << endl;
            cout << "Item name: ";
            string itemName;
            cin >> itemName;
            cout << "Item location (Format: Aisle-[Letter][number] e.g., Aisle-A1): ";
            string itemLocation;
            cin >> itemLocation;
            cout << "Item quantity: ";
            int itemQuantity;
            cin >> itemQuantity;


            Witem.insertW_item(itemName, itemLocation, itemQuantity);
        }
        else if (WarehouseChoice == 2)
        {
            cout << "You have chosen to search for an item in the warehouse." << endl;
            Witem.searchItem();
        }
        else if (WarehouseChoice == 3)
        {
            cout << "You have chosen to update item information." << endl;
            Witem.updateW_item();
        }
        else if (WarehouseChoice == 4)
        {
            cout << "You have chosen to remove an item from the warehouse." << endl;
            Witem.deleteW_item();
        }
        else if (WarehouseChoice == 5)
        {
            cout << "You have chosen to display all items in the warehouse." << endl;
            Witem.displayW_item();
        }
        else if (WarehouseChoice == 6)
        {
            cout << "Exiting the system. Goodbye!" << endl;
            return 0;
        }
        else
        {
            cout << "\nInvalid choice. Please try again with number between 1 and 6." << endl;
        }
    }

}












