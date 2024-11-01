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

// bucket that can be reused for a bunch of stuff
vector<vector<Data *>> bucketOfBuckets(500, vector<Data *>(500000));

// global variables for keeping track of the position in each individual bucket.
vector<int> startLoc(10000);
vector<int> endLoc(10000);

// literally every single first/last name for radix sorting. its preordered so its easier. RI
vector<string> firstNamesSorted = {"AALIYAH", "AARON", "ABEL", "ABIGAIL", "ABRAHAM", "ADALINE", "ADALYN", "ADALYNN", "ADAM", "ADDISON", "ADELINE", "ADELYN", "ADRIAN", "ADRIANA", "AIDAN", "AIDEN", "ALAINA", "ALAN", "ALANA", "ALAYNA", "ALEJANDRO", "ALEX", "ALEXA", "ALEXANDER", "ALEXANDRA", "ALEXIS", "ALICE", "ALINA", "ALIVIA", "ALIYAH", "ALLISON", "ALYSSA", "AMARA", "AMAYA", "AMELIA", "AMIR", "AMY", "ANA", "ANASTASIA", "ANDREA", "ANDRES", "ANDREW", "ANGEL", "ANGELA", "ANGELINA", "ANNA", "ANNABELLE", "ANTHONY", "ANTONIO", "ARABELLA", "ARIA", "ARIANA", "ARIANNA", "ARIEL", "ARTHUR", "ARYA", "ASHER", "ASHLEY", "ASHTON", "ATHENA", "AUBREE", "AUBREY", "AUDREY", "AUGUST", "AURORA", "AUSTIN", "AUTUMN", "AVA", "AVERY", "AVERY", "AXEL", "AYDEN", "AYLA", "BAILEY", "BARRETT", "BEAU", "BECKETT", "BELLA", "BENJAMIN", "BENNETT", "BENTLEY", "BLAKE", "BRADLEY", "BRADY", "BRANDON", "BRANTLEY", "BRAXTON", "BRAYDEN", "BRIAN", "BRIANNA", "BRIELLE", "BRODY", "BROOKE", "BROOKLYN", "BROOKLYNN", "BROOKS", "BRYAN", "BRYCE", "BRYNLEE", "BRYSON", "CADEN", "CALEB", "CALLIE", "CALVIN", "CAMDEN", "CAMERON", "CAMILA", "CARLOS", "CAROLINE", "CARSON", "CARTER", "CATHERINE", "CAYDEN", "CECILIA", "CHARLES", "CHARLIE", "CHARLIE", "CHARLOTTE", "CHASE", "CHLOE", "CHRISTIAN", "CHRISTOPHER", "CLAIRE", "CLARA", "CLAYTON", "COLE", "COLIN", "COLTON", "CONNOR", "COOPER", "CORA", "DAISY", "DAKOTA", "DALEYZA", "DAMIAN", "DANIEL", "DANIELA", "DAVID", "DAWSON", "DEAN", "DECLAN", "DELANEY", "DELILAH", "DEREK", "DESTINY", "DIANA", "DIEGO", "DOMINIC", "DYLAN", "EASTON", "EDEN", "EDWARD", "ELEANOR", "ELENA", "ELI", "ELIANA", "ELIAS", "ELIJAH", "ELISE", "ELIZA", "ELIZABETH", "ELLA", "ELLIANA", "ELLIE", "ELLIOT", "ELLIOTT", "ELOISE", "EMERSON", "EMERSYN", "EMERY", "EMILIA", "EMILIANO", "EMILY", "EMMA", "EMMANUEL", "EMMETT", "ERIC", "ESTHER", "ETHAN", "EVA", "EVAN", "EVELYN", "EVERETT", "EVERLY", "EZEKIEL", "EZRA", "FAITH", "FELIX", "FINLEY", "FINN", "FIONA", "GABRIEL", "GABRIELLA", "GAEL", "GAVIN", "GENESIS", "GENEVIEVE", "GEORGE", "GEORGIA", "GIANNA", "GIOVANNI", "GRACE", "GRACIE", "GRAHAM", "GRANT", "GRAYSON", "GREYSON", "GRIFFIN", "HADLEY", "HAILEY", "HANNAH", "HARLEY", "HARMONY", "HARPER", "HARRISON", "HAYDEN", "HAYDEN", "HAZEL", "HENRY", "HOLDEN", "HUDSON", "HUNTER", "IAN", "IRIS", "ISAAC", "ISABEL", "ISABELLA", "ISABELLE", "ISAIAH", "ISLA", "ISRAEL", "IVAN", "IVY", "JACE", "JACK", "JACKSON", "JACOB", "JADE", "JADEN", "JAKE", "JAMES", "JAMESON", "JASMINE", "JASON", "JASPER", "JAVIER", "JAX", "JAXON", "JAXSON", "JAYCE", "JAYDEN", "JAYLA", "JEREMIAH", "JEREMY", "JESSE", "JESSICA", "JESUS", "JOANNA", "JOCELYN", "JOEL", "JOHN", "JONAH", "JONATHAN", "JORDAN", "JORDYN", "JORGE", "JOSE", "JOSEPH", "JOSEPHINE", "JOSHUA", "JOSIAH", "JOSIE", "JOSUE", "JUAN", "JUDAH", "JUDE", "JULIA", "JULIAN", "JULIANA", "JULIANNA", "JULIET", "JULIETTE", "JUNE", "JUSTIN", "KADEN", "KAI", "KAIDEN", "KALEB", "KARTER", "KATHERINE", "KAYDEN", "KAYLA", "KAYLEE", "KENDALL", "KENNEDY", "KENNETH", "KEVIN", "KHLOE", "KILLIAN", "KIMBERLY", "KING", "KINGSTON", "KINSLEY", "KNOX", "KYLE", "KYLIE", "KYRIE", "LAILA", "LANDON", "LAUREN", "LAYLA", "LEAH", "LEILA", "LEILANI", "LEO", "LEON", "LEONARDO", "LEVI", "LIAM", "LILA", "LILIANA", "LILLIAN", "LILLY", "LILY", "LINCOLN", "LOGAN", "LOLA", "LONDON", "LONDYN", "LORENZO", "LUCA", "LUCAS", "LUCIA", "LUCY", "LUIS", "LUKAS", "LUKE", "LUNA", "LYDIA", "LYLA", "MACKENZIE", "MADDOX", "MADELINE", "MADELYN", "MADISON", "MAGGIE", "MAKAYLA", "MALACHI", "MALIA", "MARCUS", "MARGARET", "MARIA", "MARIAH", "MARK", "MARLEY", "MARY", "MASON", "MATEO", "MATIAS", "MATTEO", "MATTHEW", "MAVERICK", "MAX", "MAXIMUS", "MAXWELL", "MAYA", "MCKENZIE", "MELANIE", "MELODY", "MESSIAH", "MIA", "MICAH", "MICHAEL", "MICHELLE", "MIGUEL", "MILA", "MILES", "MILO", "MOLLY", "MORGAN", "MYA", "MYLES", "NAOMI", "NATALIA", "NATALIE", "NATHAN", "NATHANIEL", "NEVAEH", "NICHOLAS", "NICOLAS", "NICOLE", "NOAH", "NOELLE", "NOLAN", "NORA", "NORAH", "NOVA", "OLIVER", "OLIVIA", "OMAR", "OSCAR", "OWEN", "PAIGE", "PAISLEY", "PARKER", "PARKER", "PATRICK", "PAUL", "PAXTON", "PAYTON", "PENELOPE", "PETER", "PEYTON", "PIPER", "PRESLEY", "PRESTON", "QUINN", "RACHEL", "RAELYNN", "REAGAN", "REBECCA", "REESE", "REMI", "REMINGTON", "RHETT", "RICHARD", "RILEY", "RIVER", "ROBERT", "ROMAN", "ROSALIE", "ROSE", "ROWAN", "ROWAN", "RUBY", "RYAN", "RYDER", "RYKER", "RYLEE", "RYLEIGH", "SADIE", "SAMANTHA", "SAMUEL", "SANTIAGO", "SARA", "SARAH", "SAVANNAH", "SAWYER", "SAWYER", "SCARLETT", "SEBASTIAN", "SELENA", "SERENITY", "SIENNA", "SILAS", "SKYLAR", "SLOANE", "SOFIA", "SOPHIA", "SOPHIE", "STELLA", "STEVEN", "SUMMER", "SYDNEY", "TAYLOR", "TEAGAN", "TESSA", "THEODORE", "THIAGO", "THOMAS", "TIMOTHY", "TRINITY", "TRISTAN", "TUCKER", "TYLER", "VALENTINA", "VALERIA", "VALERIE", "VANESSA", "VICTOR", "VICTORIA", "VINCENT", "VIOLET", "VIVIAN", "WAYLON", "WESLEY", "WESTON", "WILLIAM", "WILLOW", "WYATT", "XANDER", "XAVIER", "XIMENA", "ZACHARY", "ZANDER", "ZANE", "ZAYDEN", "ZION", "ZOE", "ZOEY"};
vector<string> lastNamesSorted = {"ACOSTA", "ADAMS", "ADKINS", "AGUILAR", "AGUIRRE", "ALEXANDER", "ALLEN", "ALVARADO", "ALVAREZ", "ANDERSON", "ANDREWS", "ARMSTRONG", "ARNOLD", "AUSTIN", "AVILA", "AYALA", "BAILEY", "BAKER", "BALDWIN", "BANKS", "BARBER", "BARKER", "BARNES", "BARNETT", "BARRETT", "BARTON", "BATES", "BECK", "BECKER", "BELL", "BENNETT", "BENSON", "BERRY", "BISHOP", "BLACK", "BLAIR", "BLAKE", "BOWEN", "BOWMAN", "BOYD", "BRADLEY", "BRADY", "BREWER", "BROOKS", "BROWN", "BRYANT", "BURGESS", "BURKE", "BURNS", "BURTON", "BUSH", "BUTLER", "BYRD", "CABRERA", "CALDERON", "CALDWELL", "CAMACHO", "CAMPBELL", "CAMPOS", "CANNON", "CARDENAS", "CARLSON", "CARPENTER", "CARR", "CARRILLO", "CARROLL", "CARTER", "CASTANEDA", "CASTILLO", "CASTRO", "CERVANTES", "CHAMBERS", "CHAN", "CHANDLER", "CHANG", "CHAPMAN", "CHAVEZ", "CHEN", "CHRISTENSEN", "CLARK", "CLARKE", "COHEN", "COLE", "COLEMAN", "COLLINS", "COLON", "CONTRERAS", "COOK", "COOPER", "CORTEZ", "COX", "CRAIG", "CRAWFORD", "CROSS", "CRUZ", "CUMMINGS", "CUNNINGHAM", "CURRY", "CURTIS", "DANIEL", "DANIELS", "DAVIDSON", "DAVIS", "DAWSON", "DAY", "DEAN", "DELACRUZ", "DELEON", "DELGADO", "DENNIS", "DIAZ", "DIXON", "DOMINGUEZ", "DOUGLAS", "DOYLE", "DUNCAN", "DUNN", "DURAN", "EDWARDS", "ELLIOTT", "ELLIS", "ERICKSON", "ESPINOZA", "ESTRADA", "EVANS", "FARMER", "FERGUSON", "FERNANDEZ", "FIELDS", "FIGUEROA", "FISCHER", "FISHER", "FITZGERALD", "FLEMING", "FLETCHER", "FLORES", "FORD", "FOSTER", "FOWLER", "FOX", "FRANCIS", "FRANCO", "FRANK", "FRANKLIN", "FRAZIER", "FREEMAN", "FUENTES", "FULLER", "GALLAGHER", "GALLEGOS", "GARCIA", "GARDNER", "GARNER", "GARRETT", "GARZA", "GEORGE", "GIBSON", "GILBERT", "GILL", "GOMEZ", "GONZALES", "GONZALEZ", "GOODMAN", "GOODWIN", "GORDON", "GRAHAM", "GRANT", "GRAVES", "GRAY", "GREEN", "GREENE", "GREGORY", "GRIFFIN", "GRIFFITH", "GROSS", "GUERRA", "GUERRERO", "GUTIERREZ", "GUZMAN", "HAIL", "HALE", "HALL", "HAMILTON", "HAMMOND", "HAMPTON", "HANSEN", "HANSON", "HARDY", "HARMON", "HARPER", "HARRINGTON", "HARRIS", "HARRISON", "HART", "HARVEY", "HAWKINS", "HAYES", "HAYNES", "HENDERSON", "HENRY", "HERNANDEZ", "HERRERA", "HICKS", "HIGGINS", "HILL", "HINES", "HODGES", "HOFFMAN", "HOLLAND", "HOLMES", "HOLT", "HOPKINS", "HORTON", "HOWARD", "HOWELL", "HUANG", "HUBBARD", "HUDSON", "HUGHES", "HUNT", "HUNTER", "INGRAM", "JACKSON", "JACOBS", "JAMES", "JENKINS", "JENNINGS", "JENSEN", "JIMENEZ", "JOHNSON", "JOHNSTON", "JONES", "JORDAN", "JOSEPH", "JUAREZ", "KELLER", "KELLEY", "KELLY", "KENNEDY", "KHAN", "KIM", "KING", "KLEIN", "KNIGHT", "LAMBERT", "LANE", "LARA", "LARSON", "LAWRENCE", "LAWSON", "LE", "LEE", "LEON", "LEONARD", "LEWIS", "LI", "LIN", "LITTLE", "LIU", "LOGAN", "LONG", "LOPEZ", "LOVE", "LOWE", "LUCAS", "LUNA", "LYNCH", "LYONS", "MACK", "MALDONADO", "MALONE", "MANN", "MANNING", "MARQUEZ", "MARSHALL", "MARTIN", "MARTINEZ", "MASON", "MATTHEWS", "MAXWELL", "MAY", "MCCARTHY", "MCCOY", "MCDANIEL", "MCDONALD", "MCGEE", "MCKINNEY", "MCLAUGHLIN", "MEDINA", "MEJIA", "MENDEZ", "MENDOZA", "MEYER", "MILES", "MILLER", "MILLS", "MIRANDA", "MITCHELL", "MOLINA", "MONTGOMERY", "MONTOYA", "MOORE", "MORALES", "MORAN", "MORENO", "MORGAN", "MORRIS", "MORRISON", "MOSS", "MULLINS", "MUNOZ", "MURPHY", "MURRAY", "MYERS", "NAVARRO", "NEAL", "NELSON", "NEWMAN", "NEWTON", "NGUYEN", "NICHOLS", "NORMAN", "NORRIS", "NUNEZ", "OBRIEN", "OCHOA", "OCONNOR", "OLIVER", "OLSON", "ORTEGA", "ORTIZ", "OWENS", "PACHECO", "PADILLA", "PAGE", "PALMER", "PARK", "PARKER", "PARKS", "PARSONS", "PATEL", "PATTERSON", "PAUL", "PAYNE", "PEARSON", "PENA", "PEREZ", "PERKINS", "PERRY", "PERSON", "PETERS", "PETERSON", "PHAM", "PHILLIPS", "PIERCE", "PORTER", "POTTER", "POWELL", "POWERS", "PRICE", "QUINN", "RAMIREZ", "RAMOS", "RAMSEY", "RAY", "REED", "REESE", "REEVES", "REID", "REYES", "REYNOLDS", "RHODES", "RICE", "RICHARDS", "RICHARDSON", "RILEY", "RIOS", "RIVAS", "RIVERA", "ROBBINS", "ROBERTS", "ROBERTSON", "ROBINSON", "ROBLES", "RODGERS", "RODRIGUEZ", "ROGERS", "ROJAS", "ROMAN", "ROMERO", "ROSALES", "ROSE", "ROSS", "ROWE", "RUIZ", "RUSSELL", "RYAN", "SALAZAR", "SALINAS", "SANCHEZ", "SANDERS", "SANDOVAL", "SANTIAGO", "SANTOS", "SAUNDERS", "SCHMIDT", "SCHNEIDER", "SCHROEDER", "SCHULTZ", "SCHWARTZ", "SCOTT", "SERRANO", "SHARP", "SHAW", "SHELTON", "SHERMAN", "SILVA", "SIMMONS", "SIMON", "SIMPSON", "SIMS", "SINGH", "SMITH", "SNYDER", "SOLIS", "SOTO", "SPENCER", "STANLEY", "STEELE", "STEPHENS", "STEVENS", "STEVENSON", "STEWART", "STONE", "STRICKLAND", "SULLIVAN", "SUTTON", "SWANSON", "TATE", "TAYLOR", "TERRY", "THOMAS", "THOMPSON", "THORNTON", "TODD", "TORRES", "TOWNSEND", "TRAN", "TRUJILLO", "TUCKER", "TURNER", "VALDEZ", "VALENCIA", "VARGAS", "VASQUEZ", "VAUGHN", "VAZQUEZ", "VEGA", "VELASQUEZ", "WADE", "WAGNER", "WALKER", "WALLACE", "WALSH", "WALTERS", "WALTON", "WANG", "WARD", "WARNER", "WARREN", "WASHINGTON", "WATERS", "WATKINS", "WATSON", "WATTS", "WEAVER", "WEBB", "WEBER", "WEBSTER", "WELCH", "WELLS", "WEST", "WHEELER", "WHITE", "WILLIAMS", "WILLIAMSON", "WILLIS", "WILSON", "WISE", "WOLF", "WOLFE", "WONG", "WOOD", "WOODS", "WRIGHT", "WU", "YANG", "YOUNG", "ZHANG", "ZIMMERMAN"};

