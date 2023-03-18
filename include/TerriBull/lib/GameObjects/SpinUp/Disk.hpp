/**
 * @file Disk.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Disk Game Object Class for containing Disk Specific information
 *     
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISK_H
#define DISK_H

#include "../GameObject.hpp"

#include <string>


class Disk : public TerriBull::GameObject {
    private:
    public:
    
    ~Disk() {
        
    }
    Disk(Vector2* pos, byte identifier) :GameObject(pos, identifier, DISK, 5, 5) { }

};

#endif