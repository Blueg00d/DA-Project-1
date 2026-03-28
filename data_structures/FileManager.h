#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <filesystem>

/**
 * @class FileManager
 * @brief Helper class to handle file reading, writing, and comparison.
 */
class FileManager {
public:
    /**
     * @brief Reads all lines from a given file.
     * @param filename The path to the file to be read.
     * @return A vector of strings containing the lines. Empty on failure.
     */
    static std::vector<std::string> readLines(const std::string& filename);

    /**
     * @brief Writes a vector of lines to a given file.
     * @param filename The path to the file to be written.
     * @param lines The lines of text to write to the file.
     * @return true if successful, false otherwise.
     */
    static bool writeLines(const std::string& filename, const std::vector<std::string>& lines);

    /**
     * @brief Compares two files byte-by-byte to check if they are identical.
     * @param filePath1 First file path.
     * @param filePath2 Second file path.
     * @return true if files are identical, false otherwise.
     */
    static bool areFilesEqual(const std::filesystem::path& filePath1, const std::filesystem::path& filePath2);
};

#endif //FILEMANAGER_H