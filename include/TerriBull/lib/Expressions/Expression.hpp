/**
 * @file Expression.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Data Structure Representation of an Expression used specifically in Task Parsing.
 *     
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H
using namespace std;

#ifndef NodeExpressions_h
#define NodeExpressions_h

// Template Class for Node Storage Object

template <class T>
class Node {
  private:
    T *data;

  public:
    Node<T> *prev;
    Node<T> *next;
  Node() {
    this->prev = nullptr;
    this->next = nullptr;
  }

  ~Node() {
    delete data; //delete prev; delete next;
  }

  Node(T *data, Node<T> *p, Node<T> *n) {
    this->data = data;
    this->prev = p;
    this->next = n;
}

  Node(T *data) {
    this->prev = nullptr;
    this->next = nullptr;
    this->data = data;
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
  
};
#endif

typedef enum { ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION } FUNCTIONS;
/**
 * @brief Structure to hold information corresponding to the Operation
 *  in place.
 */

struct Operation {
    FUNCTIONS function;
    double* operand;/* Might Have to Be consistent */
    ~Operation() { delete this->operand; }
    Operation() {}
    Operation(FUNCTIONS func, double op) {
        this->function = func;
        this->operand = new double(op);
    }
    Operation(FUNCTIONS func, double* op) {
        this->function = func;
        this->operand = op;
    }
};
/**
 * @brief Implementation of Basic Arithmetic Function that are being performed sequentially
 * 
 */

class Expression  {  
    
    protected:
    /* Variables Which Will Be avialable to Subclasses etc.. */
        double* pValue;
        Node<Operation>* head,* tail;

                                                                                                         
    /* Protected Functions */
    void updateTotal();


    private:
        int l;
        void link(Node<Operation>* a, Node<Operation>* b);

    public:
    /* Public Contstructors & Destructors */
    Expression();
    ~Expression();
    Expression(Expression* const copy);
    Expression(const Expression& copy);
    
    /* Public Variables */

    /* Public Functions */
    double* Value();
    void newOperation(FUNCTIONS func, double *operand);
    void newOperation(FUNCTIONS func, double operand);
    void removeLastOperation();
    int length();
  
};


Expression::Expression() {
    this->pValue=0;
    this->l=0;
    this->head = nullptr;
    this->tail = nullptr;
}

/**
 * @brief Destroy the Calc List:: Calc List object
 * 
 */

Expression::~Expression() {
    Node<Operation>* listPtr = this->tail;
    Node<Operation>* above;
    while (nullptr != listPtr) {
        above = listPtr;
        listPtr = listPtr->prev;
        if (nullptr!= above) { 
            delete above->getData();
            delete above;
        }
    }
}


/**
 * @brief Construct a new Calc List:: Calc List object
 *  Copy constructor for the Calc List
 * @param copy ptr to the list to be copied
 */
Expression::Expression(Expression* const copy) : Expression::Expression() {
    this->l = copy->l;
    /* Copy In all Data from the Nodes */
    Node<Operation>* our_prev = nullptr;
    Node<Operation>* our_curr = this->head;
    Node<Operation>* copy_curr = copy->head;
    while (nullptr != copy_curr) {
        our_curr = new Node<Operation>;
        our_curr->setData(copy_curr->getData());
        link(our_prev, our_curr);
        our_prev = our_curr;
        copy_curr = copy_curr->next;
    /* Back Propagation */
    this->tail = our_curr;
    while (nullptr != our_curr->prev) our_curr= our_curr->prev;
    this->head = our_curr;
    updateTotal();
    }
}

/**
 * @brief Construct a new Calc List:: Calc List object
 * Copy constructor for the Calc List
 * @param copy reference to the list to be copied
 */
Expression::Expression(const Expression& copy): Expression::Expression() {
    this->l = copy.l;
    /* Copy In all Data from the Nodes */
    Node<Operation>* our_prev = nullptr;
    Node<Operation>* our_curr;
    Node<Operation>* copy_curr = copy.head;
    while (nullptr != copy_curr) {
        our_curr = new Node<Operation>;
        our_curr->setData(copy_curr->getData());
        link(our_prev, our_curr);
        our_prev = our_curr;
        copy_curr = copy_curr->next;
    }
    /* Back Propagation */
    this->tail = our_curr;
    while (nullptr != our_curr->prev) our_curr= our_curr->prev;
    this->head = our_curr;
    updateTotal();
}

