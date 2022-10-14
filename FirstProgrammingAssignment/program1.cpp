/*****************************
 * Richard Chung          Program1.cpp
 * Does calculations to determine the power levels of a large sum of foobars. 
 * 
 * ***********************************************/



#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

string readInput(string inputName);
void* createFoobar(string name, string type);

//Your program should loop through the input file only once, in a single pass adding information about each foobar into a vector of pointers to Foobar
string readInput(string inputName){
  ifstream input(inputName);
  string type = "";
  string name = "";
  for (int i = 0; !input.eof(); i++){
    getline(input, type, ' ');

    //exit if we reach the end of the file. 
    if(input.eof()){
      break;
    }

    void* foobarObject; 
    //protuces a foobar object
    foobarObject = createFoobar(name, type);
  }
}

//creates Foobar objects
void* createFoobar(string name, string type){
  if (!type.compare("foobar")){
    Foobar output = Foobar(name);
    return &output; 
  }
  else if (!type.compare("foo")){
    Foo output = Foo(name);
    return &output; 
  }
  else if (!type.compare("bar")){
    Bar output = Bar(name);
    return &output;
  }
}

//Ordinary foobars who are standing in a line have a strength of equal to their position in the line. 
class Foobar{
  //The name of each foobar and its current position in a line should be stored in the base class as private data members. 
  private:
    string name; 
    int position;
  public: 
  //The constructor should initially set the position to 0, meaning that the foobar is not currently in a line.
    Foobar(string nam){
      name = nam;
      position = 0; 
    }

    //public member functions to set the position in a line
    void setPosition(int pos){
      position = pos;
    }

    //to get the strength 
    int getStrength(){
      return position;
    }

    //and to get the name
    string getName() const{
      return name;
    }

    protected: 
      //a protected member function to get the position 
      int getPosition() const{
        return position;
      }
};

//A foo's strength is equal to their position in the line multiplied by 3. 
class Foo : public Foobar{
  public: 
    //The name should only be set once, in the base class's constructor.
    Foo(string name):Foobar(name){}
    //position*3
    int getStrength(){
      return getPosition()*3;
  }
};

//A bar's strength is equal to their position in the line plus 15
class Bar : public Foobar{
  public: 
    Bar(string name):Foobar(name){}
    //position +15
    int getStrength(){
      return getPosition()+15;
  }
};

//just kind of runs everything
int main(){


  readInput("sampleInput.txt");
  return 0;
}