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
        encodedStr +=  to_string(100 + i);
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
struct tableFrequency {
    map<int, int> table;

    void calcFreq(const string &line) {
       // stringstream linestream(line);
       // string numberMTF;
        for(int i = 0; i < line.size(); i+=3){
            table[stoi(line.substr(i,3))]++;
            int a = 5;
        }
        /*
        while (getline(linestream, numberMTF, ' ')) {
            table[stoi(numberMTF)]++;
        }
         */
    }

    int sum(){
        int number = 0;
        for(auto && item : table)
            number += item.second;
        return number;
    }
    void printMap(){
        int i = -1;
        cout << "Char   Freq   Prob" << endl;
        for (const auto& item : table){
            cout << item.first << ": " << item.second << endl;
        }
    }
};
struct alltable{
    int intchar;
    int freq;
    double probability;
    double cumulativeProbability;
};
struct encoding {
    char encode;
    double F;
    double G;
    string calcEncode(const string& lineMTF, vector<alltable>& tab){
        F = 0;
        G = 1;
        for(int i = 0; i < lineMTF.size(); i+=3){
            string str1 = lineMTF.substr(i, 3);
            for (int j = 0; j < tab.size(); j++){
                if(stoi(str1) == tab[j].intchar){
                    F+=tab[j].cumulativeProbability* G;
                    G *= tab[j].probability;
                    cout << F << ", " << G << endl;
                    break;
                }
            }
        }
    }
};

vector <pair<int, double>> sortVec(vector <pair<int, double>>& a){
    for(int j = 0; j<a.size();j++) {
        for (int i = 0; i < a.size(); i++) {
            if (a[i + 1].first > a[i].first) {
                pair<int, double> b = a[i];
                a[i] = a[i + 1];
                a[i + 1] = b;
            }
        }
    }
}

void sort_vector_alltable(vector<alltable>& n) {
    int kbj;
    double dfs;
    for (int j = 0; j < n.size(); j++) {
        for (int i = 0; i < n.size(); i++) {
            if (n[i].freq < n[i+1].freq) {
                alltable b = n[i];
                n[i] = n[i+1];
                n[i+1] = b;
            }
        }
    }
}
void calculate_cumulativeProbability(vector<alltable>& tab){
    tab[0].cumulativeProbability = 0;
    for (int i = 1; i < tab.size(); i++){
        tab[i].cumulativeProbability = tab[i-1].cumulativeProbability + tab[i-1].probability;
    }
}
int main() {
    ifstream inFile("bib4567");
    if(inFile.is_open()){
        tableFrequency freq;
        string line;
        while(!inFile.eof()){
            getline(inFile, line);
            line += '\n';
            freq.calcFreq(str_ToMFT(BWT(line)));
        }
        double sum = freq.sum();
        freq.printMap();
        cout << freq.sum() << endl;
        vector<alltable> vec;
        alltable a{};
        for(auto& item : freq.table){
            a.intchar = item.first;
            a.freq = item.second;
            vec.push_back(a);
        }
        //sort
        sort_vector_alltable(vec);
        calculate_cumulativeProbability(vec);
        cout << "Char   Freq   Prob" << endl;
        for (auto& item : vec){
            item.probability = item.freq/sum;
            cout << item.intchar << " : " << item.freq << " : " << item.probability <<endl;
        }
        cout << vec[0].freq;
    }
    else{
        cout << "File not open" << endl;
    }
    string kl = "ismail bayramov\n";
    tableFrequency freq;
    freq.calcFreq(str_ToMFT(BWT(kl)));
    double sum = freq.sum();
    freq.printMap();
    cout << freq.sum() << endl;
    vector<alltable> vec;
    alltable a{};
    for(auto& item : freq.table){
        a.intchar = item.first;
        a.freq = item.second;
        vec.push_back(a);
    }
    //sort
    sort_vector_alltable(vec);
    for (auto& item : vec){
        item.probability = item.freq/sum;
    }
    calculate_cumulativeProbability(vec);
    cout << "Char Freq  Prob  Cumulative" << endl;
    for (auto& item : vec){
        cout << item.intchar << " : " << item.freq << " : " << item.probability<< " : " <<item.cumulativeProbability <<endl;
    }
    encoding enc{};
    enc.calcEncode(str_ToMFT(BWT(kl)), vec);
    cout << "Here: "<< enc.F << ", " << enc.G << endl;
    return 0;
}