bool myLessThanFull(Data *d1, Data *d2);
int convertSSN(string ssn);
int listType(list<Data *> *l);
int stringSearch(int nameType, string name);

void radixSort(list<Data *> *);
void radixSortSSN(list<Data *> *);

void insertionSortDivideAndConquer(list<Data *> *);
void insertionSort(int, list<Data *>:: iterator*);

void type1Sort(list<Data *> *l);
void type2Sort(list<Data *> *l);
void type3Sort(list<Data *> *l);

// compares two Data. True if d1 less than d2, false otherwise.
bool myLessThanFull(Data *d1, Data *d2)
{
  int comparison = d1->lastName.compare(d2->lastName);
  if (comparison < 0)
  {
    return true;
  }
  else if (comparison > 0)
  {
    return false;
  }

  comparison = d1->firstName.compare(d2->firstName);
  if (comparison < 0)
  {
    return true;
  }
  else if (comparison > 0)
  {
    return false;
  }

  comparison = d1->ssn.compare(d2->ssn);
  if (comparison < 0)
  {
    return true;
  }
  return false;
}

// string of format ddd-dd-dddd to massive int.
int convertSSN(string ssn)
{
  return ssn[0] * 100000000 + ssn[1] * 10000000 + ssn[2] * 1000000 + ssn[4] * 100000 + ssn[5] * 10000 + ssn[7] * 1000 + ssn[8] * 100 + ssn[9] * 10 + ssn[10] * 1;
}

