/**
 * @file TerriBull.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief TerriBull Single Robotics Library implementation designed to run on
 * the University of South Florida VEX BullBots seamlessly across all devices.
 *
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __TERRIBULL__
#define __TERRIBULL__

#include "../pros/apix.h"
#include "./lib/Logger.hpp"
#include <list>
#include <vector>
#include <map>
#include <string>
#include <cmath>

#define MIN(a, b) (((a) < (b))? (a) : (b))
#define MAX(a, b) (((a) > (b))? (a) : (b))

class ConfigurationParser;
/**
 * @brief TerriBull Robotics V5 VEX Library Built on-top of PROS
*/
namespace TerriBull {
    /**
     * @brief TerriBull Robotics Utilities and Class Predeclarations
     *
     */
    /* Standard Library Type Definitions */
    // typedef ::std::string string;
    // typedef ::std::map<string, size_t> Str2SizeMap;
    /* Vectorization Class */
    class Vector2;
    typedef std::vector<std::vector<float>> matrix;
    /* Manages Tasks for the Robot */
    class TaskManager;
    /* Task Data Containers */
    class Task;
    class DriveTask;
    class RollerTask;
    class IntakeTask;
    class ShooterTask;
    class TimeTask;
    class VariableTask;
    typedef ::std::vector<Task*> TaskList;
    /* Manages Serial Communication of the Robot */
    class SerialController;
    /* Manages Mechanical Components */
    class MechanicalSystem;
    /* Abstract Modifiers Class for Mechanism Control */
    class MechanicalComponent;
    /* Abstract Classes for Mechanical Components */
    class Drive;
    class Expansion;
    class Intake;
    class Roller;
    class Shooter; /*FlyWheel Catapult*/
    class Magazine;
    /* Main Controller for the Robot */
    class RoboController;
    /* Takes Controller Input and Controls the Robot */
    class InputController;
    /* Object Management and Data Information */
    class ObjectHandler;
    /* Game Object Class */
    class GameObject;
    /*Subsystem Tools*/    
    class VoltageRegulator;
    /* TerriBull Type Definitions */
    typedef ::std::vector<::pros::Motor> MotorGroup;

    /* Program Constants */
    extern int const MAX_VOLTAGE;

    /**
     * @brief Useful Algorithms and Functions
     */
    extern float const PI;
    extern int const RED_REVS;
    extern int const GREEN_REVS;
    extern int const BLUE_REVS;
    extern std::map<::std::string, int> GEAR_ENCODER;
    extern std::map<int, int> ENCODER_UNIT;

    extern float DEG2RAD( const float deg );
    extern float RAD2DEG( const float rad );
    extern float GetDTheta(float tf, float ti);
    extern matrix matrix_mult(matrix &M1, int M1_rows, int M1_cols, matrix &M2, int M2_rows, int M2_cols);
    extern pros::Imu mu;
    

    /**
     * @brief Template Classes
     *
     */

    /* Node Template Class */
    #ifndef Node_h
    #define Node_h

    // #include <iostream>
    // #include <string>

    using namespace std;


    template <class T>
    class Node {
        Node<T> *prev;
        Node<T> *next;
        int16_t priority;
        T *data;

    public:

      Node(){}

      ~Node() {}

      Node(T *data, int priority, Node<T> *p, Node<T> *n) {
        this->data = data;
        this->prev = p;
        this->next = n;
        this->priority = priority;
    }

      Node(T *data, int priority) {
        this->data = data;
        this->prev = nullptr;
        this->next = nullptr;
        this->priority = priority;
    }

      Node* getNext() { return  this->next; }

      Node* getPrev() { return  this->prev; }

      bool hasPrev() { return !(this->prev == nullptr); }

      bool hasNext(){ return  !(this->next == nullptr); }

      void setPrev(Node<T> *p) {
        this->prev = p;
    }
      void setNext(Node<T> *n) {
        this->next = n;
    }

      void setData(T* data) {
        this->data = data;
    }
      T *getData() { return this->data; }

      int16_t getPriority() { return this->priority; }

    //   friend ostream &operator<<(ostream &stream, const Node<T> &node) {
    //       stream << "\tNode " << &node << ":" << endl;
    //       stream << "\t" << *node.data <<"\t Priority: " << node.priority;
    //       return stream;
    //   }
    };
    #endif

    /* Linked List Template Class */
    #ifndef linkedlist_h
    #define linkedlist_h

    #include <iostream>
    #include <string>

    using namespace std;

