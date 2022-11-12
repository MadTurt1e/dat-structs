/*******************************************************************
 * Richard Chung                    ListStacKQueue.cpp
 * A program that manipulates stacks and queues, given input files. 
*******************************************************************/

#include <iostream>
#include <list>

using namespace std;


//create an abstract base class called SimpleList that provides some aspects of singly linked list functionality
template <typename T>

class SimpleList{
    private: 
    
    string name; 

    class Node{
        private: 
            //The base class should maintain pointers to the start and end of the singly linked list
            T currentValue;
            void* nextValue; 
        public: 
            //include a private nested class called Node, which contains one field for data and another to point to the next node
            Node(T current, void* next){
                currentValue = current;
                nextValue = &next;
            }
            //changes currentValue to something else
            void setCurrentValue(T current){
                currentValue = current;
            }
            //returns currentValue
            T getCurrentValue(){
                return currentValue;
            }

            //changes the pointer to the next value
            void setNextValue(void* next){
                nextValue = next;
            }
            //returns address of the nextValue
            void* getNextValue(){
                return nextValue;
            }
    };

    Node* headerNode; 
    Node* lastNode; 
    int size; 

    public: 

        //list constructor
        SimpleList(string nam){
            name = nam; 
            size = 0;
            
            lastNode = new Node(nullptr, nullptr);
            headerNode = new Node(nullptr, &lastNode);
        }

        string getName(){
            return name; 
        }

        //include two public pure virtual member functions for push and pop
        virtual void push(){

        }
        virtual T pop(){

        }

    protected: 
        //provide the ability to insert a node at the start of the list
        void insertAtStart(T value){
            //Creates a new node, and shoves it in between header node and first node. 
            Node temp = new Node(value, headerNode -> getNextValue());
            headerNode -> setNextValue(&temp);

            size++;
        }
        //to insert a node at the end of the list
        void insertAtEnd(T value){
            //A new node is now the last node, and the old last node is not. 
            Node temp = lastNode;
            lastNode -> setCurrentValue(value);

            size++;
        }
        //to remove a node from the start of the list
        T removeAtStart(){
            T output;
            void* pointerToStart;
            //do nothing if there is nothing
            if (size == 0){
                return null;
            }
            else{
                size--;
                //point header to the second node, return value of first node, than remove first node. 
                pointerToStart = headerNode -> getNextvalue();
                output = headerNode->getCurrentValue();
                headerNode -> setNextValue(pointerToStart -> getNextValue());
                delete pointerToStart;
                return output;
            }
        }
};

template <typename T>
class Stack : public SimpleList{
    public:
        Stack(string name) : SimpleList<T>(name){

        }

        void push(T value){
            insertAtStart(value);
        }
        T pop(){
            return removeAtStart();
        }
}; 

template <typename T>
class Queue : public SimpleList{
    public: 
        Stack(string name) : SimpleList<T>(name){
            
        }

        void push(T value){
            insertAtEnd(value);
        }
        T pop(){
            return removeAtStart();
        }
};

int main(){
    SimpleList<int>* first = new SimpleList<int>("a");
    cout << first -> getName();
    return 0;
}