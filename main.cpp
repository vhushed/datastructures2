#include "path_stack.hpp"
#include <iostream>

int main() {
    PathStack robotPath;

    std::cout << "Robot navigating to item...\n";
    robotPath.push("FORWARD");
    robotPath.push("FORWARD");
    robotPath.push("LEFT");
    robotPath.push("FORWARD");
    robotPath.push("RIGHT");

    robotPath.retracePath();

    return 0;
}