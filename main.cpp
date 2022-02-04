#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;


string shiftLine(string&& line){
    char last = line.back();
    for(int i = line.size(); i > 0; i--){
        line[i] = line[i-1];
    }
    line[0] = last;
    return line;
}

int main() {
    ifstream inFile("bib");
    if(inFile.is_open()){
        string line;
        while(!inFile.eof()){
            getline(inFile, line, ' ');
        }
    }
    else{
        cout << "File not open" << endl;
    }
    cout << shiftLine("asfdgskgs") << endl;
    return 0;
}
