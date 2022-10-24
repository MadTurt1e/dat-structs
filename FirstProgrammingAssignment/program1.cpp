/*****************************
 * Richard Chung          Program1.cpp
 * Does calculations to determine the power levels of a large sum of foobars. 
 * 
 * ***********************************************/



#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Foobar;
class Foo;
class Bar;



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
    virtual int getStrength(){
      return position;
    }

    //and to get the name
    virtual string getName() const{
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

//function prototypes
vector<Foobar*> readInput(string inputName);
Foobar* createFoobar(string name, string type);
void printFoobarList(vector<Foobar*> input);
void outputFoobarList(vector<Foobar*> input, string outputName);

//Your program should loop through the input file only once, in a single pass adding information about each foobar into a vector of pointers to Foobar
vector<Foobar*> readInput(string inputName){
  ifstream input(inputName);
  string type = "";
  string name = "";

  Foobar* foobarObject; 

  vector<Foobar*> line;

  for (int i = 0; !input.eof(); i++){
    getline(input, type, ' ');
    getline(input, name, '\n');

    //exit if we reach the end of the file. 
    if(input.eof()){
      break;
    }

    //protuces a foobar object
    foobarObject = createFoobar(name, type);
    line.push_back(foobarObject);
  }

  //sets position to the correct one
  for (int i = line.size(), j = 0; i > 0; i--, j++){
    line[j]->setPosition(i);
  }

  input.close();
  return line;
}


//creates Foobar objects and returns as a pointer
Foobar* createFoobar(string name, string type){
  Foobar* output = nullptr;

  if (!type.compare("foobar")){
    output = new Foobar(name);
    return output; 
  }
  else if (!type.compare("foo")){
    output = new Foo(name);
    return output; 
  }
  else if (!type.compare("bar")){
    output = new Bar(name);
    return output;
  }
  return output;
}

void printFoobarList(vector<Foobar*> input){
  for (int i = 0; i < input.size(); i++){
    cout << input[i] -> getName() << " " << input[i] -> getStrength() << "\n";
  }
  return;
}

void outputFoobarList(vector<Foobar*> input, string outputName){
  ofstream output (outputName); 

  for (int i = 0; i < input.size(); i++){
    output << input[i] -> getName() << " " << input[i] -> getStrength() << "\n";
  }

  output.close();
  return;
}

//main
int main(){
  string s;
  cout << "Please input the name of your input file. \n";
  cin >> s;
  vector<Foobar*> list = readInput(s);

  cout << "Please input the name of your output file. \n";
  cin >> s;
  outputFoobarList(list, s);
  return 0;
}
