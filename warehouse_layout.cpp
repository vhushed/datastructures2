#include "warehouse_layout.hpp"
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// parse coordinates
bool WarehouseLayout::parseLocation(const string& location, int& outX, int& outY) {
    if (location.empty()) return false;

    char zoneChar = 0;
    string digits;
    bool foundLetter = false;

    for (size_t i = 0; i < location.length(); i++) {
        char c = location[i];
        if (isalpha(c)) {
            zoneChar = toupper(c);
            foundLetter = true;
            digits.clear();
        } else if (isdigit(c) && foundLetter) {
            digits += c;
        }
    }

    if (!foundLetter || digits.empty()) return false;
    if (zoneChar < 'A' || zoneChar > 'E') return false;

    int shelfNum = stoi(digits);
    if (shelfNum < 1 || shelfNum > 5) return false;

    outX = zoneChar - 'A';     // A=0, B=1, C=2, D=3, E=4
    outY = shelfNum - 1;       // 1->0, 2->1, 3->2, 4->3, 5->4
    return true;
}

// generate path from base (2,-1) to destination (destX, destY)
void WarehouseLayout::generatePath(int destX, int destY, PathStack& path,
                                    int robotID, const string& itemName) {
    const int baseX = 2;
    int cx = baseX;          // current x (zone)
    int cy = -1;             // current y (row): -1=base, 0=corridor, 1..5=shelf

    cout << "   +---------------------------------------+\n";
    cout << "   |  FORWARD PATH (to destination)        |\n";
    cout << "   +---------------------------------------+\n";

    int s = 1;

    // 1: move from start to base corridor
    cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
         << " row " << cy << " -> row " << (cy+1) << "): base -> corridor\n";
    path.push("FORWARD");
    cy = 0;

    // 2: move to the correct zone (x-axis)
    int dx = destX - cx;

    if (dx < 0) {
        cout << "   [" << s++ << "] LEFT     (Zone " << char('A'+cx)
             << "): turn toward Zone " << char('A'+destX) << "\n";
        path.push("LEFT");

        for (int i = 0; i < -dx; i++) {
            int nextX = cx - 1;
            cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
                 << " -> Zone " << char('A'+nextX) << "): along corridor\n";
            path.push("FORWARD");
            cx = nextX;
        }

        cout << "   [" << s++ << "] RIGHT    (Zone " << char('A'+cx)
             << "): turn to face aisle\n";
        path.push("RIGHT");
    }
    else if (dx > 0) {
        cout << "   [" << s++ << "] RIGHT    (Zone " << char('A'+cx)
             << "): turn toward Zone " << char('A'+destX) << "\n";
        path.push("RIGHT");

        for (int i = 0; i < dx; i++) {
            int nextX = cx + 1;
            cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
                 << " -> Zone " << char('A'+nextX) << "): along corridor\n";
            path.push("FORWARD");
            cx = nextX;
        }

        cout << "   [" << s++ << "] LEFT     (Zone " << char('A'+cx)
             << "): turn to face aisle\n";
        path.push("LEFT");
    }
    else {
        cout << "   [" << s++ << "] (Already at Zone " << char('A'+cx)
             << " -- no lateral move needed)\n";
    }

    // 3: enter the aisle and move up to the correct shelf row (y-axis)
    if (destX != OBSTACLE_X) {
        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " row " << cy << " -> row " << (cy+1) << "): enter aisle\n";
        path.push("FORWARD");
        cy = 1;

    } else {
        // obstacle at entrance of aisle B
        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " row " << cy << " -> row " << (cy+1) << "): attempt to enter...\n";
        path.push("FORWARD");
        cy = 1;

        cout << "       >>> OBSTACLE DETECTED! Entrance to aisle "
             << char('A'+OBSTACLE_X)
             << " blocked by fallen crate. <<<\n";

        path.backtrack();
        cy = 0;

        cout << "       Taking detour via Zone " << char('A'+destX+1) << "...\n";

        cout << "   [" << s++ << "] RIGHT    (Zone " << char('A'+cx)
             << "): move toward Zone " << char('A'+cx+1) << "\n";
        path.push("RIGHT");

        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " -> Zone " << char('A'+cx+1) << "): along corridor\n";
        path.push("FORWARD");
        cx = cx + 1;

        cout << "   [" << s++ << "] LEFT     (Zone " << char('A'+cx)
             << "): turn to face aisle\n";
        path.push("LEFT");

        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " row " << cy << " -> row " << (cy+1) << "): enter clear aisle\n";
        path.push("FORWARD");
        cy = 1;

        // go back to the correct zone
        cout << "   [" << s++ << "] LEFT     (Zone " << char('A'+cx)
             << "): turn to face Zone " << char('A'+destX) << "\n";
        path.push("LEFT");

        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " -> Zone " << char('A'+destX) << " row " << cy
             << "): cross into target zone\n";
        path.push("FORWARD");
        cx = destX;

        cout << "   [" << s++ << "] RIGHT    (Zone " << char('A'+cx)
             << "): turn to face up the aisle\n";
        path.push("RIGHT");

        cout << "       Successfully passed the obstacle.\n";
    }

    // 4: move up the aisle to the correct shelf row
    for (int i = 0; i < destY; i++) {
        int nextY = cy + 1;
        cout << "   [" << s++ << "] FORWARD  (Zone " << char('A'+cx)
             << " row " << cy << " -> row " << nextY
             << "): approaching shelf " << char('A'+cx) << nextY << "\n";
        path.push("FORWARD");
        cy = nextY;
    }

    cout << "\n[NAVIGATION] Robot #" << robotID
         << " has arrived at: " << itemName
         << " (Shelf " << char('A'+destX) << (destY+1) << ")\n";
}

// show warehouse layout in grid
void WarehouseLayout::displayGrid() {
    const int baseX = 2;

    cout << "\n";
    cout << "========================================\n";
    cout << "        WAREHOUSE LAYOUT (GRID)\n";
    cout << "========================================\n\n";

    cout << "      A       B       C       D       E\n";
    cout << "  +-------+-------+-------+-------+-------+\n";

    // Shelves: rows 5 down to 1
    for (int row = 4; row >= 0; row--) {
        cout << (row + 1) << " |";
        for (int col = 0; col < 5; col++) {
            cout << "  " << char('A' + col) << (row + 1) << "   ";
            cout << "|";
        }
        cout << "\n";

        // After row 1: obstacle marker between corridor and aisle entrance
        if (row == 0) {
            cout << "  +";
            for (int col = 0; col < 5; col++) {
                if (col == OBSTACLE_X) {
                    cout << "--[!]--+";
                } else {
                    cout << "-------+";
                }
            }
            cout << "  <-- aisle entrance\n";
        } else {
            cout << "  +-------+-------+-------+-------+-------+\n";
        }
    }

    // Row 0: corridor
    cout << "0 |";
    for (int col = 0; col < 5; col++) {
        cout << "  ---  ";
        cout << "|";
    }
    cout << "  <-- corridor\n";

    cout << "  +-------+-------+-------+-------+-------+\n";

    // Row -1: base (only under Zone C)
    cout << "-1|";
    for (int col = 0; col < 5; col++) {
        if (col == baseX) {
            cout << "  BASE ";
        } else {
            cout << "       ";
        }
        cout << "|";
    }
    cout << "  <-- robot start\n";

    cout << "  +-------+-------+-------+-------+-------+\n\n";
    cout << "  [!]  = Obstacle (fallen crate at entrance of aisle "
         << char('A' + OBSTACLE_X) << ")\n";
    cout << "  BASE = Robot starting position (under Zone C, row -1)\n";
    cout << "========================================\n";
}
