#include <iostream>
using namespace std;

int main(){
    try {
  while (true) {cout << "."; cout.flush(); new char[1'000'000];}
}
catch (bad_alloc& b) {
  cerr << "Memory exhausted: " << b.what() << '\n';
}

}