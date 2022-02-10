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
//переделать разделение с  пробела на три символа
string reverseMTF(const string& line){
    string number;
    string strline;
    stringstream line2(line);
    vector<char> tabAscii = tableASCII();
    while(getline(line2, number, ' ')){
        for(int i = 0; i<tabAscii.size(); i++){
            if(i==stoi(number)){
                strline.push_back(tabAscii[i]);
                for(int j = i; j > 0; j--){
                    char a = tabAscii[j];
                    tabAscii[j] = tabAscii[j-1];
                    tabAscii[j-1] = a;
                }
                break;
            }
        }
    }
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
        encodedStr +=  to_string(100 + i) + " ";
        //alphabet table permutation
        for(int j = i; j > 0; j--){
            char a = tabAscii[j];
            tabAscii[j] = tabAscii[j-1];
            tabAscii[j-1] = a;
        }
    }
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
    return lineOut;

}
struct tableFrequency{
    map<int, unsigned int> table;
    vector <double> probability;
    void calcFreq(const string& line){
        stringstream linestream(line);
        string numberMTF;
        while(getline(linestream, numberMTF, ' ')){
            table[stoi(numberMTF)]++;
        }
    }

    void calcProbability(unsigned sum){
        for (const auto& item : table){
            probability.push_back(item.second/(double)sum);
        }
    }

    unsigned sum(){
        unsigned number = 0;
        for(auto && item : table)
            number += item.second;
        return number;
    }
    void printMap(){
        int i = -1;
        cout << "Char   Freq   Prob" << endl;
        for (const auto& item : table){
            cout << item.first << ": " << item.second << ": "  << probability[++i] << endl;
        }
    }
};

struct encoding {
    char encode;
    double low;
    double high;
    double cumulativeProbability;

    string calcEncode(const string& lineMTF){
        cumulativeProbability = 0;
        for (int i = 1; i < 95; i++){
        }
    }
};

bool comp (pair<int, double> a, pair<int, double> b){
    return a.first > b.first;
}
int main() {
    ifstream inFile("bib");
    if(inFile.is_open()){
        tableFrequency freq;
        string line;
        while(!inFile.eof()){
            getline(inFile, line);
            line += '\n';
            freq.calcFreq(str_ToMFT(BWT(line)));
        }
        unsigned s = freq.sum();
        freq.calcProbability(s);
        freq.printMap();
        cout << freq.sum() << endl;
        vector<pair<int, double>> sortedFreqProb;
        int indexProb = -1;
        for(auto& item : freq.table){
            sortedFreqProb.push_back(make_pair(item.second, freq.probability[++indexProb]));
        }
        std::sort(sortedFreqProb.begin(), sortedFreqProb.end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
            return left.second < right.second;
        });
        for(auto& item : sortedFreqProb){
            cout << item.first << " : " << item.second <<endl;
        }


    }
    else{
        cout << "File not open" << endl;
    }
    return 0;
}
