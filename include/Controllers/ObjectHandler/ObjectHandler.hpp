/**
 * @file ObjectHandler.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Maintains and manages objects and object locations
 *  in the Field
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include "../../TerriBull/TerriBull.hpp"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <math.h>

class TerriBull::ObjectHandler { 
    private:
    // Vector2 
    public:
      ::std::list<TerriBull::GameObject> Objects;
      const ::std::map<int, int> MaxOfTypes = {
        {2, 1}, //REDGOAL
        {6, 1}, //BLUEGOAL
    };
    const ::std::map<int, ::std::vector<float>> WidthHeights = {
        {2, {13.5,13.5}}, //REDGOAL
        {6, {13.5,13.5}} //BLUEGOAL
      /* ADD MORE */
    };

    ObjectHandler() {}
    ~ObjectHandler() {}
    TerriBull::GameObject* query(::std::string identifier);
    TerriBull::GameObject* getClosestObj(GameObject::Types type, Vector2 pos);
    void updateObjPos(::std::string identifier,GameObject::Types type, Vector2 pos) {}
    void addObject(::std::string identifier, GameObject::Types type, Vector2 pos) {}
    void update() {}
};

TerriBull::GameObject* ObjectHandler::getClosestObj (GameObject::Types type, TerriBull::Vector2 pos) {
   return nullptr;
}

TerriBull::GameObject* ObjectHandler::query(::std::string identifier) {
  return nullptr;
}


#endif // OBJECTHANDLER_H