// determine what list type it is. There are 3 types: full RNG (type 1), only social security numbers unsorted (type 2), and everybody with the same name (type 3)
int listType(list<Data *> *l)
{
  auto i = l->begin();

  string lName = (*i)->lastName;
  string fName = (*i)->firstName;
  string ssn = (*i)->ssn;

  int dataType = 0;
  int lNameComp = 0;
  int fNameComp = 0;

  // number of iterations to check. Less is better, but usually at the cost of possibly bad RNG

  int numberOfRepititions = 3;

  for (int number = 0; number < numberOfRepititions || i == l->end(); advance(i, 100), number++)
  {
    lNameComp = lName.compare((*i)->lastName);

    // if it is less than 0 we know first is less than second, so maybe type 2? Well, we know for sure it's not type 3.
    if (lNameComp < 0)
    {
      lName = (*i)->lastName;
      fName = (*i)->firstName;
      dataType = 2;
    }
    // if the last name is the same we have to check other stuff
    else if (lNameComp == 0)
    {
      // running this only here is faster, i hope
      fNameComp = fName.compare((*i)->firstName);

      // We check if the first name is in order
      if (fNameComp < 0)
      {
        fName = (*i)->firstName;
        dataType = 2;
      }

      // if last name is the same and the first name is the same it might be type 3.
      else if (fNameComp == 0)
      {
        dataType = 3;
      }
    }
    // if it is none of these, we know the data type is full RNG for sure
    else
    {
      return 1;
    }
  }

  // once we break out of this loop we have a pretty good idea of what type this is.
  return dataType;
}

