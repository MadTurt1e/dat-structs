/*******************************************************************
 * Richard Chung                    ListStacKQueue.cpp
 * A program that manipulates stacks and queues, given input files. 
*******************************************************************/

#include <iostream>
#include <list>

using namespace std;




//if the list is empty we want to throw an exception so nothing breaks. We can detect this exception so it should be fine. 
class EmptyListException:public exception{
	public:
		const char* what()const throw(){
			return "The list is empty!\n";
		}
};

//create an abstract base class called SimpleList that provides some aspects of singly linked list functionality
template <typename T>
class SimpleList{
    private: 
    
    string name; 

    class Node{
        private: 
            //The base class should maintain pointers to the start and end of the singly linked list
            T* currentValue = new T;
            Node* nextValue = nullptr; 
        public: 
            //include a private nested class called Node, which contains one field for data and another to point to the next node
            Node (T current, Node* next){
                *currentValue = current;
                nextValue = next;
            }
            Node (T* current, Node* next){
                currentValue = current;
                nextValue = next;
            }

            //changes currentValue to something else
            void setCurrentValue(T* current){
                currentValue = current;
            }
            //returns currentValue
            T getCurrentValue(){
                return *currentValue;
            }

            //changes the pointer to the next value
            void setNextValue(Node* next){
                nextValue = next;
            }
            //returns address of the nextValue
            Node* getNextValue(){
                return nextValue;
            }
    };

    Node* headerNode;

    //tail node makes changing around the final value more straightforward
    Node* tailNode; 
    int size; 

    public: 

        //list constructor
        SimpleList(string nam){
            name = nam; 
            size = 0;
            
            tailNode = new Node(nullptr, nullptr);
            headerNode = new Node(nullptr, tailNode);
        }

        string getName(){
            return name; 
        }

        //include two public pure virtual member functions for push and pop
        virtual void push(T value) = 0;
        virtual T pop() = 0;

    protected: 
        //provide the ability to insert a node at the start of the list
        void insertAtStart(T value){
            //Creates a new node, and shoves it in between header node and first node. 
            headerNode -> setNextValue(new Node(value, headerNode -> getNextValue()));

            size++;
        }

        //to insert a node at the end of the list
        void insertAtEnd(T value){
            //new last node, we set the last node to cout << pSLi -> pop() << '\n';the value of the new one, and change the last node's memory address
            Node* newLastNode = new Node(nullptr, nullptr);
            *tailNode = *(new Node(value, newLastNode));
            tailNode = newLastNode; 
            size++;
        }
        //to remove a node from the start of the listStack
        T removeAtStart(){
            T output;
            Node* pointerToStart;
            //return null pointer if there is nothing
            if (size != 0){
                size--;
                //point header to the second node, return value of first node, than remove first node. 
                pointerToStart = headerNode -> getNextValue();
                output = pointerToStart -> getCurrentValue();
                headerNode -> setNextValue(pointerToStart -> getNextValue());
                return output;
            }
            //empty
            throw EmptyListException();
        }
};

template <typename T>
class Stack : public SimpleList<T>{
    public:
        Stack(string name) : SimpleList<T>(name){

        }
        void push(T value){
            this -> insertAtStart(value);
        }
        T pop(){
            return this -> removeAtStart();
        }
}; 

template <typename T>
class Queue : public SimpleList<T>{
    public: 
        Queue(string name) : SimpleList<T>(name){
            
        }

        void push(T value){
            this -> insertAtEnd(value);
        }
        T pop(){
            return this -> removeAtStart();
        }
};

template <typename T>
SimpleList<T>* findList(list<SimpleList<T>*> searchThru, string name){

}

int main(){
    list<SimpleList<int> *> listSLi; // all integer stacks and queues
    list<SimpleList<double> *> listSLd; // all double stacks and queues
    list<SimpleList<string> *> listSLs; // all string stacks and queues

    pSLi = new Queue<string>("sample1");
    pSLi -> push("a");
    pSLi -> push("b");
    cout << pSLi -> pop() << '\n';
    pSLi -> push("c");
    cout << pSLi -> pop() << '\n';
    cout << pSLi -> pop() << '\n';
    cout << pSLi -> pop() << '\n';
    return 0;
}