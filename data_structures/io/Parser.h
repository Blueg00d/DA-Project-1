#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "utils/Parameters.h"
#include "models/Reviewer.h"
#include "models/Submission.h"
#include "FileManager.h"

/**
 * @class Parser
 * @brief Responsible for parsing input dataset files and extracting configurations, submissions, and reviewers.
 */
class Parser {
private:
    vector<Submission> submissions;
    vector<Reviewer> reviewers;
    Parameters params;

public:
    /**
     * @brief Parses the specified input file.
     * @param fileName The path to the dataset file to be parsed.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseFile(const string &fileName);

    /**
     * @brief Gets the parsed list of submissions.
     * @return A const reference to the vector of parsed Submission objects.
     */
    const vector<Submission>& getSubmissions() const {return this->submissions;}

    /**
     * @brief Gets the parsed list of reviewers.
     * @return A const reference to the vector of parsed Reviewer objects.
     */
    const vector<Reviewer>& getReviewers() const {return this->reviewers;}

    /**
     * @brief Gets the execution parameters parsed from the file.
     * @return A reference to the parsed Parameters object.
     */
    Parameters& getParams() {return this->params;}

    /**
     * @brief Gets a read-only reference to the execution parameters.
     * @return A const reference to the parsed Parameters object.
     */
    const Parameters& getParams() const {return this->params;}

    /**
     * @brief Utility function to test if the parsing functionality works as expected.
     */
    void testParser();
};

#endif //PARSER_H