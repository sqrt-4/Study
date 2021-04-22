////////////////////ТВЕРСКОЙ А.Е./////////////////////
#include <iostream>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <vector>
#include "process.h"

using namespace std;

int main(){
    string p;
    string arg;
    vector<string>argv;
    cin>>p;
    cin>>arg;
    argv.push_back(arg);

    try{
        Process A(p,argv);
        /*код*/
       return 0;
    }

    catch(runtime_error err){
        cerr<<err.what()<<endl;
        _exit(-1);
    }
}