/**
 * @file Roller.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Roller Game Object Class for containing Roller Specific Information
 *     
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ROLLER_H
#define ROLLER_H

#include "../GameObject.hpp"

#include <string>


class Roller : public TerriBull::GameObject {
    public:
    typedef enum {
            RED, BLUE, NEUTRAL
    } Color;

    private:
    Color pCurrentColor;
    bool pIsInContact;
    
    ~Roller() {
        
    }
    public:
    Roller(Vector2 pos, byte identifier) :GameObject(pos, identifier, ROLLER, 6, 1) { }
    
    bool isInContact() {
        return pIsInContact;
    }

    Color getCurrentColor() {
        return pCurrentColor;
    }
    
    void setCurrentColor(Color color) {
        pCurrentColor = color;
    }
    
    void setIsInContact(bool isInContact) {
        pIsInContact = isInContact;
    }

};

#endif