    template <class T>
    class linkedlist {

    int size;
    Node<T> *g(int index, int cur, Node<T> *curr);
    public:

    Node<T> *head;
    Node<T> *tail;

    linkedlist<T>() {
        this->size = 0;
        this->head = nullptr;
        this->tail = nullptr;
    }

    ~linkedlist() {
        if (this->head != nullptr) {
            Node<T> *node = this->head;
            Node<T> *tempNode;
            while(node->hasNext()) {
            tempNode = node;
            node = node->getNext();
            delete tempNode;
        }
        delete node;
        }
    }

    linkedlist(Node<T> *h){
        this->size = 0;
        if (h != nullptr) {
            this->head = h;
            this->size++;
        }
    }

    linkedlist(Node<T> *h, Node<T> *t) {
        this->size = 0;
        if (h != nullptr) {
            this->head = h;
            this->size++;
        }
        if (t != nullptr) {
            this->tail = t;
            this->size++;
        }
    }

    bool isEmpty() {
        return this->size < 1;
    }

    void add(T* data, int priority) {
        bool insertion = false;
        Node<T> *node = new Node<T>(data, priority);
        /*
        The List is empty
        */
        if(this->head == nullptr) {
            this->head= node;
            this->tail= node;
            this->size++;
            insertion = true;
        }
        /*
        The List has only one node, check the prioritys and insert accordingly
        */
        else if(this->tail == this->head) {

            if (this->head->getPriority() < node->getPriority()) { // HEAD has less priority, swap them
                /*Create Linkage*/
                this->head->setPrev(node);
                node->setNext(this->head);
                this->tail = this->head;
                this->head = node;
                insertion = true;
                this->size++;
            }
            else {
                /*Create Linkage*/
                this->tail = node;
                this->head->setNext(this->tail);
                this->tail->setPrev(this->head);
                insertion = true;
                this->size++;
            }
        } else  {
            /*
                Our list has multiple nodes, back->front approach until we find our insertion spot
            */
            Node<T> *current = this->tail;
            while(priority>current->getPriority() && insertion==false) {
                //Our current node has not found the right insertion spot, and we haven't reached the end of the list
                if (current != this->head) {

                    current = current->getPrev();
                }
                //Our current node has not found the right insertion spot, and we have reached our head node.
                else {
                    /*Create Linkage*/
                    node->setNext(this->head);
                    this->head->setPrev(node);
                    this->head = node;
                    insertion = true;
                    this->size++;
                }
            }
            // WE found our spot!
            if(!insertion) {
                /*
                * Point our new nodes nodes to the current and the currents next node
                * Point the next nodes previous to our new node
                * Point our current nodes next node to our new node
                */
                if(current == this->tail) {// Insertion at end of list (tail)
                    /*Create Linkage*/
                    this->tail->setNext(node);
                    node->setPrev(this->tail);
                    this->tail = node;
                    this->size++;
                }
                // Insertion in middle of list
                else {
                    node->setPrev(current);
                    node->setNext(current->getNext());
                    current->getNext()->setPrev(node);
                    current->setNext(node);
                    insertion = true;
                    this->size++;
                }
            }
        }
    }

    bool remove(T* data){
            /* To Prevent a break, we want to return null if index isn't in our list.*/
        Node<T> *node = this->head;

        for (int i=0; i<this->length(); i++) {
            if (data == node->getData()) {
                /*We want to get references to these two nodes and link them*/
                Node<T> *prev = node->getPrev();
                Node<T> *next = node->getNext();
                /*But as always, edge cases:*/
                if (this->length() > 2)
                {
                /*Edge Case Handling of populated list, with deletions at the front or back*/
                    if(prev == nullptr) {
                        next->setPrev(nullptr);
                        this->head = next;
                        delete node;
                    }
                    else if(next == nullptr) {
                        prev->setNext(nullptr);
                        this->tail = prev;
                        delete node;
                    }

                    else {
                        /*Create Linkage*/
                        prev->setNext(next);
                        next->setPrev(prev);
                        delete node;
                    }

                }/* if (this->length > 2) */

                else /* We have either 1 or two nodes in our list.*/
                {
                    if (this->length() == 1)
                    {
                        this->head = nullptr;
                        this->tail = nullptr;
                        delete node;
                    }
                    else
                    {/* We are deleting the head node of a length-2 list */
                        if(node == this->head)
                        {
                            this->head = this->tail;
                            delete node;
                        }/* We are deleting the tail node of a length-2 list */
                        else {
                            this->tail = this->head;
                            delete node;
                        }
                    }
                }
                this->size--;
                return true;
            }
            node = node->getNext();
        }
        return false;
    }

