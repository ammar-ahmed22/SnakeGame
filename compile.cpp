#include <iostream>

using namespace std;


int main();

int main(){

    string filename;

    cout << "Welcome to C++ Compiler!" << endl;
    cout << "==========================================================" << endl;
    cout << "Enter filename (witout suffix): ";
    cin >> filename;


    string compileCall = "g++ " + filename + ".cpp -o " + filename;
   

    const char * compileSystem = compileCall.c_str(); 

    system(compileSystem);



    return 0;
}

