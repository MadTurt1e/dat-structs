/*****************************
 * Richard Chung          Program1.cpp
 * Does calculations to determine the power levels of a large sum of foobars. 
 * 
 * ***********************************************/



#include <iostream>
#include <vector>

using namespace std;



int powerLevel(){
  return 0;
}

//Ordinary foobars who are standing in a line have a strength of equal to their position in the line. 
class Foobar{
  protected:
    string name; 
    int position;
  public: 
    Foobar(string nam, int pos){
      name = nam;
      position = pos;
    }

    //simply returns the name of the thing
    string getName() const{
      return name;
    }

    //returns the thing's position
    int getPosition() const{
      return position;
    }

    //powerlevel = position, returns position.
    int getPowerLevel(){
      return position;
    }
};

//A foo's strength is equal to their position in the line multiplied by 3. 
class Foo : public Foobar{
  public: 
    //position*3
    int getPowerLevel(){
      return position*3;
  }
};

//Some other foobars are bars. A bar's strength is equal to their position in the line plus 15.
class Bar : public Foobar{
  public: 
    //position +15
    int getPowerLevel(){
      return position+15;
  }
};

//just kind of runs everything
int main(){
  Foobar a = Foobar("John", 2);
  cout << a.getPowerLevel() << "\n" << "a";
  return 0;
}