//
// Created by bluegood on 21/03/2026.
//

#include "Tester.h"
#include "Utils.h"
#include <cstring>
#include <fstream>
#include "Parser.h"
#include "ConferenceManager.h"

/**
 * @copybrief
 * Time complexity: O(F* )
 * Parses and executes executeAllTasks() once for each number of files F
 */
void Tester::executeAllInputTasks() {
    for (const auto& entry: fs::directory_iterator(this->mainFolder + "/input/")) {
        if (entry.is_regular_file()) {
            Parser parser;
            parser.parseFile(entry.path().string());

            ConferenceManager manager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            manager.executeAllTasks(this->mainFolder + "/generated/");
        }
    }
}

namespace fs = std::filesystem;

/**
 * @copydoc areFilesEqual
 * Time complexity: O(N)
 * Size in bytes of the files being read is N
 */
bool Tester::areFilesEqual(const fs::path& filePath1, const fs::path& filePath2) {
    //Checking if both paths exist and are actually files
    if (!fs::exists(filePath1) || !fs::exists(filePath2) ||
        !fs::is_regular_file(filePath1) || !fs::is_regular_file(filePath2)) {
        return false;
        }

    //If the sizes are different, the files are different
    if (fs::file_size(filePath1) != fs::file_size(filePath2)) {
        return false;
    }

    //Opening both files in binary mode
    std::ifstream file1(filePath1, std::ios::binary);
    std::ifstream file2(filePath2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        return false; // Failed to open one or both files
    }

    //Comparing the contents in chunks to keep memory usage low
    const size_t bufferSize = 8192; // 8KB buffer
    std::vector<char> buffer1(bufferSize);
    std::vector<char> buffer2(bufferSize);

    do {
        //reading a chunk from both files
        file1.read(buffer1.data(), bufferSize);
        file2.read(buffer2.data(), bufferSize);

        //Checking how many bytes were read
        std::streamsize bytesRead1 = file1.gcount();
        std::streamsize bytesRead2 = file2.gcount();

        // If read amounts differ, or if the memory contents don't match exactly, they aren't equal
        if (bytesRead1 != bytesRead2 ||
            std::memcmp(buffer1.data(), buffer2.data(), bytesRead1) != 0) {
            return false;
            }
    } while (file1.good() && file2.good());

    return true; //The files are identical
}

/**
 * @copybrief
 * Time complexity: O(F*N)
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

    try {
        // Iterating through all files in the "output" directory (the expected files)
        for (const auto& entry : fs::directory_iterator(this->mainFolder + "/output/")) {
            if (entry.is_regular_file()) {
                totalCount++;

                fs::path expectedPath = entry.path();
                std::string originalFilename = expectedPath.filename().string();

                // Constructing the path for the generated file. They have an "output_" prefix.
                fs::path generatedPath = this->mainFolder + "/generated/output_" + originalFilename;

                // Checking if the generated file even exists
                if (!fs::exists(generatedPath)) {
                    std::cout << FG_RED << "[-] FAIL: " << originalFilename
                              << " -> Missing generated file: " << TXT_RESET << generatedPath.filename().string() << '\n';
                    allPassed = false;
                    continue;
                }

                //Comparing the files using the areFilesEqual method
                if (areFilesEqual(generatedPath, expectedPath)) {
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
