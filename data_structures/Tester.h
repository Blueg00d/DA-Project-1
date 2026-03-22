//
// Created by bluegood on 21/03/2026.
//

#ifndef DA_PROJECT_TESTER_H
#define DA_PROJECT_TESTER_H

#include <filesystem>
#include <string>
namespace fs = std::filesystem;
using namespace std;

class Tester {
private:
    const string mainFolder;
public:
    Tester(const string& mainFolder): mainFolder(mainFolder) {};
    void executeAllInputTasks();

    static bool areFilesEqual(const fs::path& filePath1, const fs::path& filePath2);
    void compareGeneratedWithExpected();
};

#endif //DA_PROJECT_TESTER_H