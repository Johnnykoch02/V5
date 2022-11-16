#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../TerriBull.hpp"

#include <string>

class TerriBull::GameObject {
    private:
    public:
    TerriBull::string identifier;
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
    GameObject(float x, float y, TerriBull::string identifier, int type, float width, float height) : GameObject() {
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

    bool checkID(TerriBull::string identifier) { 
        if (this->identifier != identifier) return false;

        return true;
    }

};

#endif