/* Public Functions */
/**
 * @brief Gives back calculated total
 * 
 * @return double total
 */
double* Expression::Value() {
    return this->pValue;
}

/**
 * @brief Adds a new Operation into our List
 * 
 * @param func The function being applied
 * @param operand the Value of which to perform the function
 */
void Expression::newOperation(FUNCTIONS func, double* operand) {
    if(func == DIVISION && operand == 0) {
        return;
    }
    /* This logic adds in a new data point into the right position in the list and updates our total. */
    struct Operation* new_data = new Operation(func, operand);//;new_data->function=func;new_data->operand=operand;
    if (nullptr == this->head) { /* n < 1 */
        this->head = new Node<Operation>;
        this->tail = this->head;
        this->head->setData(new_data);
    }
    else if(this->head == this->tail) { /* n < 2*/
        this->tail = new Node<Operation>;
        this->tail->setData(new_data);
        link(this->head, this->tail);
    }
    else { /* n > 2 */
        Node<Operation>* old_tail = this->tail;
        this->tail = new Node<Operation>;
        this->tail->setData(new_data);
        link(old_tail, this->tail);
    }
    /* Now Reflect the Changes */
    this->l++;
}


void Expression::newOperation(FUNCTIONS func, double operand) {
    if(func == DIVISION && operand == 0) {
        return;
    }
    /* This logic adds in a new data point into the right position in the list and updates our total. */
    struct Operation* new_data = new Operation(func, operand);//;new_data->function=func;new_data->operand=operand;
    if (nullptr == this->head) { /* n < 1 */
        this->head = new Node<Operation>;
        this->tail = this->head;
        this->head->setData(new_data);
    }
    else if(this->head == this->tail) { /* n < 2*/
        this->tail = new Node<Operation>;
        this->tail->setData(new_data);
        link(this->head, this->tail);
    }
    else { /* n > 2 */
        Node<Operation>* old_tail = this->tail;
        this->tail = new Node<Operation>;
        this->tail->setData(new_data);
        link(old_tail, this->tail);
    }
    /* Now Reflect the Changes */
    this->l++;
    this->updateTotal();
}
/**
 * @brief 
 */
void Expression::removeLastOperation() {
    /* Change the Tail, and free the old Tails memory. Reflect this change in the total. */
    if(this->l == 0) {
        return;
    }
    Node<Operation>* old_tail = this->tail;
    if ( nullptr != old_tail && old_tail->hasPrev() ) {
        this->tail = old_tail->prev;
        link(this->tail, nullptr);
        this->l--;
    }
    else { /* Single Node List */
        this->tail = nullptr; this->head = nullptr;
        this->l = 0;
        this->pValue = 0;
    }
    delete old_tail;
}

/**
 * @brief return the length of the linked list currently being pointed to
 * 
 * @return size_t 
 */

int Expression::length() {
    return this->l;
}

/**
 * @brief to connect two nodes 
 * 
 * @param a 
 * @param b 
 */

void Expression::link(Node<Operation>* a, Node<Operation>* b) {
    /*This process puts node a's next to b, and b's prev to a*/
    if(nullptr != a)/*Cannot dereference null...*/
        a->next = b;
    if(nullptr != b)
        b->prev = a;
}

/**
 * @brief Arithmetic Logic Implementation
 * 
 */


void Expression::updateTotal() {
    /* Start from Begginging of the List */
    Node<Operation>* curr = this->head;
    this->pValue = 0;
    while (nullptr != curr) { 
        /* Determine which function to apply the operand by */
        Operation* curr_op = curr->getData();
        curr=curr->next;
        switch (curr_op->function) {
            
            case ADDITION:
                *(this->pValue)+=*(curr_op->operand);
                break;
           
            case SUBTRACTION:
                *(this->pValue)-=*(curr_op->operand);
                break;

            case MULTIPLICATION:
                *(this->pValue)*=*(curr_op->operand);
                break;

            case DIVISION:
                *(this->pValue)/=*(curr_op->operand);
                break;

            default:
                break; /* Maybe exception shtuff*/
        }

    }
}

#endif
