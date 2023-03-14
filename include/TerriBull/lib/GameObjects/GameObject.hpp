/**
 * @file GameObject.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Abstract Data structure for implementing game objects on the field
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
        DISK, ROBOT, ROLLER, GOAL, LOWER_GOAL
    } Types;

    byte id;
    Vector2* pos;
    float width;
    float height;
    Types type;
    
    ~GameObject() {
        
    }

    GameObject(Vector2* pos, byte identifier, Types type, float width, float height) : pos(pos), id(identifier), type(type), width(width), height(height) { }
    

    virtual void setPos(Vector2* pos) final {
        this->pos = pos;
    }

    virtual Vector2* getPos() const final {
        return new Vector2(this->pos);
    }

    virtual Vector2* getPosPtr() final { return this->pos; }
    

    virtual bool checkID(byte identifier) final { 
        return this->id == identifier;
    }

};

#endif