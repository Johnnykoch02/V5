/**
 * @file ObjectHandler.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief Controller of all physical components on the BullBot
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../include/Controllers/ObjectHandler/ObjectHandler.hpp"
#include <sstream>
#include <iomanip>

char ObjectHandler::addObject(GameObject* _obj) {
    this->pNumObjects++;
    this->Objects[_obj->type].push_back(_obj);
    char index = static_cast<char>(this->Objects[_obj->type].size() - 1);
    _obj->id = index;
    return index;
}

GameObject* ObjectHandler::getClosestObjByType(GameObject::Types type, Vector2* currentPos) {
    GameObject* closestObj = nullptr;
    float minRadius = INFINITY;
    vector<GameObject*> desired_query = this->Objects[type];
    for( auto it = desired_query.begin(); it!= desired_query.end(); it++) {
            GameObject* currentObj =  *(it);
            Vector2* currentVector = *(currentPos) - *(currentObj->getPosPtr());
            if (closestObj == nullptr || currentVector->r < minRadius) {
                closestObj = currentObj;
                minRadius = currentVector->r;
            }
            delete currentVector;
        }
        return closestObj;
}

 TerriBull::GameObject* ObjectHandler::query(GameObject::Types type, char identifier) {
    std::vector<GameObject*> objects = this->Objects[type];
    for (GameObject* obj : objects) {
            if (obj->checkID(identifier)) { // Check if the object's identifier matches the inputted identifier
                return obj;
            }
        }
    return nullptr;
 }


GameObject* ObjectHandler::getClosestObj(Vector2* currentPos) {
    GameObject* closestObj = nullptr;
    float minRadius = INFINITY;
    for (auto it = this->Objects.begin(); it!= this->Objects.end(); it++) {
        for (auto it2 = it->second.begin(); it2!= it->second.end(); it2++) {
            GameObject* currentObj =  *(it2);
            Vector2* currentVector = *(currentPos) - *(currentObj->getPosPtr());
            if (closestObj == nullptr || currentVector->r < minRadius) {
                closestObj = currentObj;
                minRadius = currentVector->r;
            }
            delete currentVector;
        }
    }
    return closestObj;
}

void ObjectHandler::updateObjPos(char identifier, GameObject::Types type, Vector2 pos) {
    GameObject* queryObj = this->query(type, identifier);
    if (queryObj!= nullptr) {
        Vector2* updateVector = queryObj->getPosPtr();
        Vector2::UpdateValues(updateVector, &pos);
    }
}

void ObjectHandler::Update(GameObject::Types type, char id, void* args) {
    GameObject* obj = this->query(type, id); // Find the GameObject with the specified type and id
    if(obj != nullptr) { // If the object exists
        switch(type) {
            case GameObject::Types::DISK: {
                Disk* disk = dynamic_cast<Disk*>(obj);
                if (disk != nullptr) {
                  Disk::UpdateArgs* diskArgs = static_cast<Disk::UpdateArgs*>(args);
                  Vector2* updater = Vector2::cartesianToVector2(diskArgs->x, diskArgs->y);
                  Vector2::UpdateValues(disk->pos, updater);
                  delete updater;
                  delete diskArgs;
                }
                break;
            }
            case GameObject::Types::GOAL: {
                Goal* goal = dynamic_cast<Goal*>(obj);
                if (goal != nullptr) {
                  Goal::UpdateArgs* goalArgs = static_cast<Goal::UpdateArgs*>(args);
                  Vector2* updater = Vector2::cartesianToVector2(goalArgs->x, goalArgs->y);
                  Vector2::UpdateValues(goal->pos, updater);
                  goal->setDTheta(goalArgs->dTheta);
                  goal->setValid(goalArgs->valid);
                  delete updater;
                  delete goalArgs;
                }
                break;
            }
            case GameObject::Types::ROLLER: {
                FieldRoller* roller = dynamic_cast<FieldRoller*>(obj);
                if (roller != nullptr) {
                  FieldRoller::UpdateArgs* rollerArgs = static_cast<FieldRoller::UpdateArgs*>(args);
                  Vector2* updater = Vector2::cartesianToVector2(rollerArgs->x, rollerArgs->y);
                  Vector2::UpdateValues(roller->pos, updater);
                  roller->setCurrentColor((FieldRoller::Color)rollerArgs->color);
                  roller->setIsInContact(rollerArgs->is_in_contact);
                  delete updater;
                  delete rollerArgs;
                }
                break;
            }
            default:
                // Handle unsupported types or throw an exception
                break;
        }
    }
}