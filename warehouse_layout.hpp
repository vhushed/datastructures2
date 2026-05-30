#ifndef WAREHOUSE_LAYOUT_HPP
#define WAREHOUSE_LAYOUT_HPP

#include "path_stack.hpp"
#include <string>

class WarehouseLayout {
public:
    // Parse "D2" or "Aisle-D2" into coordinates (x=3, y=1)
    bool parseLocation(const std::string& location, int& outX, int& outY);

    // Generate a path from base (2,-1) to (destX, destY),
    // pushing each direction step onto the given PathStack.
    void generatePath(int destX, int destY, PathStack& path,
                      int robotID, const std::string& itemName);

    // 2D grid view with obstacle marker
    void displayGrid();

    // Fixed obstacle: fallen crate blocking entrance to aisle B
    static const int OBSTACLE_X = 1;   // Zone B
    static const int OBSTACLE_Y = 0;   // entrance at row 1 level
};

#endif
