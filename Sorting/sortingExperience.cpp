// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, FALL 2022

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData:l) {
    output << pData->lastName << " " 
	   << pData->firstName << " " 
	   << pData->ssn << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/********************************************************
 * Richard Chung            sortingExperience.cpp
 * 
 * Sorts the nodes of a linked list. 
*********************************************************/

#include <queue>
//vector of 90 as in there are 90 ascii values from NUL to Z. 
vector<queue<Data*>> bucketOfBuckets(90);


bool myLessThanFull(Data* d1, Data* d2); 
int convertSSN(string ssn);
int listType(list<Data *> l);
void radixSort(list<Data *> &);
void radixSortSSN(list<Data*> &);
void insertionSort(list<Data*> &, int, int);

//compares two Data. True if d1 less than d2, false otherwise. 
bool myLessThanFull(Data* d1, Data* d2){
  int comparison = d1 -> lastName.compare(d2 -> lastName);
  if (comparison < 0){
    return true;
  } else if (comparison > 0){
    return false;
  }

  comparison = d1 -> firstName.compare(d2 -> firstName);
  if (comparison < 0){
    return true;
  } else if (comparison > 0){
    return false;
  }

  comparison = d1 -> ssn.compare(d2 -> ssn);
  if (comparison < 0){
    return true;
  }
  return false;
}

//string of format ddd-dd-dddd to massive int. 
int convertSSN(string ssn){
  return
    ssn[0] * 100000000 
  + ssn[1] * 10000000
  + ssn[2] * 1000000
  + ssn[4] * 100000
  + ssn[5] * 10000
  + ssn[7] * 1000
  + ssn[8] * 100
  + ssn[9] * 10
  + ssn[10]* 1;

}


//determine what list type it is. There are 3 types: full RNG (type 1), only social security numbers unsorted (type 2), and everybody with the same name (type 3)
int listType(list<Data *> l){
  auto i = l.begin();

  string lName = (*i)->lastName;
  string fName = (*i)->firstName;
  string ssn = (*i)->ssn;

  int dataType = 0;
  int lNameComp = 0;
  int fNameComp = 0;

  //number of iterations to check.  

  int numberOfRepititions = 10;

  for (int number = 0; number < numberOfRepititions || i == l.end(); i++, number++){

   

    
    lNameComp = lName.compare((*i)->lastName);

    //if it is less than 0 we know first is less than second, so maybe type 2? Well, we know for sure it's not type 3. 
    if (lNameComp < 0){
      lName = (*i)-> lastName;
      fName = (*i)-> firstName;
      dataType = 2;
    }
    //if the last name is the same we have to check other stuff 
    else if (lNameComp == 0){
      //running this only here is faster, i hope
      fNameComp = fName.compare((*i)->firstName);

      //We check if the first name is in order
      if(fNameComp){
        fName = (*i)-> firstName;
        dataType = 2;
      }

      //if last name is the same and the first name is the same it might be type 3. 
      else if(fNameComp == 0){
        dataType = 3;
      }
    }
    //if it is none of these, we know the data type is full RNG for sure
    else{
      return 1;
    }
  }

  //once we break out of this loop we have a pretty good idea of what type this is. 
  return dataType;
}

//raddix sort! calss the ssn sort to sort ssns, but does some other wacky stuff later
void radixSort(list<Data*> &l){
  //step 1: sort by SSNs first
  radixSortSSN(l);

  //step 2: sort first names

}

// uses raddix sort to sort ssns only
void radixSortSSN(list<Data *> &l)
{
  clock_t t1 = clock();
  // goes through all 11 SSN characters
  for (int charNum = 10; charNum >= 0; charNum--)
  {
    //skip character 3 and 6 because they are hyphens
    if (charNum == 3 || charNum == 6){
      continue;
    }
    // scan the entire list
    for (auto i = l.begin(); i != l.end(); i++)
    {
      bucketOfBuckets[((*i)->ssn)[charNum]].push(*i); // stick the data pointer into the bucket
    }

    list<Data*>::iterator it = l.begin();
    // sticks all the values from the relevant superbucket buckets back into the original vector
    for (int j = 48; j <= 57; j++)
    {
      // pop all the values until the thing is empty
      while (!bucketOfBuckets[j].empty())
      {
        (*it) = (bucketOfBuckets[j].front());
        bucketOfBuckets[j].pop();
        it++;
      }
      if (it == l.end()){
        break;
      }
    }
  }
}

//insertion sort! Runs for the small chunks found in type2 sorting where both first and last names are the same. 
void insertionSort(list<Data*> &l, int start, int end){

}

void type1Sort(list<Data*> &l){
  radixSort(l);
}
void type2Sort(list<Data*> &l){
  
}
void type3Sort(list<Data*> &l){
  radixSortSSN(l);
}

// Gets a linked list as input, and will rewrite the linked list to be sorted
void sortDataList(list<Data *> &l)
{
  // step 1: go through the first 5 or so data members to figure out which list classification it is.
  int type = listType(l);

  // each type has different necessities.
  switch (type)
  {
  case 1:
    type1Sort(l);
    break;
  case 2:
    type2Sort(l);
    break;
  case 3:
    type3Sort(l);
    break;
  default:
    // if we reach default, we just use bad sort.
    l.sort(myLessThanFull);
    break;
  }
}
