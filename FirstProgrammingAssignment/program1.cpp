/*****************************
 * Richard Chung          Program1.cpp
 * Does calculations to determine the power levels of a large sum of foobars. 
 * 
 * ***********************************************/



#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//reads every line of a document and shoves the results into a foobar array
string readInput(string inputName){
  ifstream input(inputName);
  string inString = "";
  for (int i = 0; inString != "\n"; i++){
    getline(input, inString);
    cout << inString;
  }
}
//Ordinary foobars who are standing in a line have a strength of equal to their position in the line. 
class Foobar{
  //The name of each foobar and its current position in a line should be stored in the base class as private data members. 
  private:
    string name; 
    int position;
  public: 
  //The name should only be set once, in the base class's constructor.
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
    //position*3
    int getStrength(){
      return getPosition()*3;
  }
};

//A bar's strength is equal to their position in the line plus 15
class Bar : public Foobar{
  public: 
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