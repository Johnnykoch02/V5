#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include "../TeriBull/TerriBull.hpp"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <math.h>

class TerriBull::ObjectHandler { 
    public:
      std::list<TerriBull::GameObject> Objects;
      const std::map<int, int> MaxOfTypes = {
        {2, 1}, //REDGOAL
        {6, 1}, //BLUEGOAL
    
    };
    const std::map<int, std::vector<float>> WidthHeights = {
        {2, {13.5,13.5}}, //REDGOAL
        {6, {13.5,13.5}} //BLUEGOAL
      /* ADD MORE */
    };

    ObjectHandler();
    GameObject* query(std::string identifier);
    GameObject* getClosestObj(int type, float x, float y);
    void updateObjPos(std::string identifier,int type, float x, float y);

    void addObject(std::string identifier, int type, float x, float y);




};