int simplerSearch(string* name, int searchStart, int searchEnd, int nameType){
  vector<string>::iterator begin;
  vector<string>::iterator end;
  if (nameType){
    begin = lastNamesSorted.begin();
    end = lastNamesSorted.begin();
    advance(begin, searchStart);
    advance(end, searchEnd);
    return distance(lastNamesSorted.begin(), lower_bound(begin, end, *name));
  }
  else{
    begin = firstNamesSorted.begin();
    end = firstNamesSorted.begin();
    advance(begin, searchStart);
    advance(end, searchEnd);
    return distance(firstNamesSorted.begin(), lower_bound(begin, end, *name));
  }
}
//figures out the position of a specific name. type 0 is firstnames, type 1 is lastnames. 
int stringSearch(int nameType, string name){
  char firstChar = name[0];
  char secondChar = name[1];
  //lastnames
  if(nameType){
    switch(firstChar){
      case 'A':
      return simplerSearch(&name, 0, 15, 1);
      case 'B':
      return simplerSearch(&name, 16, 52, 1);
      case 'C':
      return simplerSearch(&name, 53, 98, 1);
      case 'D':
      return simplerSearch(&name, 99, 117, 1);
      case 'E':
      return simplerSearch(&name, 118, 124, 1);
      case 'F':
      return simplerSearch(&name, 125, 147, 1);
      case 'G':
      return simplerSearch(&name, 148, 178, 1);
      case 'H':
      return simplerSearch(&name, 179, 220, 1);
      case 'I':
      return 221;
      case 'J':
      return simplerSearch(&name, 222, 234, 1);
      case 'K':
      return simplerSearch(&name, 235, 243, 1);
      case 'L':
      return simplerSearch(&name, 244, 267, 1);
      case 'M':
      return simplerSearch(&name, 268, 313, 1);
      case 'N':
      return simplerSearch(&name, 314, 323, 1);
      case 'O':
      return simplerSearch(&name, 324, 331, 1);
      case 'P':
      return simplerSearch(&name, 332, 415, 1);
      case 'Q':
      return 360;
      case 'R':
      return simplerSearch(&name, 361, 396, 1);
      case 'S':
      return simplerSearch(&name, 396, 437, 1);
      case 'T':
      return simplerSearch(&name, 438, 450, 1);
      case 'V':
      return simplerSearch(&name, 451, 458, 1);
      case 'W':
      return simplerSearch(&name, 459, 495, 1);
      case 'Y':
      return simplerSearch(&name, 496, 497, 1);
      case 'Z':
      return simplerSearch(&name, 498, 499, 1);
    }
  }
  else{
    switch(firstChar){
      case 'A':
      return simplerSearch(&name, 0, 72, 0);
      case 'B':
      return simplerSearch(&name, 73, 99, 0);
      case 'C':
      return simplerSearch(&name, 100, 130, 0);
      case 'D':
      return simplerSearch(&name, 131, 148, 0);
      case 'E':
      return simplerSearch(&name, 149, 185, 0);
      case 'F':
      return simplerSearch(&name, 186, 191, 0);
      case 'G':
      return simplerSearch(&name, 191, 207, 0);
      case 'H':
      return simplerSearch(&name, 208, 221, 0);
      case 'I':
      return simplerSearch(&name, 222, 232, 0);
      case 'J':
      return simplerSearch(&name, 233, 285, 0);
      case 'K':
      return simplerSearch(&name, 284, 306, 0);
      case 'L':
      return simplerSearch(&name, 307, 339, 0);
      case 'M':
      return simplerSearch(&name, 340, 381, 0);
      case 'N':
      return simplerSearch(&name, 382, 396, 0);
      case 'O':
      return simplerSearch(&name, 397, 401, 0);
      case 'P':
      return simplerSearch(&name, 402, 415, 0);
      case 'Q':
      return simplerSearch(&name, 415, 416, 0);
      case 'R':
      return simplerSearch(&name, 417, 439, 0);
      case 'S':
      return simplerSearch(&name, 440, 463, 0);
      case 'T':
      return simplerSearch(&name, 464, 474, 0);
      case 'V':
      return simplerSearch(&name, 475, 483, 0);
      case 'W':
      return simplerSearch(&name, 484, 489, 0);
      case 'X':
      return simplerSearch(&name, 490, 492, 0);
      case 'Z':
      return simplerSearch(&name, 493, 499, 0);
    }
  }
  return 0;
}

