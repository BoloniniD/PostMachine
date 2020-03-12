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
    getline(inp_line, str);
    try {
        curr = stoi(str) - 1;
    } catch (std::invalid_argument& e) {
        cerr << "Cannot extract head's position from " << name + ".postline!\nSetting position to 1!\n";
        curr = 0;
    }
    getline(inp_line, str);
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '1') {
            line.push_back(true);
        } else if (str[i] == '0') {
            line.push_back(false);
        } else {
            cerr << "Unexpected symbol: " << str[i] << " in " << name << ".postline!" << "\n";
            return 0;
        }
    }
    inp_line.close();
    for (size_t i = 0; i < prog.size(); ++i) {
        /*cout << curr << ' ';
        for (size_t i = 0; i < line.size(); ++i) {
            cout << static_cast<char>(line[i]);
        }
        cout << '\n';*/
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
                    break;
                } else {
                    line[curr] = true;
                }
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        } else if (pa1.first == "X") {
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
                    break;
                } else {
                    line[curr] = false;
                }
            } catch (std::invalid_argument& e) {
                cerr << "Expected integer or empty space in line " << i << ", found " << pa2.first << '\n';
                break;
            }
        } else if (pa1.first == "<") {
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
        } else if (pa1.first == ">") {
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
        } else if (pa1.first == "?") {
            auto pa2 = word(str, pa1.second);
            auto pa3 = word(str, pa2.second);
            if (!line[curr]) {
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
        } else if (pa1.first == "!") {
            cout << "Found !, shutting down...\n";
            break;
        }
    }
    ofstream fout;
    fout.open(name + " output.postline");
    fout << to_string(curr) << '\n';
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i]) {
            fout << "1";
        } else {
            fout << "0";
        }
    }
    fout.close();
}
