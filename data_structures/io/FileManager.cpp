#include "FileManager.h"
#include "utils/Utils.h"
#include <fstream>
#include <iostream>
#include <cstring>

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
    cout << "success! results saved in: " << filename << endl;
    return true;
}

bool FileManager::areFilesEqual(const fs::path& filePath1, const fs::path& filePath2) {
    // Validate that both paths exist and are regular files
    if (!fs::exists(filePath1) || !fs::exists(filePath2) ||
        !fs::is_regular_file(filePath1) || !fs::is_regular_file(filePath2)) {
        return false;
    }

    // Quick size check: if sizes differ, files are definitely not equal
    if (fs::file_size(filePath1) != fs::file_size(filePath2)) {
        return false;
    }

    std::ifstream file1(filePath1, std::ios::binary);
    std::ifstream file2(filePath2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        return false;
    }

    // Read and compare the files in chunks to optimize memory usage
    const size_t bufferSize = 8192;
    std::vector<char> buffer1(bufferSize);
    std::vector<char> buffer2(bufferSize);

    do {
        file1.read(buffer1.data(), bufferSize);
        file2.read(buffer2.data(), bufferSize);

        std::streamsize bytesRead1 = file1.gcount();
        std::streamsize bytesRead2 = file2.gcount();

        if (bytesRead1 != bytesRead2 ||
            std::memcmp(buffer1.data(), buffer2.data(), bytesRead1) != 0) {
            return false;
        }
    } while (file1.good() && file2.good());

    return true;
}