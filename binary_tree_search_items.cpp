#include "binary_tree_search_items.hpp"
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>



string W_itemBTS::toLowerCase(string str)
{
    for (char &c : str)
    {
        c = tolower(c);
    }

    return str;
}


W_itemBTS::W_itemBTS()
{
    root = NULL;
}

WarehouseItem* W_itemBTS::createW_item(int id,string name, string location, int quantity)
{
    WarehouseItem* newItem = new WarehouseItem;
    newItem->W_itemID = id;
    newItem->W_itemName = name;
    newItem->W_location = location;
    newItem->W_quantity = quantity;
    newItem->left = NULL;
    newItem->right = NULL;

    return newItem;
}

WarehouseItem* W_itemBTS::insertW(WarehouseItem* root, int ID, string name, string location, int quantity)
{
    if (root == NULL)
    {
        return createW_item(ID, name, location, quantity);
    }

    if (ID < root->W_itemID)
    {
        root->left = insertW(root->left, ID, name, location, quantity);
    }
    else if (ID > root->W_itemID)
    {
        root->right = insertW(root->right, ID, name, location, quantity);
    }

    return root;
}

bool W_itemBTS::nameExists(WarehouseItem* root, string name)
{
    if (root == NULL) return false;
    if (root->W_itemName == name) return true;
    
    return nameExists(root->left, name) || nameExists(root->right, name);
}

void W_itemBTS::insertW_item(string name, string location, int quantity)
{
    int lastID = 100; 
    bool duplicateFound = false;

    
    ifstream fileIn("Item.csv");
    if (fileIn.is_open())
    {
        string line;
        
        getline(fileIn, line); 

        while (getline(fileIn, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, currentName, currentLocation, currentQty;

            
            if (getline(ss, idStr, ',') && 
                getline(ss, currentName, ',') && 
                getline(ss, currentLocation, ',') && 
                getline(ss, currentQty, ','))
            {
                
                try {
                    int currentID = stoi(idStr);
                    if (currentID > lastID) {
                        lastID = currentID;
                    }
                } catch (...) {}

                
                if (toLowerCase(currentName) == toLowerCase(name))
                {
                    duplicateFound = true;
                    break;
                }
            }
        }
        fileIn.close();
    }

    
    if (duplicateFound)
    {
        cout << "\n[ERROR] Creation Canceled: An item named '" << name << "' already exists in warehouse!" << endl;
        return; 
    }

    
    int newID = lastID + 1;

    
    root = insertW(root, newID, name, location, quantity);

    
    ofstream fileOut("Item.csv", ios::app);
    if (fileOut.is_open())
    {
        
        ifstream fileCheck("Item.csv", ios::ate);
        if (fileCheck.is_open()) {
            if (fileCheck.tellg() > 0) {
                fileCheck.seekg(-1, ios::end);
                char lastChar;
                fileCheck.get(lastChar);
                if (lastChar != '\n') {
                    fileOut << "\n"; 
                }
            }
            fileCheck.close();
        }

        fileOut << newID << "," << name << "," << location << "," << quantity << "\n";
        fileOut.close();
        cout << "\n[SUCCESS] Item successfully created with ID: " << newID << endl;
    }
    else
    {
        cout << "\n[WARNING] Item added to tree structure, but failed to write to database!" << endl;
    }
}

void W_itemBTS::displayW_item()
{
    ifstream file("Item.csv");

    if (!file.is_open())
    {
        cout << "\n[ERROR] Could not open database to display inventory." << endl;
        return;
    }

    string line;
    getline(file, line);


    cout << "\n" << string(65, '=') << endl;
    cout << left << setw(10) << "Item ID" 
         << setw(20) << "Item Name" 
         << setw(20) << "Location" 
         << setw(10) << "Quantity" << endl;
    cout << string(65, '-') << endl;

    int recordCount = 0;


    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, name, location, qtyStr;

        if (getline(ss, idStr, ',') && 
            getline(ss, name, ',') && 
            getline(ss, location, ',') && 
            getline(ss, qtyStr, ','))
        {
            cout << left << setw(10) << idStr 
                 << setw(20) << name 
                 << setw(20) << location 
                 << setw(10) << qtyStr << endl;
            
            recordCount++;
        }
    }

    file.close();

    cout << string(65, '=') << endl;
    cout << "Total unique items found: " << recordCount << endl;
}

