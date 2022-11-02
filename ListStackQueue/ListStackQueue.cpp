/*******************************************************************
 * Richard Chung                    ListStacKQueue.cpp
 * A program that manipulates stacks and queues, given input files. 
*******************************************************************/

#include <iostream>

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
    Node* tailNode; 
    int size; 

    public: 

        //list constructor
        SimpleList(string nam){
            name = nam; 
            size = 0;
            
            tailNode = new Node(null, nullptr);
            headerNode = new Node(null, &tailNode);
        }

        string getName(){
            return name; 
        }

        //include two public pure virtual member functions for push and pop
        virtual void push(){

        }
        virtual void* pop(){

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
        void insertAtEnd(){
            //Creates a new node, and shoves it in between tail node and last node. 
            Node temp = new Node(value, headerNode -> getNextValue());
            headerNode -> setNextValue(&temp);

            size++;
        }
        //to remove a node from the start of the list
        T removeAtStart(){
            if (size == 0){
                return null;
            }
            else{

            }
        }
};
class Stack : public SimpleList{
    public:
        void push(){

        }
        void* pop(){

        }
}; 
class Queue : public SimpleList{
    public: 
        void push(){
            
        }
        void* pop(){

        }
};

int main(){
    return 0;
}