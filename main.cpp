#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <sstream>
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
//надо сделать поиск по концу строки, пока что не работает
string reverseBWT(const string& line){
    vector <string> vecStr;
    string str = line;
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
    for (auto item : vecStr){
        if(item[item.size() - 1] == '\n'){
            return item;
        }
    }
}



vector<char> tableASCII(){
    vector<char> table;
    char a = '\n';
    for (char i = ' '; i <= '~'; ++i) {
        table.push_back(i);
    }
    table.push_back(a);
    return table;
}
string reverseMTF(const string& line){
    string number;
    string strline;
    stringstream line2(line);
    vector<char> tabAscii = tableASCII();
    while(getline(line2, number, ' ')){
        for(int i = 0; i<tabAscii.size(); i++){
            if(i==stoi(number)){
                strline.push_back(tabAscii[i]);
                //char searchedSymbol = tabAscii[i];
                for(int j = i; j > 0; j--){
                    char a = tabAscii[j];
                    tabAscii[j] = tabAscii[j-1];
                    tabAscii[j-1] = a;
                }
                //tabAscii[0] = searchedSymbol;
                break;
            }
        }
    }
    //cout << " size reverse tab:"<<tabAscii.size()<<endl;
    //cout<< endl <<"rev:" << strline<<endl;
    return strline;
}
string str_ToMFT(const string& str) {
    vector<char> tabAscii = tableASCII();
    string encodedStr;
    int i;
    for(auto item : str){

        for(i = 0; i < tabAscii.size(); i++){
            if(tabAscii[i] == item){
                break;
            }
        }
        encodedStr += to_string(i) + " ";
        //alphabet table permutation
        //char searchedSymbol = tabAscii[i];
        for(int j = i; j > 0; j--){
            char a = tabAscii[j];
            tabAscii[j] = tabAscii[j-1];
            tabAscii[j-1] = a;
        }
        //tabAscii[0] = searchedSymbol;
    }

    //cout << " mtf size:" << tabAscii.size() << endl;
    //cout << "mtf:" << encodedStr << endl;
    return encodedStr;
}

string BWT(string& line){
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
    //cout << "bwt:" << lineOut << endl;
    return lineOut;

}


int main() {
    ifstream inFile("bib");
    if(inFile.is_open()){
        string line;
        while(!inFile.eof()){
            getline(inFile, line);
            line += '\n';
            cout << reverseBWT(reverseMTF( str_ToMFT(BWT(line))));
        }
    }
    else{
        cout << "File not open" << endl;
    }
    //string kl = "Is mail Bayram ov asda&&~%ds\n", kl2 = "Ismail Bayramov\n";
    //string rt = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    //string  a = BWT(kl2), b = str_ToMFT(a), c = reverseMTF(b), d = reverseBWT(c);
    vector<char> tabAscii = tableASCII();
    int i = 0;
    for(auto item : tabAscii){
        cout<< item << ((int(item)%16 == 15) ? '\n': ' ');
    }
    return 0;
}
