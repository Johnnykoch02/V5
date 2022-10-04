#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../TerriBull.hpp"

#include <string>

class TerriBull::GameObject {
    private:
    public:
    std::string identifier;
    float *x;
    float width;
    float height;
    float * y;
    int type;
    ~GameObject() {
        delete this->x;
        delete this->y;
    }
    GameObject() {
        this->x = new float;
        this->y = new float;
    }
    GameObject(float x, float y, std::string identifier, int type, float width, float height) : GameObject() {
        this->x[0] = x;
        this->y[0] = y;
        this->identifier = identifier;
    }

    void setX(float x) {    
        this->x[0] = x;
    }
    
    void setY(float y) {
        this->y[0] = y;
    }

    bool checkID(std::string identifier) { 
        if (this->identifier != identifier) return false;

        return true;
    }

};

#endif