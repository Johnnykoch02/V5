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
#include "../../TerriBull/lib/GameObjects/SpinUp/Disk.hpp"
#include "../../TerriBull/lib/GameObjects/SpinUp/FieldRoller.hpp"
#include "../../TerriBull/lib/GameObjects/SpinUp/Goal.hpp"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <math.h>

class TerriBull::ObjectHandler { 
    private:
    std::map<GameObject::Types, std::vector<GameObject*>> Objects;
    size_t pNumObjects;
    public:

    ObjectHandler() : pNumObjects(0) {
      /** Initialize the Data Management System */
      Objects[GameObject::Types::DISK] = std::vector<GameObject*>();
      Objects[GameObject::Types::ROLLER] = std::vector<GameObject*>();
      Objects[GameObject::Types::GOAL] = std::vector<GameObject*>();
      /*TODO: Create and develop the Other Object Types */
    }
    ~ObjectHandler() {

    }
    TerriBull::GameObject* query(GameObject::Types type, char identifier) {return nullptr;} /*TODO*/
    TerriBull::GameObject* getClosestObjByType(GameObject::Types type, Vector2* currentPos);
    TerriBull::GameObject* getClosestObj(Vector2* currentPos);
    void updateObjPos(char identifier,GameObject::Types type, Vector2 pos); /*TODO*/
    void addObject(GameObject* _obj); /*TODO*/
    void update(GameObject::Types type, char id, void* args); 
};

// TerriBull::GameObject* TerriBull::ObjectHandler::getClosestObj (GameObject::Types type, TerriBull::Vector2 pos) {
//    return nullptr;
// }

// TerriBull::GameObject* TerriBull::ObjectHandler::query(::std::string identifier) {
//   return nullptr;
// }


#endif // OBJECTHANDLER_H