// raddix sort! calss the ssn sort to sort ssns, but does some other wacky stuff later
void radixSort(list<Data *> *l)
{

  int radixing = 0;

  // step 1: sort first names
  list<Data *>::iterator it;

  vector<char*>::iterator find;

  for (it = l->begin(); it != l->end(); ++it)
  {
    //this beefy thing basically searches all the first names to figure out where it is in relation to the relevant value
    radixing = stringSearch(0, (*it) -> firstName);

    //what are the odds that more than half the list is just one name? 

    // when we break out of the loop we will have the index to stick the name in. endLoc basically just tells us where the next free location in the bucket is.
    bucketOfBuckets[radixing][endLoc[radixing]] = (*it);
    ++endLoc[radixing];
  }
  // after we break out of this loop we will have all the first names sorted.

  it = l->begin();
  // stick it back into the list
  for (int j = 0; j <= bucketOfBuckets.size(); ++j)
  {
    // pop all the values until the thing is empty
    while (startLoc[j] != endLoc[j])
    {
      // kick it out of the bucket
      (*it) = (bucketOfBuckets[j][startLoc[j]]);
      ++startLoc[j];
      ++it;
    }
    // once we break out we reset startloc and endloc to show those two are now empty.
    startLoc[j] = 0;
    endLoc[j] = 0;

    if (it == l->end())
    {
      break;
    }
  } // first names are now sorted.

  // step 2: sort last names.
  for (it = l->begin(); it != l->end(); ++it)
  {
    radixing = stringSearch(1, (*it) -> lastName);

    bucketOfBuckets[radixing][endLoc[radixing]] = (*it);
    ++endLoc[radixing];
  }
  // after we break out of this loop we will have all the last names sorted.

  it = l->begin();
  // stick it back into the list
  for (int j = 0; j <= bucketOfBuckets.size(); j++)
  {
    // get the values until empty
    while (startLoc[j] != endLoc[j])
    {
      (*it) = (bucketOfBuckets[j][startLoc[j]]);
      ++startLoc[j];
      ++it;
    }
    // once we break out we reset startloc and endloc to show those two are now empty.
    startLoc[j] = 0;
    endLoc[j] = 0;

    if (it == l->end())
    {
      break;
    }
  } // everything is now sorted.

  //step 3: why is insertion sort so fast
  insertionSortDivideAndConquer(l);
}

