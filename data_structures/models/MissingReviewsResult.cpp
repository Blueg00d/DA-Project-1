//
// Created by bluegood on 3/20/26.
//

#include "MissingReviewsResult.h"
#include <sstream>
using namespace std;

MissingReviewsResult::MissingReviewsResult(const Submission &submission, int missingReviewsNumber) {
    this->submissionID = submission.getId();
    this->primaryDomain = submission.getPrimary();
    this->missingReviewsNumber = missingReviewsNumber;
}

MissingReviewsResult::MissingReviewsResult(int submissionID, int primaryDomain, int missingReviewsNumber) {
    this->submissionID = submissionID;
    this->primaryDomain = primaryDomain;
    this->missingReviewsNumber = missingReviewsNumber;
}

int MissingReviewsResult::getSubmissionID() const {
    return this->submissionID;
}

int MissingReviewsResult::getPrimaryDomain() const {
    return this->primaryDomain;
}

int MissingReviewsResult::getMissingReviewsNumber() const {
    return this->missingReviewsNumber;
}

string MissingReviewsResult::toStringMissingReviewsResult() const {
    stringstream ss;
    ss << this->submissionID << ", " << this->primaryDomain << ", " << this->missingReviewsNumber;
    return ss.str();
}
