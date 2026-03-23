//
// Created by bluegood on 21/03/2026.
//

#include "Tester.h"
#include "Utils.h"
#include <cstring>
#include <fstream>
#include "Parser.h"
#include "ConferenceManager.h"

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

bool Tester::areFilesEqual(const fs::path& filePath1, const fs::path& filePath2) {
    // 1. Check if both paths exist and are actually files
    if (!fs::exists(filePath1) || !fs::exists(filePath2) ||
        !fs::is_regular_file(filePath1) || !fs::is_regular_file(filePath2)) {
        return false;
        }

    // 2. Fast fail: If the sizes are different, the files are definitely different
    if (fs::file_size(filePath1) != fs::file_size(filePath2)) {
        return false;
    }

    // 3. Open both files in binary mode
    std::ifstream file1(filePath1, std::ios::binary);
    std::ifstream file2(filePath2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        return false; // Failed to open one or both files
    }

    // 4. Compare the contents in chunks to keep memory usage low
    const size_t bufferSize = 8192; // 8KB buffer
    std::vector<char> buffer1(bufferSize);
    std::vector<char> buffer2(bufferSize);

    do {
        // Read a chunk from both files
        file1.read(buffer1.data(), bufferSize);
        file2.read(buffer2.data(), bufferSize);

        // Check how many bytes were actually read
        std::streamsize bytesRead1 = file1.gcount();
        std::streamsize bytesRead2 = file2.gcount();

        // If read amounts differ, or if the memory contents don't match exactly, they aren't equal
        if (bytesRead1 != bytesRead2 ||
            std::memcmp(buffer1.data(), buffer2.data(), bytesRead1) != 0) {
            return false;
            }
    } while (file1.good() && file2.good());

    return true; // If we made it here, the files are identical
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
        // Iterate through all files in the "output" directory (the expected files)
        for (const auto& entry : fs::directory_iterator(this->mainFolder + "/output/")) {
            if (entry.is_regular_file()) {
                totalCount++;

                fs::path expectedPath = entry.path();
                std::string originalFilename = expectedPath.filename().string();

                // Construct the path for the generated file.
                // Based on your previous files, they have an "output_" prefix.
                fs::path generatedPath = this->mainFolder + "/generated/" + originalFilename;

                // Check if the generated file even exists
                if (!fs::exists(generatedPath)) {
                    std::cout << FG_RED << "[-] FAIL: " << originalFilename
                              << " -> Missing generated file: " << TXT_RESET << generatedPath.filename().string() << '\n';
                    allPassed = false;
                    continue;
                }

                // Compare the files using the areFilesEqual method
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