//returns powers of 100
int pow1k(int input){
  int output = 1;
  for (int i = 0; i < input; i++){
    output *= 100;
  }
  return output;
}

// uses raddix sort to sort ssns only
void radixSortSSN(list<Data *>* l)
{
  list<Data*>::iterator it;
  int temp;
  int right2dig;
  for (int i = 0; i < 5; ++i){
    // scan the entire list
    for (it = l -> begin(); it != l -> end(); ++it)
    {
      //clips off the rightmost i*2 digits
      temp = (convertSSN((*it)->ssn))/pow1k(i);
      right2dig = temp%100;

      //a check to make sure the end does not esceed the array size -- if it does, we overflow to a new section of the array. We only do this overflow once, so there better not be 1 million items with one value
      // if(endLoc[tempChar] >= bucketOfBuckets[0].size()){
      //   tempChar += 10;
      // }

      //store into bucket based off of right 2 digits
      bucketOfBuckets[right2dig][endLoc[right2dig]] = (*it); // stick the data pointer into the bucket
      ++endLoc[right2dig];
    }

    list<Data*>::iterator it = l -> begin();
    // sticks all the values from the relevant superbucket buckets back into the original vector.
    for (int j = 0; j < 100; ++j)
    {
      // pop all the values until the thing is empty
      while (startLoc[j] != endLoc[j])
      {
        (*it) = (bucketOfBuckets[j][startLoc[j]]);
        ++startLoc[j];
        ++it;
      }
      //reset the locations for this specific value
      startLoc[j] = 0;
      endLoc[j] = 0;

      //check the overflow bucket
      // while (startLoc[j + 10] != endLoc[j + 10])
      // {
      //   (*it) = (bucketOfBuckets[j][startLoc[j + 10]]);
      //   ++startLoc[j + 10];
      //   ++it;
      // }

      //reset the locations for the overflow bucket value
      // startLoc[j+10] = 0;
      // endLoc[j+10] = 0;
    }
  }
}

