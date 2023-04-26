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
#ifndef FIELD_ROLLER_H
#define FIELD_ROLLER_H

#include "../GameObject.hpp"

#include <string>


class FieldRoller : public TerriBull::GameObject { /*TODO: Change name not to conflict with Mech-Sys Roller*/
    public:
    typedef enum {
            RED, BLUE, NEUTRAL
    } Color;

    struct UpdateArgs {
        float x;
        float y;
        int color;
        bool is_in_contact;
    };

    static void* ConstructUpdateArgs(float x, float y, int color, bool is_in_contact) {
        UpdateArgs* data = (UpdateArgs*) malloc(sizeof(UpdateArgs));
        data->x = x;
        data->y = y;
        data->color = color;
        data->is_in_contact = is_in_contact;
        return data;
    }

    private:
    Color pCurrentColor;
    bool pIsInContact;
    
    ~FieldRoller() {
        
    }
    public:

    FieldRoller(TerriBull::Vector2* pos, char identifier) : GameObject(pos, identifier, ROLLER, 6, 1) { }
    
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