void W_itemBTS::deleteW_item()
{
    ifstream fileIn("Item.csv");
    if (!fileIn.is_open())
    {
        cout << "\n[ERROR] Could not open database to read stock." << endl;
        return;
    }

    string line;
    string header;
    getline(fileIn, header); 

    WarehouseItem* head = NULL;
    WarehouseItem* tail = NULL;
    int totalItems = 0;
  
    while (getline(fileIn, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, locationStr, qtyStr;

        if (getline(ss, idStr, ',') && 
            getline(ss, nameStr, ',') && 
            getline(ss, locationStr, ',') && 
            getline(ss, qtyStr, ','))
        {
            
            WarehouseItem* newNode = new WarehouseItem();
            newNode->W_itemID = stoi(idStr);
            newNode->W_itemName = nameStr;
            newNode->W_location = locationStr;
            newNode->W_quantity = stoi(qtyStr);
            newNode->left = NULL;  
            newNode->right = NULL; 

            totalItems++;
            if (head == NULL)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->right = newNode;
                tail = newNode;
            }
        }
    }
    fileIn.close();

    if (head == NULL)
    {
        cout << "\nThe warehouse inventory is currently empty! Nothing to delete." << endl;
        return;
    }
   
    cout << "\n--- Available Stock to Delete ---" << endl;
    cout << left << setw(6) << "No." 
         << setw(10) << "Item ID" 
         << setw(20) << "Item Name" 
         << setw(20) << "Location" 
         << setw(10) << "Quantity" << endl;
    cout << string(66, '-') << endl;

    WarehouseItem* curr = head;
    int index = 1;
    while (curr != NULL)
    {
        cout << left << setw(6) << index 
             << setw(10) << curr->W_itemID 
             << setw(20) << curr->W_itemName 
             << setw(20) << curr->W_location 
             << setw(10) << curr->W_quantity << endl;
        curr = curr->right;
        index++;
    }
    cout << string(66, '=') << endl;

    
    cout << "Enter the selection number to delete (1 to " << totalItems << "): ";
    int selection;
    cin >> selection;

    
    if (cin.fail() || selection < 1 || selection > totalItems)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n[ERROR] Invalid selection number. Returning to main menu." << endl;
        
        
        while (head != NULL) {
            WarehouseItem* nextNode = head->right;
            delete head;
            head = nextNode;
        }
        return;
    }

    
    curr = head;
    for (int i = 1; i < selection; i++)
    {
        curr = curr->right;
    }
    string selectedName = curr->W_itemName;

    
    cout << "\nAre you sure you want to delete [" << selectedName << "] and its related data?" << endl;
    cout << "1. Yes\n2. No\nEnter choice: ";
    int confirmChoice;
    cin >> confirmChoice;

    if (cin.fail() || confirmChoice != 1)
    {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "\nDeletion canceled. Returning to main menu interface." << endl;
        
        // Memory Cleanup
        while (head != NULL) {
            WarehouseItem* nextNode = head->right;
            delete head;
            head = nextNode;
        }
        return;
    }

    
    ofstream fileOut("Item.csv", ios::trunc); 
    if (!fileOut.is_open())
    {
        cout << "\n[ERROR] System failure: Could not update Item.csv file directly!" << endl;
        return;
    }

    fileOut << header << "\n";

    curr = head;
    index = 1;
    while (curr != NULL)
    {
        if (index != selection)
        {
            fileOut << curr->W_itemID << "," 
                    << curr->W_itemName << "," 
                    << curr->W_location << "," 
                    << curr->W_quantity << "\n";
        }
        curr = curr->right;
        index++;
    }
    fileOut.close();

    
    while (head != NULL)
    {
        WarehouseItem* nextNode = head->right;
        delete head; 
        head = nextNode;
    }

    cout << "\n[SUCCESS] Item [" << selectedName << "] and its records have been successfully deleted!" << endl;
}

