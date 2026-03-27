//
// Created by bluegood on 21/03/2026.
//

#ifndef DA_PROJECT_TESTER_H
#define DA_PROJECT_TESTER_H

#include <filesystem>
#include <string>
namespace fs = std::filesystem;
using namespace std;

/**
 * @class Tester
 * @brief Helper class to compare our outputs to the expected ones
 *
 */
class Tester {
private:
    const string mainFolder;
public:
    /**
     * @brief Constructor of Tester class
     * @param mainFolder
     */
    Tester(const string& mainFolder): mainFolder(mainFolder) {};

    /**
    *@brief Takes all valid inputs in a file and executes all tasks
    */
    void executeAllInputTasks();

    /**
     * @brief Function takes two file paths and checks if they are valid and, if so, they match
     * @param filePath1
     * @param filePath2
     * @return bool value; true if files in path are equal, false otherwise
     */
    static bool areFilesEqual(const fs::path& filePath1, const fs::path& filePath2);

    /**
     * @brief Visual representation of the results of the tests performed
     */
    void compareGeneratedWithExpected();
};

#endif //DA_PROJECT_TESTER_H