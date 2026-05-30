#ifndef PATH_STACK_HPP
#define PATH_STACK_HPP

#include "structs.hpp" 
#include <string>

class PathStack {
private:
    Step* top;
    int stepCount; 

public:
    PathStack();
    ~PathStack();

    void push(std::string dir);
    std::string pop();
    bool isEmpty();

    bool backtrack();     // pop one step (obstacle handling)
    void retracePath();
};



#endif