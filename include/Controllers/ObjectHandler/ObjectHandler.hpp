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
    public:

    ObjectHandler() {
      /** Initialize the Data Management System */
      Objects[GameObject::Types::DISK] = std::vector<GameObject*>();
      Objects[GameObject::Types::ROLLER] = std::vector<GameObject*>();
      Objects[GameObject::Types::GOAL] = std::vector<GameObject*>();
      /*TODO: Create and develop the Other Object Types */
    }
    ~ObjectHandler() {

    }
    TerriBull::GameObject* query(GameObject::Types type, char identifier) {return nullptr;} /*TODO*/
    TerriBull::GameObject* getClosestObj(GameObject::Types type, Vector2 pos) {return nullptr;} /*TODO*/
    void updateObjPos(char identifier,GameObject::Types type, Vector2 pos) {} /*TODO*/
    void addObject(GameObject* _obj) {} /*TODO*/
    void update(GameObject::Types type, char id, void* args) {
      GameObject* obj = this->query(type, id); // Find the GameObject with the specified type and id
      if(obj != nullptr) { // If the object exists
          switch(type) {
              case GameObject::Types::DISK: {
                  Disk* disk = dynamic_cast<Disk*>(obj);
                  if (disk != nullptr) {
                    Disk::UpdateArgs* diskArgs = static_cast<Disk::UpdateArgs*>(args);
                    disk->pos->x = diskArgs->x;
                    disk->pos->y = diskArgs->y;
                  }
                  break;
              }
              case GameObject::Types::GOAL: {
                  Goal* goal = dynamic_cast<Goal*>(obj);
                  if (goal != nullptr) {
                    Goal::UpdateArgs* goalArgs = static_cast<Goal::UpdateArgs*>(args);
                    goal->pos->x = goalArgs->x;
                    goal->pos->y = goalArgs->y;
                    goal->setDTheta(goalArgs->dTheta);
                  }
                  break;
              }
              case GameObject::Types::ROLLER: {
                  FieldRoller* roller = dynamic_cast<FieldRoller*>(obj);
                  if (roller != nullptr) {
                    FieldRoller::UpdateArgs* rollerArgs = static_cast<FieldRoller::UpdateArgs*>(args);
                    roller->pos->x = rollerArgs->x;
                    roller->pos->y = rollerArgs->y;
                    roller->setCurrentColor((FieldRoller::Color)rollerArgs->color);
                    roller->setIsInContact(rollerArgs->is_in_contact);
                  }
                  break;
              }
              default:
                  // Handle unsupported types or throw an exception
                  break;
          }
      }
    }
};

// TerriBull::GameObject* TerriBull::ObjectHandler::getClosestObj (GameObject::Types type, TerriBull::Vector2 pos) {
//    return nullptr;
// }

// TerriBull::GameObject* TerriBull::ObjectHandler::query(::std::string identifier) {
//   return nullptr;
// }


#endif // OBJECTHANDLER_H
