/*******************************************************************
 * Richard Chung                    ListStacKQueue.cpp
 * A program that manipulates stacks and queues, given input files.
 *******************************************************************/

#include <iostream>
#include <fstream>
#include <list>

using namespace std;

////if the list is empty we want to throw an exception so nothing breaks. We can detect this exception so it should be fine.
class EmptyListException : public exception
{
public:
    const char *what() const throw()
    {
        return "The list is empty!\n";
    }
};

// create an abstract base class called SimpleList that provides some aspects of singly linked list functionality
template <typename T>
class SimpleList
{
private:
    string name;

    class Node
    {
    private:
        // The base class should maintain pointers to the start and end of the singly linked list
        T *currentValue = new T;
        Node *nextValue = nullptr;

    public:
        // include a private nested class called Node, which contains one field for data and another to point to the next node
        Node(T current, Node *next)
        {
            *currentValue = current;
            nextValue = next;
        }
        Node(T *current, Node *next)
        {
            currentValue = current;
            nextValue = next;
        }

        // changes currentValue to something else
        void setCurrentValue(T *current)
        {
            currentValue = current;
        }
        // returns currentValue
        T getCurrentValue()
        {
            return *currentValue;
        }

        // changes the pointer to the next value
        void setNextValue(Node *next)
        {
            nextValue = next;
        }
        // returns address of the nextValue
        Node *getNextValue()
        {
            return nextValue;
        }
    };

    Node *headerNode;

    // tail node makes changing around the final value more straightforward
    Node *tailNode;
    int size;

public:
    // list constructor
    SimpleList(string nam)
    {
        name = nam;
        size = 0;

        tailNode = new Node(nullptr, nullptr);
        headerNode = new Node(nullptr, tailNode);
    }

    string getName()
    {
        return name;
    }

    // include two public pure virtual member functions for push and pop
    virtual void push(T value) = 0;
    virtual T pop() = 0;

protected:
    // provide the ability to insert a node at the start of the list
    void insertAtStart(T value)
    {
        // Creates a new node, and shoves it in between header node and first node.
        headerNode->setNextValue(new Node(value, headerNode->getNextValue()));

        size++;
    }

    // to insert a node at the end of the list
    void insertAtEnd(T value)
    {
        // new last node, we set the last node to cout << pSLi -> pop() << '\n';the value of the new one, and change the last node's memory address
        Node *newLastNode = new Node(nullptr, nullptr);
        *tailNode = *(new Node(value, newLastNode));
        tailNode = newLastNode;
        size++;
    }
    // to remove a node from the start of the listStack
    T removeAtStart()
    {
        T output;
        Node *pointerToStart;
        // return null pointer if there is nothing
        if (size != 0)
        {
            size--;
            // point header to the second node, return value of first node, than remove first node.
            pointerToStart = headerNode->getNextValue();
            output = pointerToStart->getCurrentValue();
            headerNode->setNextValue(pointerToStart->getNextValue());
            return output;
        }
        // empty
        throw EmptyListException();
    }
};

template <typename T>
class Stack : public SimpleList<T>
{
public:
    Stack(string name) : SimpleList<T>(name)
    {
    }
    void push(T value)
    {
        this->insertAtStart(value);
    }
    T pop()
    {
        return this->removeAtStart();
    }
};

template <typename T>
class Queue : public SimpleList<T>
{
public:
    Queue(string name) : SimpleList<T>(name)
    {
    }

    void push(T value)
    {
        this->insertAtEnd(value);
    }
    T pop()
    {
        return this->removeAtStart();
    }
};

// function prototypes
void readInput(string inputName, string outputName);
template <typename T>
SimpleList<T> *findList(list<SimpleList<T> *> searchThru, string name);
template <typename T>
void addToList(list<SimpleList<T> *> inputList, string name, string entry);

// IN: filename
// OUT: output file
void readInput(string inputName, string outputName)
{
    list<SimpleList<int> *> listSLi;    // all integer stacks and queues
    list<SimpleList<double> *> listSLd; // all double stacks and queues
    list<SimpleList<string> *> listSLs; // all string stacks and queues

    ifstream input(inputName);
    ofstream output(outputName);

    string action = "";
    string name = "";
    string entry = "";

    // for keeping track of the type
    char simpListType;

    for (int i = 0; !input.eof(); i++)
    {
        output << "PROCESSING COMMAND: ";
        getline(input, action, ' ');

        // pop: only takes two entrees: pop, than name of stack / queue
        if (action.compare("pop") == 0)
        {
            getline(input, name, '\n');

            output << action << " " << name << '\n';

            try
            {
                // do something different for each different data type
                simpListType = name[0];

                if (simpListType = 'i')
                {
                    output << findList(listSLi, name)->pop();
                }
                if (simpListType = 'd')
                {
                    output << findList(listSLd, name)->pop();
                }
                if (simpListType = 's')
                {
                    output << findList(listSLs, name)->pop();
                }
            }
            catch (EmptyListException)
            {
                output << "ERROR: This list is empty!" << '\n';
            }
            continue;
        }
        else
        {

            getline(input, name, ' ');
            simpListType = name[0];

            getline(input, entry, '\n');
        }

        // create: takes three entries: create, name, stack/queue
        if (action.compare("create") == 0)
        {
            // steps: we check to make sure the name isn't already there, then we add the thing in.
            if (simpListType = 'i')
            {
                if (findList(listSLi, name) != nullptr)
                {
                    addToList(listSLi, name, entry);
                }
                else
                {
                    output << "ERROR: This name already exists!" << '\n';
                    continue;
                }
            }
            if (simpListType = 'd')
            {
                if (findList(listSLd, name) != nullptr)
                {
                    addToList(listSLd, name, entry);
                }
                else
                {
                    output << "ERROR: This name already exists!" << '\n';
                    continue;
                }
            }
            if (simpListType = 's')
            {
                if (findList(listSLs, name) != nullptr)
                {
                    addToList(listSLs, name, entry);
                }
                else
                {
                    output << "ERROR: This name already exists!" << '\n';
                    continue;
                }
            }
        }
        // push: takes three entries: push, name, data value
        if (action.compare("push") == 0)
        {
            
        }
        //  exit if we reach the end of the file.
        if (input.eof())
        {
            break;
        }
    }

    input.close();
    output.close();
    return;
}

// returns a pointer to the correct simpleList with name
template <typename T>
SimpleList<T> *findList(list<SimpleList<T> *> searchThru, string name)
{
    for (auto pos = searchThru.begin(); pos != searchThru.end(); pos++)
    {
        if (name.compare((*pos)->getName()) == 0)
        {
            return *pos;
        }
    }
    return nullptr;
}

// creates a new simple list given a name and type of list, and shoves it into a given list.
template <typename T>
void addToList(list<SimpleList<T> *> inputList, string name, string entry)
{
    SimpleList<T> *newSimpleList;

    if (entry.compare("stack") == 0)
    {
        newSimpleList = new Stack<T>(name);
    }
    else
    {
        newSimpleList = new Queue<T>(name);
    }
    inputList.push_front(newSimpleList);

    return;
}

int main()
{
    return 0;
}