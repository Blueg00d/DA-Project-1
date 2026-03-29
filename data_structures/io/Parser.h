#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "utils/Parameters.h"
#include "core/Reviewer.h"
#include "core/Submission.h"
#include "FileManager.h"


class Parser {
private:
    vector<Submission> submissions;
    vector<Reviewer> reviewers;
    Parameters params;

public:
    bool parseFile(const string &fileName);
    vector<Submission> getSubmissions() {return this->submissions;}
    vector<Reviewer> getReviewers() {return this->reviewers;}
    Parameters& getParams() {return this->params;}
    void testParser();
};

#endif //PARSER_H