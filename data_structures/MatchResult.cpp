//
// Created by bluegood on 3/20/26.
//

#include "MatchResult.h"
#include <sstream>
using namespace std;

MatchResult::MatchResult(int reviewerID, int submissionID, int match) {
    this->reviewerID = reviewerID;
    this->submissionID = submissionID;
    this->match = match;
}

int MatchResult::getReviewerID() const {
    return reviewerID;
}

int MatchResult::getSubmissionID() const {
    return submissionID;
}

int MatchResult::getMatch() const {
    return match;
}

string MatchResult::toStringRevSubMatch() const {
    stringstream ss;
    ss << reviewerID << ", " << submissionID << ", " << match;
    return ss.str();
}

string MatchResult::toStringSubRevMatch() const {
    stringstream ss;
    ss << submissionID << ", " << reviewerID << ", " << match;
    return ss.str();
}


