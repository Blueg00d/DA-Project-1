
#ifndef UTILS_H
#define UTILS_H

// ANSI Prefix
#define ANSI_ESC "\033["

// --- TEXT STYLES ---
#define TXT_RESET      ANSI_ESC "0m"
#define TXT_BOLD       ANSI_ESC "1m"
#define TXT_DIM        ANSI_ESC "2m"
#define TXT_ITALIC     ANSI_ESC "3m"
#define TXT_UNDERLINE  ANSI_ESC "4m"
#define TXT_BLINK      ANSI_ESC "5m"
#define TXT_INVERT     ANSI_ESC "7m"
#define TXT_HIDDEN     ANSI_ESC "8m"
#define TXT_STRIKE     ANSI_ESC "9m"

// --- TEXT BASIC COLORS ---
#define FG_BLACK       ANSI_ESC "30m"
#define FG_RED         ANSI_ESC "31m"
#define FG_GREEN       ANSI_ESC "32m"
#define FG_YELLOW      ANSI_ESC "33m"
#define FG_BLUE        ANSI_ESC "34m"
#define FG_MAGENTA     ANSI_ESC "35m"
#define FG_CYAN        ANSI_ESC "36m"
#define FG_WHITE       ANSI_ESC "37m"
#define FG_DEFAULT     ANSI_ESC "39m"

// --- BACKGROUND BASIC COLORS ---
#define BG_BLACK       ANSI_ESC "40m"
#define BG_RED         ANSI_ESC "41m"
#define BG_GREEN       ANSI_ESC "42m"
#define BG_YELLOW      ANSI_ESC "43m"
#define BG_BLUE        ANSI_ESC "44m"
#define BG_MAGENTA     ANSI_ESC "45m"
#define BG_CYAN        ANSI_ESC "46m"
#define BG_WHITE       ANSI_ESC "47m"
#define BG_DEFAULT     ANSI_ESC "49m"

// --- SCREEN AND CURSOR CONTROLS ---
#define CLR_SCREEN     ANSI_ESC "2J"
#define CLR_LINE       ANSI_ESC "K"
#define CUR_HOME       ANSI_ESC "H"
#define CUR_UP(n)      ANSI_ESC #n "A"  // MOVES THE CURSOR N LINES UP


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
 *to 'trim' poorly written inputs (extra spaces, tabs, \r, \n)
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