// insertion sort! Runs for the small chunks found in type2 sorting where both first and last names are the same -- so basically just insertion sorts based on ssn.
void insertionSortDivideAndConquer(list<Data *> *l)
{
  list<Data *>::iterator it = l->begin();

  string fName;
  string lName;

  fName = (*it) -> firstName;
  lName = (*it) -> lastName;

  int index = 0;

  //iterator for actually sorting the list
  list<Data *>:: iterator sorter= l -> begin();

  // we want to go through the entire list
  for (it = l->begin(); it != l->end(); ++it)
  {
    // step 1: while the first names are the same, we just iterate and chuck the Data* pointer into the superBucket. We're just kind of treating the superbucket as a vector here, because we are memory limited.  
    if((*it) -> firstName == fName){
      bucketOfBuckets[0][index] = (*it);
      ++index;
    }
    //if it's not the same, we run insertion sort, stick all the sorted stuff into the list, and continue on. 
    else{
      insertionSort(index, &sorter);
      index = 0;
      fName = (*it) -> firstName;
      --it;
    }
  }
  //runs this thing one last time to "clear" the buffer. 
  insertionSort(index, &sorter);
}

//for doing insertion sort
void insertionSort(int finalValue, list<Data *>:: iterator* sorter)
{
  int pos;
  Data* key;
  int j;

  for (pos = 1; pos < finalValue; ++pos)
  {
    //store a value as a key and take note of the value before it
    key = bucketOfBuckets[0][pos];
    j = pos - 1;

    //swap as long as the key is smaller than whatever is to its left. 
    while (j >= 0 && bucketOfBuckets[0][j]->ssn > key -> ssn)
    {
      bucketOfBuckets[0][j+1] = bucketOfBuckets[0][j];
      j = j - 1;
    }
    bucketOfBuckets[0][j + 1] = key;
  }//now the bucket is sorted up to the final value, and we can put the content back in to the function. 

  for (int i = 0; i < finalValue; ++i){
    *(*sorter) = bucketOfBuckets[0][i];
    ++(*sorter);
  }
}

void type1Sort(list<Data *> *l)
{
  radixSort(l);
}
void type2Sort(list<Data *> *l)
{
  insertionSortDivideAndConquer(l);
}
void type3Sort(list<Data *> *l)
{
  radixSortSSN(l);
}

// Gets a linked list as input, and will rewrite the linked list to be sorted
void sortDataList(list<Data *> &l)
{
  // step 1: go through the first 5 or so data members to figure out which list classification it is.
  int type = listType(&l);

  // each type has different necessities.
  switch (type)
  {
  case 1:
    type1Sort(&l); 
    break;
  case 2:
    type2Sort(&l);
    break;
  case 3:
    type3Sort(&l);
    break;
  default:
    // if we reach default, we just use bad sort.
    l.sort(myLessThanFull);
    break;
  }
}
