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
            RED, BLUE
    } Color;

    private:
    float pDTheta;
    Color pColor; 
    bool valid;

    ~Goal() {}

    public:
    
    struct UpdateArgs {
        float x;
        float y;
        float dTheta;
        bool valid;
    };

    static void* ConstructUpdateArgs(float x, float y, float dtheta, bool valid) {
        UpdateArgs* args = (UpdateArgs*) malloc(sizeof(UpdateArgs));
        args->x = x;
        args->y = y;
        args->dTheta = dtheta;
        args->valid = valid;
        return (void*)args;
    }
    
    Goal(TerriBull::Vector2* pos, char identifier, Goal::Color color) : GameObject(pos, identifier, DISK, 8, 8), pColor(color), valid(false) { }
    
    float getDTheta() {
        return pDTheta;
    }
    bool getValid() {
        return valid;
    }
    void setDTheta(float pDTheta) {
        this->pDTheta = pDTheta;
    }
    void setValid(bool valid) {
        this->valid = valid;
    }

};

#endif