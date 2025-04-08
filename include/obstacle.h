#ifndef OBSTACLE_H
#define OBSTACLE_H

struct Obstacle {
    int x, y;
    bool passed = false;
    Obstacle(int _x, int _y) : x(_x), y(_y) {}
};

#endif 