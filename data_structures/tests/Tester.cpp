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

/**
 * @copybrief executeAllInputTasks
 * Parses and executes executeAllTasks() once for each number of files F
 */
void Tester::executeAllInputTasks() {
    string inputPath = this->mainFolder + "/input/";

    // FIX: Verify the directory exists before constructing a directory_iterator
    if (!fs::exists(inputPath) || !fs::is_directory(inputPath)) {
        std::cerr << FG_RED << TXT_BOLD << TXT_INVERT << "ERROR: Input directory does not exist or is not a valid directory: " << inputPath << TXT_RESET << '\n';
        return;
    }

    try {
        // Iterating through all files in the "input" directory
        for (const auto& entry: fs::directory_iterator(inputPath)) {
            if (entry.is_regular_file()) {
                Parser parser;
                parser.parseFile(entry.path().string());

                Brainer manager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
                manager.executeAllTasks(this->mainFolder + "/generated/");
            }
        }
    } catch (const fs::filesystem_error& e) {
        // FIX: Gracefully catch any filesystem-level exceptions
        std::cerr << FG_RED << TXT_BOLD << TXT_INVERT << "Filesystem error while accessing input tasks: " << e.what() << TXT_RESET << '\n';
    }
}

/**
 * @copybrief compareGeneratedWithExpected
 * Size in bytes of the files being compared in areFilesEqual is N
 * Number of files expected is F
 */
void Tester::compareGeneratedWithExpected() {
    std::cout << CLR_ALL << FG_YELLOW << TXT_BOLD;
    std::cout << "======================================\n";
    std::cout << "   Comparing Generated vs Expected    \n";
    std::cout << "======================================\n";
    std::cout << TXT_RESET;

    bool allPassed = true;
    int passedCount = 0;
    int totalCount = 0;

    string outputPath = this->mainFolder + "/output/";

    // FIX: Verify the directory exists before constructing a directory_iterator
    if (!fs::exists(outputPath) || !fs::is_directory(outputPath)) {
        std::cerr << FG_RED << TXT_BOLD << TXT_INVERT << "ERROR: Output directory does not exist or is not a valid directory: " << outputPath << TXT_RESET << '\n';
        return;
    }

    try {
        // Iterating through all files in the "output" directory (the expected files)
        for (const auto& entry : fs::directory_iterator(outputPath)) {
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
        std::cerr << FG_RED << TXT_BOLD << TXT_INVERT << "Filesystem error: " << e.what() << TXT_RESET << '\n';
        return;
    }

    std::cout << "======================================\n";
    if (allPassed && totalCount > 0) {
        std::cout << FG_GREEN << TXT_BOLD << TXT_INVERT << "SUCCESS: All " << totalCount << " tests passed!\n" << TXT_RESET;
    } else {
        std::cout << FG_RED << TXT_BOLD << TXT_INVERT << "WARNING: " << (totalCount - passedCount) << " out of " << totalCount << " tests failed.\n" << TXT_RESET;
    }
}