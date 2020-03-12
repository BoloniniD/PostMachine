#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

pair<string, int> word(string s, int j) {
    string r ;
    int t = j;
    for (size_t i = j; i <= s.size(); ++i) {
        t = i;
        if (t >= s.size()) {
            break;
        }
        if (s[i] == ' ') {
            ++t;
            break;
        } else {
            r += s[i];
        }
    }
    return make_pair(r, t);
}

int main() {
    vector<bool> line;
    int curr = 0;
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
    for (size_t i = 0; i < prog.size(); ++i) {
        str = prog[i];
        auto pa1 = word(str, 0);
        if (curr < 0 && curr >= line.size()) {
            cerr << "Machine's head is out of sequence!\nShutting down...\n";
            break;
        }
        if (pa1.first == "V") {
            auto pa2 = word(str, pa1.second);
            try {
                if (pa2.first == "") {
                    int temp = i + 1;
                    i = temp - 1;
                } else {
                    int temp = stoi(pa2.first);
                    i = temp - 1;
                }
                if (line[curr]) {
                    cerr << "Marking an already marked slot!\nShutting down...\n";
                } else {
                    line[curr] = true;
                }
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        }
        if (pa1.first == "X") {
            auto pa2 = word(str, pa1.second);
            try {
                if (pa2.first == "") {
                    int temp = i + 1;
                    i = temp - 1;
                } else {
                    int temp = stoi(pa2.first);
                    i = temp - 1;
                }
                if (!line[curr]) {
                    cerr << "Cannot remove a mark from an unmarked slot!\nShutting down...\n";
                } else {
                    line[curr] = false;
                }
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        }
        if (pa1.first == "<") {
            auto pa2 = word(str, pa1.second);
            try {
                if (pa2.first == "") {
                    int temp = i + 1;
                    i = temp - 1;
                } else {
                    int temp = stoi(pa2.first);
                    i = temp - 1;
                }
                --curr;
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        }
        if (pa1.first == ">") {
            auto pa2 = word(str, pa1.second);
            try {
                if (pa2.first == "") {
                    int temp = i + 1;
                    i = temp - 1;
                } else {
                    int temp = stoi(pa2.first);
                    i = temp - 1;
                }
                ++curr;
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        }
        if (pa1.first == "?") {
            auto pa2 = word(str, pa1.second);
            auto pa3 = word(str, pa2.second);
            if (line[curr]) {
                try {
                    auto temp = stoi(pa2.first);
                    i = temp - 1;
                } catch (std::invalid_argument& e) {
                    cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                    break;
                }
            } else {
                try {
                    auto temp = stoi(pa3.first);
                    i = temp - 1;
                } catch (std::invalid_argument& e) {
                    cerr << "Expected integer or empty space in line " << i << ", found " << pa3.first << '\n';
                    break;
                }
            }
        }
        if (pa1.first == "!") {
            cout << "Found !, shutting down...\n";
            break;
        }
    }
    ofstream fout;
    fout.open(name + " output.postline");
    for (size_t i = 0; i < line.size(); ++i) {
        fout << static_cast<int>(line[i]);
    }
    fout.close();
}
