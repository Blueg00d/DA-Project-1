

#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Parameters.h"
#include "Reviewer.h"
#include "Submission.h"
class parser {
    public:
    bool parseFile( const std::string fileName);
    vector<Submission> submissions;
    vector<Reviewer> reviewers;
    Parameters params;
};

#endif //PARSER_H
