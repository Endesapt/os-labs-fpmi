#include "Reporter.h"


void generateReport(const string& inputFile, const string& outputFile, double wage) {
    ifstream in(inputFile, ios::binary);
    if (!in) throw runtime_error("Cannot open input file: " + inputFile);

    vector<employee> employees;
    employee e;
    while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        employees.push_back(e);
    }

    sort(employees.begin(), employees.end(), [](const employee& a, const employee& b) {
        return a.num < b.num;
        });

    ofstream out(outputFile);
    if (!out) throw runtime_error("Cannot create report: " + outputFile);

    out << "Report for file: " << inputFile << "\n";
    out << "ID\tName\tHours\tSalary\n";
    for (const auto& emp : employees) {
        out << emp.num << "\t" << emp.name << "\t" << emp.hours << "\t" << emp.hours * wage << "\n";
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) throw runtime_error("Usage: Reporter <input> <output> <wage>");
        generateReport(argv[1], argv[2], stod(argv[3]));
    }
    catch (const exception& e) {
        cerr << "[Reporter] Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}