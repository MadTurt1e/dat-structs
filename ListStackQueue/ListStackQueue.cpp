/*******************************************************************
 * Richard Chung                    ListStacKQueue.cpp
 * A program that manipulates stacks and queues, given input files. 
*******************************************************************/

#include <iostream>

using namespace std;


class SimpleList{
    private: 
    
    string name; 
    void* firstValue; 
    void* lastValue; 

    class Node{
        private: 
        void* currentValue;
        void* nextValue; 

    };

    public: 
    string getName(){
        return name; 
    }
    virtual void push(){

    }
    virtual void pop(){

    }

    protected: 
    void insertAtEnd(){

    }
    void insertAtStart(){

    }
};
class Stack : public SimpleList{
    public:
        void push(){

        }
}; 
class Queue : public SimpleList{
    public: 
        void pop(){
            
        }
};

int main(){
    return 0;
}