#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file.\n";
        return 1;
    }

    int lines = 0, words = 0, characters = 0;
    string line;
    while (getline(file, line)) {
        lines++;
        characters += line.length();
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words++;
        }
    }
    file.close();

    cout << "Lines: " << lines << endl;
    cout << "Words: " << words << endl;
    cout << "Characters: " << characters << endl;

    return 0;
}