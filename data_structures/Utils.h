
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <sstream>
using namespace std;
/** @brief Function: split.
 *split so we can split the lines where there are commas
 *
 *@param const string & line (the line being read)
 *@return vector<string> result (with all parts of line separated)
 */
inline vector<string> split(const string & line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}
/** @brief Function: trim
 *to 'trim' poorly written inputs (extra spaces etc.)
 *
 *@param const string & line (the line being read)
 *@return string line with no leading or useless whitespace
 */
inline string trim(const string & line) {
    // Add spaces ' ', \r, and \n to the list of characters to remove
    size_t start = line.find_first_not_of(" \t\r\n\"");
    size_t end = line.find_last_not_of(" \t\r\n\"");
    return (start == string::npos) ? "" : line.substr(start, end - start + 1);
}

#endif //UTILS_H
