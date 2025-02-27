#include "Main.h"

void printBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) throw runtime_error("Cannot open file: " + filename);

    employee e;
    while (file.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        cout << "ID: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << endl;
    }
    file.close();
}

void runProcess(const string& exeName, const string& args) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION pi;
    std::string cmdStr = exeName +" "+args;
    char* cmd=&cmdStr[0];
    if (!CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        throw runtime_error("Failed to start " + exeName);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    try {
        string binFile, reportFile;
        int recordCount;
        double wage;
        cout << "Enter binary file name: ";
        cin >> binFile;
        cout << "Enter number of records: ";
        cin >> recordCount;
        runProcess("Creator.exe", "\"" + binFile + "\" " + to_string(recordCount));


        cout << "\nBinary file content:\n";
        printBinaryFile(binFile);

        cout << "\nEnter report file name: ";
        cin >> reportFile;
        cout << "Enter hourly wage: ";
        cin >> wage;
        runProcess("Reporter.exe", "\"" + binFile + "\" \"" + reportFile + "\" " + to_string(wage));

        cout << "\nReport content:\n";
        ifstream report(reportFile);
        string line;
        while (getline(report, line)) cout << line << endl;

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}