void W_itemBTS::searchItem()
{
    cout << "\n===============================" << endl;
    cout << "       SEARCH INVENTORY        " << endl;
    cout << "===============================" << endl;
    cout << "1. Search by Item ID" << endl;
    cout << "2. Search by Item Name" << endl;
    cout << "3. Search by Quantity (Filters)" << endl;
    cout << "4. Search by Aisle / Location" << endl;
    cout << "Enter your choice: ";
    
    int searchChoice;
    cin >> searchChoice;

    if (cin.fail() || searchChoice < 1 || searchChoice > 4)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n[ERROR] Invalid choice. Returning to main menu." << endl;
        return;
    }

    string searchID = "", searchName = "", searchAisle = "";
    int searchQtyThreshold = 0;
    int qtyMode = 0;


    if (searchChoice == 1)
    {
        cout << "Enter Item ID to look for: ";
        cin >> searchID;
    }
    else if (searchChoice == 2)
    {
        cout << "Enter Item Name to look for: ";
        cin >> searchName;
    }
    else if (searchChoice == 3)
    {
        cout << "\n1. Less than a quantity" << endl;
        cout << "2. More than a quantity" << endl;
        cout << "Enter mode: ";
        cin >> qtyMode;
        
        if (cin.fail() || qtyMode < 1 || qtyMode > 2)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[ERROR] Invalid layout selection." << endl;
            return;
        }
        cout << "Enter quantity value: ";
        cin >> searchQtyThreshold;
    }
    else if (searchChoice == 4)
    {
        cout << "Enter Aisle ID (e.g., Aisle-A1): ";
        cin >> searchAisle;
    }


    ifstream file("Item.csv");
    if (!file.is_open())
    {
        cout << "\n[ERROR] Database file Item.csv could not be opened." << endl;
        return;
    }

    string line;
    string header;
    getline(file, header);

    cout << "\n" << string(65, '=') << endl;
    cout << left << setw(10) << "Item ID" 
         << setw(20) << "Item Name" 
         << setw(20) << "Location" 
         << setw(10) << "Quantity" << endl;
    cout << string(65, '-') << endl;

    int matchCount = 0;

    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, locationStr, qtyStr;

        if (getline(ss, idStr, ',') && 
            getline(ss, nameStr, ',') && 
            getline(ss, locationStr, ',') && 
            getline(ss, qtyStr, ','))
        {
            bool isMatch = false; 
            int currentQty = stoi(qtyStr); 

            if (searchChoice == 1 && idStr == searchID)
            {
                isMatch = true;
            }
            else if (searchChoice == 2 && toLowerCase(nameStr) == toLowerCase(searchName))
            {
                isMatch = true;
            }
            else if (searchChoice == 3)
            {
                if (qtyMode == 1 && currentQty < searchQtyThreshold) isMatch = true;
                if (qtyMode == 2 && currentQty > searchQtyThreshold) isMatch = true;
            }
            else if (searchChoice == 4 && toLowerCase(locationStr) == toLowerCase(searchAisle))
            {
                isMatch = true;
            }

            if (isMatch)
            {
                cout << left << setw(10) << idStr 
                     << setw(20) << nameStr 
                     << setw(20) << locationStr 
                     << setw(10) << qtyStr << endl;
                matchCount++;
            }
        }
    }
    file.close();

    cout << string(65, '=') << endl;
    
    if (matchCount == 0)
    {
        cout << "[INFO] No inventory items found matching your search parameters." << endl;
    }
    else
    {
        cout << "Search complete. Total matching records: " << matchCount << endl;
    }

}

