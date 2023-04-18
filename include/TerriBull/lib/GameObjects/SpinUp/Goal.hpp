/**
 * @file Goal.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Goal Object Class for containing Roller Specific Information
 *     
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef GOAL_H
#define GOAL_H

#include "../GameObject.hpp"

#include <string>


class Goal : public TerriBull::GameObject {
    public:
    typedef enum {
            RED, BLUE, NEUTRAL
    } Color;

    private:
    float pDTheta;
        
    ~Goal() {}

    public:
    
    struct UpdateArgs {
        float x;
        float y;
        float dTheta;
    };
    
    static void* ConstructUpdateArgs(float x, float y, float dtheta) {
        size_t size = sizeof(float) * 3; /* Calculate Size*/
        void* data = malloc(size);
        size_t offset = 0;
        memcpy((char*)data + offset, &x, sizeof(float));
        offset += sizeof(float);
        memcpy((char*)data + offset, &y, sizeof(float));
        offset += sizeof(float);
        memcpy((char*)data + offset, &dtheta, sizeof(bool));
        return data;
    }
    
    Goal(TerriBull::Vector2* pos, char identifier) : GameObject(pos, identifier, DISK, 8, 8) { }
    
    float getDTheta() {
        return pDTheta;
    }
    
    void setDTheta(float pDTheta) {
        this->pDTheta = pDTheta;
    }

};

#endif