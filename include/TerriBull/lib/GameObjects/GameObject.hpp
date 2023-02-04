/**
 * @file GameObject.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Data structure for implementing game objects on the field
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../TerriBull.hpp"

#include <string>


class TerriBull::GameObject {
    private:
    public:
    typedef enum {
        DISK, ROBOT, ROLLER
    } Types;

    TerriBull::string identifier;
    Vector2 pos;
    float width;
    float height;
    Types type;
    
    ~GameObject() {
        
    }
    GameObject() {
        
    }
    GameObject(Vector2 pos, TerriBull::string identifier, int type, float width, float height) : GameObject() {
        this->pos = pos;
        this->identifier = identifier;
    }

    void setPos(Vector2 pos) {
        this->pos = pos;
    }

    bool checkID(TerriBull::string identifier) { 
        if (this->identifier != identifier) return false;

        return true;
    }

};

#endif