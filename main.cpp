#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <sstream>
#include <math.h>
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
    vector<char> tabAscii = tableASCII();
    for(int i = 0; i < line.size(); i+=3) {
        string number = line.substr(i, 3);
        for (int i = 0; i < tabAscii.size(); i++) {
            if (i+100 == stoi(number)) {
                strline.push_back(tabAscii[i]);
                for (int j = i; j > 0; j--) {
                    char a = tabAscii[j];
                    tabAscii[j] = tabAscii[j - 1];
                    tabAscii[j - 1] = a;
                }
                break;
            }
        }
    }
    return strline;
}
string str_ToMFT(const string& str ) {
    string encodedStr;
    int i;
    vector<char> tabAscii = tableASCII();
    for(auto item : str){
        for(i = 0; i < tabAscii.size(); i++){
            if(tabAscii[i] == item){
                break;
            }
        }
        encodedStr += to_string(i) + " ";
        /*
        if(i/10 > 1) {
            encodedStr += "0" + to_string(i);
        }
        else {
            encodedStr += "00" + to_string(i);
        }
        */
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
    long double probability;
    long double cumulativeProbability;
    long double left;
    long double right;
};
struct encoding {
    char encode;
    long double F;
    long double G;
    long double calcEncode(const string& lineMTF, vector<alltable>& tab){
        F = 0;
        G = 1;
        for(int i = 0; i < lineMTF.size(); i+=3){
            string str1 = lineMTF.substr(i, 3);
            for (int j = 0; j < tab.size(); j++){
                if(stoi(str1) == tab[j].intchar){
                    F = F + tab[j].cumulativeProbability * G;
                    G = G * tab[j].probability;
                    cout << F << ", " << G << endl;
                    break;
                }
            }
        }

        return -log2(G) + 1;
    }
};

vector <pair<int, long double>> sortVec(vector <pair<int, long double>>& a){
    for(int j = 0; j<a.size();j++) {
        for (int i = 0; i < a.size(); i++) {
            if (a[i + 1].first > a[i].first) {
                pair<int, long double> b = a[i];
                a[i] = a[i + 1];
                a[i + 1] = b;
            }
        }
    }
}

void sort_vector_alltable(vector<alltable>& n) {
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

void mtfto_binaryfile(const string& mtf, ofstream& fout){
    stringstream line(mtf);
    string str1;
    while(getline(line, str1, ' ')) {
        //string str1 = mtf.substr(i, 3);
        int num = stoi(str1);
        //cout << "num: " <<num << endl;
        fout.write(reinterpret_cast<const char *>(&num), sizeof(char));
    }
}

int main() {
    ifstream inFile("bib");
    if(inFile.is_open()){
        ofstream out_mtf_bwt_binary("BWT_MTF_Binary", ios::binary);
        ofstream out_bwt("BWT");
        ofstream out_mtf("MTF");
        tableFrequency freq;
        string line;
        while(!inFile.eof()){
            getline(inFile, line);
            line += '\n';
            string str_bwt = BWT(line);
            string str_mtf = str_ToMFT(str_bwt);
            out_bwt << str_bwt;
            out_mtf << str_mtf;
            cout << str_mtf << endl;
            //string dec = reverseBWT(reverseMTF(str_mtf));
            mtfto_binaryfile(str_mtf, out_mtf_bwt_binary);
            //freq.calcFreq(str_mtf);
        }
        out_mtf_bwt_binary.close();
        out_mtf.close();
        out_bwt.close();

        ifstream i("BWT_MTF_Binary" ,ios::binary);
        if(i.is_open()) {
            int toRestore=0;
            while(i.read((char *) &toRestore, sizeof(char))){
                cout << toRestore;
            };
        }
/*
        long double sum = freq.sum();
        //freq.printMap();
        cout <<"freq sum: " <<freq.sum() << endl;
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
        */
    }
    else{
        cout << "File not open" << endl;
        ifstream in("text.txt", ios::binary);
        string kl = "mississippi$";
        //string bwt = BWT(kl), mtf = str_ToMFT(bwt);
        //cout << bwt << endl << mtf << endl;
        //string revmtf = reverseMTF(mtf);
        //cout << revmtf <<endl;
        //string revBWT = reverseBWT(revmtf);
        //cout << revBWT << endl;
        string n = "173145164";
        //cout << reverseBWT(revmtf) <<endl;
    }

    return 0;

}
