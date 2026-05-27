#include "path_stack.hpp"
#include <iostream>

PathStack::PathStack() {
    top = nullptr;
    stepCount = 0;
}

PathStack::~PathStack() {
    while (!isEmpty()) {
        pop();
    }
}

bool PathStack::isEmpty() {
    return top == nullptr;
}

void PathStack::push(std::string dir) {
    Step* n = new Step(dir);
    n->nxt = top;
    top = n;
    stepCount++;
}

std::string PathStack::pop() {
    if (isEmpty()) return ""; 
    Step* temp = top;
    std::string val = temp->direct;
    top = top->nxt;
    delete temp;
    stepCount--;
    return val;
}

std::string PathStack::peek() {
    if (isEmpty()) return ""; 
    Step* temp = top;
    std::string val = temp->direct;
    return val;
}


void PathStack::retracePath() {
    std::cout << "\nInitiating return:\n";

    if (isEmpty()) {
        std::cout << "Robot is already at the starting point.";
        return;
    }

    //keep popping until we hit the bottom of the stack
    while (!isEmpty()) {
        std::string dir = pop();
        std::string returnDir = "";

        //inverse dir for return path
        if (dir == "FORWARD") {
            returnDir = "BACKWARD";
        } 
        else if (dir == "BACKWARD") {
            returnDir = "FORWARD";
        } 
        else if (dir == "LEFT") {
            returnDir = "RIGHT";
        } 
        else if (dir == "RIGHT") {
            returnDir = "LEFT";
        } 
        else {
            returnDir = dir; 
        }

        std::cout << "Moving: " << returnDir << "\n";
    }
    
    std::cout << "Robot has returned.\n";
}