    void clear() {
        if (this->head != nullptr) {
            Node<T> *node = this->head;
            Node<T> *tempNode;
            while(node->hasNext()) {
                tempNode = node;
                node = node->getNext();
                delete tempNode;
            }
            delete node;
        }
    }

    T* get(int index) {
        /* To Prevent a break, we want to return null if index isn't in our list.*/
        Node<T> *node = this->head;
        if (index >= this->length()) return nullptr;

        for (int i=0; i<this->length(); i++) {
            if(i == index) return node->getData();
            node = node->getNext();
        }
        return nullptr;
    }

    T* pop() {
        T* returnValue = this->head->getData();
        this->head->setData(nullptr);
        Node<T>* oldHead = this->head;
        this->head = this->head->getNext();
        this->size--;
        delete oldHead;
        return returnValue;
        /*This could easily be wrong*/
    }

    T* peek() {
        return this->head->getData();
    }

    int length(){ return this->size;}

    // friend ostream& operator<<(ostream &stream, linkedlist<T> const &ll) {
    //     stream << "LinkedList @ " << &ll << " of size " << ll.size << ", " << "data:" << endl;
    //     Node<T> *curr = ll.head;
    //     int16_t index = 0;
    //     while(!(curr == nullptr)) {
    //         stream << *curr << ": "<<"at Index "<<index<< endl;
    //         curr = curr->getNext();
    //         index++;
    //     }
    //     return stream;
    // }
    };

    #endif

    /* Priority Queue Template Class */
    #ifndef PriorityQueue_h
    #define PriorityQueue_h

    using namespace std;
    template <class T>
    class PriorityQueue {
    private:

    linkedlist<T> *pq;

    public:
    PriorityQueue() {
        this->pq = new linkedlist<T>();
    }
    bool isEmpty() {
        return this->pq->isEmpty();
    }

    T* deque_index(int index) {
        T* node = this->get(index);
        T* copy = new T(*node);
        bool removed = this->pq->remove(this->get(index));
        if (removed) {
            return copy;
        }
        return nullptr;
    }

    void enqueue(T* data, int priority) {
        this->pq->add(data, priority);
    }

    T* get(int index) {
        return this->pq->get(index);
    }
    T* deque() {
        return this->pq->pop();
    }

    T* peek() {
        return this->pq->peek();
    }
    void deque_all() {
        this->pq->clear();
    }
    };

    #endif

    #ifndef __TERRIBULL_GLOBALS_H
    #define __TERRIBULL_GLOBALS_H
    /* Global Variables */
    extern Logger logger; /* Global Logger */
    extern ::pros::Controller controller; /* Global Controller */
    #endif
};
    #ifndef __TERRIBULL_INCLUDES__
    #define __TERRIBULL_INCLUDES__

    #include "../api.h"

    #include "./lib/Vector2.hpp"
    #include "./lib/SubsystemTools/VoltageRegulator.hpp"
    #include "../MechanicalComponents/MechanicalComponent.hpp"
    #include "../MechanicalComponents/Drive/drive.hpp"
    #include "../MechanicalComponents/Intakes/intake.hpp"
    #include "../MechanicalComponents/Rollers/roller.hpp"
    #include "../MechanicalComponents/Shooters/Magazines/magazine.hpp"
    #include "../MechanicalComponents/Shooters/shooter.hpp"
    #include "../Controllers/MechanicalSystem/MechanicalSystem.hpp"
    #include "./lib/Tasking/Task.hpp"
    #include "./lib/Tasking/Types/DriveTask.hpp"
    #include "./lib/Tasking/Types/RollerTask.hpp"
    #include "./lib/Tasking/Types/IntakeTask.hpp"
    #include "./lib/Tasking/Types/ShooterTask.hpp"
    #include "./lib/Tasking/Types/TimeTask.hpp"
    #include "./lib/Tasking/Types/VariableTask.hpp"
    #include "../Controllers/TaskManager/TaskManager.hpp"
    #include "./lib/ConfigurationParser.hpp"
    #include "./lib/GameObjects/GameObject.hpp"
    #include "../Controllers/ObjectHandler/ObjectHandler.hpp"
    #include "../Controllers/SerialController/SerialController.hpp"
    #include "../Controllers/RoboController/RoboController.hpp"
    #include "../Controllers/InputController/InputController.hpp"
    #endif

#endif