#ifndef BINARY_TREE_SEARCH_ITEMS_HPP
#define BINARY_TREE_SEARCH_ITEMS_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;



struct WarehouseItem
{
    int W_itemID;
    string W_itemName;
    string W_location;
    int W_quantity;

    WarehouseItem* left;
    WarehouseItem* right;
};


class W_itemBTS{

    private:
        WarehouseItem* root;

        WarehouseItem* createW_item(int id, string name, string location, int quantity);

        WarehouseItem* insertW(WarehouseItem*root, int ID, string name, string location, int quantity);

        bool nameExists(WarehouseItem* root, string name);
        
        void displayW_item(WarehouseItem* root);

        string toLowerCase(string str);

    public:
        W_itemBTS();

        void insertW_item(string name, string location, int quantity);

        void displayW_item();

        WarehouseItem* search(WarehouseItem* root, int id);

        void searchItem();
        
        void deleteW_item();

        void updateW_item();

        

};

class BinaryTree {

    private:
        WarehouseItem* root;

    public:

        BinaryTree();

        void insertItem(int id, string name, string location);

        void displayItems();

        void loadCSV(string filename);
};



#endif
