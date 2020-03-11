#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<bool> line;
    vector<string> prog;
    ifstream inp_line, inp_prog;
    string name, str;
    cout << "Awaiting post machine program - *.postcfg filename: ";
    cin >> name;
    inp_prog.open(name + ".postcfg");
    while (!inp_prog) {
        inp_prog.close();
        cerr << "\nFile doesn't exist! Retry: ";
        cin >> name;
        inp_prog.open(name + ".postcfg");
    }
    cout << "\nSuccess!\n\n";
    while (inp_prog) {
        getline(inp_prog, str);
        prog.push_back(str);
    }
    inp_prog.close();
    cout << "Awaiting post machine mark line - *.postline filename: ";
    cin >> name;
    inp_line.open(name + ".postline");
    while (!inp_line) {
        inp_line.close();
        cerr << "\nFile doesn't exist! Retry: ";
        cin >> name;
        inp_line.open(name + ".postline");
    }
    cout << "\nSuccess!\n\n";
    while (inp_line) {
        getline(inp_line, str);
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == '1') {
                line.push_back(true);
            } else if (str[i] == '0') {
                line.push_back(false);
            } else {
                cerr << "UNEXPECTED SYMBOL " << str[i] << " IN " << name << ".postline!" << "\n";
                return 0;
            }
        }
    }
    inp_line.close();
}