void W_itemBTS::updateW_item()
{
    ifstream fileIn("Item.csv");
    if (!fileIn.is_open())
    {
        cout << "\n[ERROR] Could not open Item.csv to read data." << endl;
        return;
    }

    string line, header;
    getline(fileIn, header);

    WarehouseItem* head = NULL;
    WarehouseItem* tail = NULL;
    int totalItems = 0;

    while (getline(fileIn, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, locationStr, qtyStr;

        if (getline(ss, idStr, ',') && 
            getline(ss, nameStr, ',') && 
            getline(ss, locationStr, ',') && 
            getline(ss, qtyStr, ','))
        {
            WarehouseItem* newNode = new WarehouseItem();
            newNode->W_itemID = stoi(idStr);
            newNode->W_itemName = nameStr;
            newNode->W_location = locationStr;
            newNode->W_quantity = stoi(qtyStr);
            newNode->left = NULL;
            newNode->right = NULL;

            totalItems++;
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->right = newNode;
                tail = newNode;
            }
        }
    }
    fileIn.close();

    if (head == NULL)
    {
        cout << "\nThe warehouse inventory is currently empty! Nothing to update." << endl;
        return;
    }

    cout << "\n--- Select Stock Item to Update ---" << endl;
    cout << left << setw(6) << "No." 
         << setw(10) << "Item ID" 
         << setw(20) << "Item Name" 
         << setw(20) << "Location" 
         << setw(10) << "Quantity" << endl;
    cout << string(66, '-') << endl;

    WarehouseItem* curr = head;
    int index = 1;
    while (curr != NULL)
    {
        cout << left << setw(6) << index 
             << setw(10) << curr->W_itemID 
             << setw(20) << curr->W_itemName 
             << setw(20) << curr->W_location 
             << setw(10) << curr->W_quantity << endl;
        curr = curr->right;
        index++;
    }
    cout << string(66, '=') << endl;

    cout << "Enter the item selection number (1 to " << totalItems << "): ";
    int selection;
    cin >> selection;

    if (cin.fail() || selection < 1 || selection > totalItems)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n[ERROR] Invalid selection number. Aborting." << endl;
        
        while (head != NULL) {
            WarehouseItem* nextNode = head->right;
            delete head;
            head = nextNode;
        }
        return;
    }

    WarehouseItem* targetItem = head;
    for (int i = 1; i < selection; i++) {
        targetItem = targetItem->right;
    }

    while (true)
    {
        cout << "\nWhat would you like to change for item ID [" << targetItem->W_itemID << "]?" << endl;
        cout << "1. Item Name" << endl;
        cout << "2. Aisle / Location" << endl;
        cout << "3. Quantity" << endl;
        cout << "Enter choices (1-3): ";
        int fieldChoice;
        cin >> fieldChoice;

        if (cin.fail() || fieldChoice < 1 || fieldChoice > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[ERROR] Invalid field field selection. Try again." << endl;
            continue;
        }

        if (fieldChoice == 1)
        {
            cout << "Enter safe new Item Name: ";
            cin >> targetItem->W_itemName;
        }
        else if (fieldChoice == 2)
        {
            cout << "Enter safe new Aisle / Location (Format: Aisle-[Letter][number] e.g., Aisle-A1): ";
            cin >> targetItem->W_location;
        }
        else if (fieldChoice == 3)
        {
            cout << "Enter safe new Quantity: ";
            cin >> targetItem->W_quantity;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[ERROR] Format violation; value unaltered." << endl;
                continue;
            }
        }

        cout << "\n================= UPDATED STOCK PREVIEW =================" << endl;
        cout << left << setw(10) << "Item ID" 
             << setw(20) << "Item Name" 
             << setw(20) << "Location" 
             << setw(10) << "Quantity" << endl;
        cout << string(60, '-') << endl;
        cout << left << setw(10) << targetItem->W_itemID 
             << setw(20) << targetItem->W_itemName 
             << setw(20) << targetItem->W_location 
             << setw(10) << targetItem->W_quantity << endl;
        cout << string(60, '=') << endl;

        // 6. Loop escape verification validation check sequence
        cout << "Are you done editing this item?" << endl;
        cout << "1. Yes (Save and return to main options)" << endl;
        cout << "2. No (Modify another parameter of this item)" << endl;
        cout << "Enter tracking choice: ";
        int editingDone;
        cin >> editingDone;

        if (cin.fail() || editingDone == 1)
        {
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        }
    }

    ofstream fileOut("Item.csv", ios::trunc);
    if (fileOut.is_open())
    {
        fileOut << header << "\n";
        curr = head;
        while (curr != NULL)
        {
            fileOut << curr->W_itemID << "," 
                    << curr->W_itemName << "," 
                    << curr->W_location << "," 
                    << curr->W_quantity << "\n";
            curr = curr->right;
        }
        fileOut.close();
        cout << "\n[SUCCESS] Records updated successfully on storage media!" << endl;
    }

    while (head != NULL)
    {
        WarehouseItem* nextNode = head->right;
        delete head;
        head = nextNode;
    }
}