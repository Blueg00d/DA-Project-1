#include "FileManager.h"
#include "utils/Utils.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;
namespace fs = std::filesystem;

vector<string> FileManager::readLines(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    // Check if the file was successfully opened
    if (!file.is_open()) {
        cerr << TXT_BOLD << FG_RED << TXT_INVERT << "ERROR OPENING FILE!" << endl << TXT_RESET;
        return lines;
    }

    string line;
    // Read the file line by line
    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

bool FileManager::writeLines(const string& filename, const vector<string>& lines) {
    ofstream outFile(filename);

    // Check if the file was successfully opened for writing
    if (!outFile.is_open()) {
        cerr << TXT_BOLD << FG_RED << TXT_INVERT << "ERROR OPENING FILE!" << endl << TXT_RESET;
        return false;
    }

    // Write all lines to the file
    for (const string& line : lines) {
        outFile << line << "\n";
    }

    outFile.close();
    cout << FG_GREEN << TXT_INVERT << "Success! Results saved in: " << filename << TXT_RESET << endl;
    return true;
}

bool FileManager::areFilesEqual(const fs::path& filePath1, const fs::path& filePath2) {
    // Validate that both paths exist and are regular files
    if (!fs::exists(filePath1) || !fs::exists(filePath2) ||
        !fs::is_regular_file(filePath1) || !fs::is_regular_file(filePath2)) {
        return false;
    }

    // Refactored to compare textually rather than in strict binary.
    // This safely ignores Windows (CRLF) vs Unix (LF) newline differences
    // which commonly cause false-negative file size / binary mismatches.
    std::ifstream file1(filePath1);
    std::ifstream file2(filePath2);

    if (!file1.is_open() || !file2.is_open()) {
        return false;
    }

    std::string line1, line2;
    while (true) {
        bool has1 = (bool)std::getline(file1, line1);
        bool has2 = (bool)std::getline(file2, line2);

        // Normalize line endings by stripping trailing carriage returns (\r)
        if (!line1.empty() && line1.back() == '\r') line1.pop_back();
        if (!line2.empty() && line2.back() == '\r') line2.pop_back();

        // If one file ends before the other
        if (has1 != has2) return false;

        // If both reached EOF simultaneously
        if (!has1) break;

        // Compare the normalized strings
        if (line1 != line2) {
            return false;
        }
    }

    return true;
}