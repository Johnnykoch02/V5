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
        
    ~Goal() {
        
    }
    public:
    Goal(Vector2* pos, byte identifier) :GameObject(pos, identifier, GOAL, 8, 8) { }
    
    float getDTheta() {
        return pDTheta;
    }
    
    void setDTheta(float pDTheta) {
        this->pDTheta = pDTheta;
    }

};

#endif