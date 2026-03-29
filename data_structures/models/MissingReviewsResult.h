//
// Created by bluegood on 3/20/26.
//

#ifndef MYPROJECT_MISSINGREVIEWSRESULT_H
#define MYPROJECT_MISSINGREVIEWSRESULT_H

#include "core/Submission.h"
#include <string>
using namespace std;

class MissingReviewsResult {
private:
    int submissionID;
    int primaryDomain;
    int missingReviewsNumber;
public:
    bool operator<(const MissingReviewsResult& r) const {
        if (submissionID != r.submissionID) return submissionID < r.submissionID;
        else if (primaryDomain != r.primaryDomain) return primaryDomain < r.primaryDomain;
        else return missingReviewsNumber < r.missingReviewsNumber;
    };
    MissingReviewsResult(int submissionID, int primaryDomain, int missingReviewsNumber);
    MissingReviewsResult(const Submission &submission, int missingReviewsNumber);

    int getSubmissionID() const;
    int getPrimaryDomain() const;
    int getMissingReviewsNumber() const;

    string toStringMissingReviewsResult() const;
};

#endif //MYPROJECT_MISSINGREVIEWSRESULT_H