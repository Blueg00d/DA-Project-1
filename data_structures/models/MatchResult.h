//
// Created by bluegood on 3/20/26.
//

#ifndef MYPROJECT_RESULT_H
#define MYPROJECT_RESULT_H

#include <string>
using namespace std;

class MatchResult {
private:
    int reviewerID;
    int submissionID;
    int match;
public:
    bool operator<(const MatchResult& r) const {
        if (reviewerID != r.reviewerID) return reviewerID < r.reviewerID;
        else if (submissionID != r.submissionID) return submissionID < r.submissionID;
        else return match < r.match;
    };
    MatchResult(int reviewerID, int submissionID, int match);

    int getReviewerID() const;
    int getSubmissionID() const;
    int getMatch() const;

    string toStringRevSubMatch() const;
    string toStringSubRevMatch() const;
};

#endif //MYPROJECT_RESULT_H