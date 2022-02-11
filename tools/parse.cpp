#include <string>
#include <iostream>

using namespace std;




int next_token(string & tokenized, int i, string & out){
    int k = tokenized.find(" ", i);
    out = tokenized.substr(i, k);
    return k + 1;
}


int main(int argc, char ** argv){
    string s = "set fl 0.1";
    if(argc == 2){
        s = argv[1];
    }
    int i = 0;
    string command;
    i = next_token(s, i, command);
    cout << "Token 1 : " << command << "\n";
}