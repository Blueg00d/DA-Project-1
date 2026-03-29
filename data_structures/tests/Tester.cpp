//
// Created by bluegood on 21/03/2026.
//

#include "Tester.h"
#include "utils/Utils.h"
#include "io/Parser.h"
#include "core/Brainer.h"
#include "io/FileManager.h"
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void Tester::executeAllInputTasks() {
    // Iterating through all files in the "input" directory
    for (const auto& entry: fs::directory_iterator(this->mainFolder + "/input/")) {
        if (entry.is_regular_file()) {
            Parser parser;
            parser.parseFile(entry.path().string());

            Brainer manager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            manager.executeAllTasks(this->mainFolder + "/generated/");
        }
    }
}

void Tester::compareGeneratedWithExpected() {
    std::cout << CLR_ALL << FG_YELLOW << TXT_BOLD;
    std::cout << "======================================\n";
    std::cout << "   Comparing Generated vs Expected    \n";
    std::cout << "======================================\n";
    std::cout << TXT_RESET;

    bool allPassed = true;
    int passedCount = 0;
    int totalCount = 0;

    try {
        // Iterating through all files in the "output" directory (the expected files)
        for (const auto& entry : fs::directory_iterator(this->mainFolder + "/output/")) {
            if (entry.is_regular_file()) {
                totalCount++;

                fs::path expectedPath = entry.path();
                std::string originalFilename = expectedPath.filename().string();

                // Constructing the path for the generated file. They have an "output_" prefix.
                fs::path generatedPath = this->mainFolder + "/generated/" + originalFilename;

                // Checking if the generated file even exists
                if (!fs::exists(generatedPath)) {
                    std::cout << FG_RED << "[-] FAIL: " << originalFilename
                              << " -> Missing generated file: " << TXT_RESET << generatedPath.filename().string() << '\n';
                    allPassed = false;
                    continue;
                }

                // Comparing the files using the areFilesEqual method from FileManager
                if (FileManager::areFilesEqual(generatedPath, expectedPath)) {
                    std::cout << FG_GREEN << "[+] PASS: " << TXT_RESET << originalFilename << '\n';
                    passedCount++;
                } else {
                    std::cout << FG_RED << "[-] FAIL: " << TXT_RESET << originalFilename << " -> Contents do not match!\n";
                    allPassed = false;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << FG_RED << TXT_BOLD << TXT_INVERT << "Filesystem error: " << e.what() << '\n';
        return;
    }

    std::cout << "======================================\n";
    if (allPassed && totalCount > 0) {
        std::cout << FG_GREEN << TXT_BOLD << TXT_INVERT << "SUCCESS: All " << totalCount << " tests passed!\n" << TXT_RESET;
    } else {
        std::cout << FG_RED << TXT_BOLD << TXT_INVERT << "WARNING: " << (totalCount - passedCount) << " out of " << totalCount << " tests failed.\n" << TXT_RESET;
    }
}