#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Employee.h"

using namespace std;


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) throw runtime_error("Usage: Creator <filename> <record_count>");

        ofstream file(argv[1], ios::binary);
        if (!file) throw runtime_error("Cannot create file: " + string(argv[1]));

        int count = stoi(argv[2]);
        employee e;

        for (int i = 0; i < count; ++i) {
            cout << "Employee " << i + 1 << ":\n";
            cout << "ID: "; cin >> e.num;
            cout << "Name (max 9 chars): "; cin >> e.name;
            cout << "Hours: "; cin >> e.hours;
            file.write(reinterpret_cast<char*>(&e), sizeof(e));
        }

    }
    catch (const exception& e) {
        cerr << "[Creator] Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}