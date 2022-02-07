#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;


string shiftLine(string& line){
    char last = line.back();
    for(int i = line.size(); i > 0; i--){
        line[i] = line[i-1];
    }
    line[0] = last;
    return line;
}
void printVector(vector<string> &a){
    for(const auto& item : a){
        cout << item << endl;
    }
}
//for decoder
void reverseBWT(const pair<string, int>& strAndIndex){
    vector <string> vecStr;
    int index = strAndIndex.second;
    string str = strAndIndex.first;

    for(auto item : str){
        vecStr.push_back(string{item});
    }
    sort(vecStr.begin(), vecStr.end());

    for(int i = 1; i < str.size(); i++){
        for(int j = 0; j < str.size(); j++){
            vecStr[j] = str[j] + vecStr[j];
        }
        sort(vecStr.begin(), vecStr.end());
    }

}
//for decoder
vector<char> tableASCII(){
    vector<char> table;
    char a = '\n';
    table.push_back(a);
    table.reserve(97);
    for (char i = ' '; i <= '~'; ++i) {
        table.push_back(i);
    }
    return table;
}
string str_ToMFT(const string& str) {
    vector<char> tabAscii = tableASCII();
    string encodedStr;
    int cs = 0;
    for(auto item : str){
        int i;
        for(i = 0; i < tabAscii.size(); i++){
            if(tabAscii[i] == item){
                break;
            }
        }
        encodedStr += to_string(i) + " ";

        //alphabet table permutation
        /*
        for(auto ascii : tabAscii){
            cout << int(ascii) << ((int(ascii) % 16 == 15) ? '\n' : ' ');
        }
        cout << endl;
         */
        char searchedSymbol = tabAscii[i];
        for(int j = 0; j < i; j++){
            char a = tabAscii[j+1];
            tabAscii[j+1] = tabAscii[j];
            tabAscii[j] = a;
        }
        tabAscii[0] = searchedSymbol;
        //cout << endl;
    }
    cout << encodedStr << endl;
    return encodedStr;
}

pair<string, int> BWT(string& line){
    vector<string> arrayLines;
    arrayLines.reserve(line.size());
    for(int i = 0; i < line.size(); i++) {
        arrayLines.push_back(shiftLine(line));
    }
    sort(arrayLines.begin(), arrayLines.end());
    int pos = -1, buffPos;
    string lineOut;
    for ( auto item : arrayLines){
        pos++;
        if(line == item)
            buffPos = pos;
        lineOut += item.back();
    }
    //printVector(arrayLines);
    pair<string, int> out = make_pair(lineOut, buffPos);
    cout << out.first << " " << out.second << endl;
    return out;

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
    string kl = "\\fIAdditional key words and phrases:\\fR  arithmetic coding, Huffman coding, adaptive modeling\n";
    //reverseBWT(BWT(kl));
    str_ToMFT(BWT(kl).first);
    vector<char> tabAscii = tableASCII();
    for(auto item : tabAscii){
        cout << item << ((int(item)%16 == 15) ? '\n': ' ');
    }
    return 0;
}
