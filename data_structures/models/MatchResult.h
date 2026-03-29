//
// Created by bluegood on 3/20/26.
//

#ifndef MYPROJECT_RESULT_H
#define MYPROJECT_RESULT_H

#include <string>
using namespace std;

/**
 * @class MatchResult
 * @brief Represents the result of an assignment match between a reviewer and a submission.
 */
class MatchResult {
private:
    int reviewerID;
    int submissionID;
    int match;
public:
    /**
     * @brief Overloads the less-than operator for sorting MatchResults.
     * @param r The other MatchResult to compare against.
     * @return True if this object should precede the other object, false otherwise.
     */
    bool operator<(const MatchResult& r) const {
        if (reviewerID != r.reviewerID) return reviewerID < r.reviewerID;
        else if (submissionID != r.submissionID) return submissionID < r.submissionID;
        else return match < r.match;
    };

    /**
     * @brief Constructor for MatchResult.
     * @param reviewerID The ID of the assigned reviewer.
     * @param submissionID The ID of the assigned submission.
     * @param match The matching weight/score.
     */
    MatchResult(int reviewerID, int submissionID, int match);

    /**
     * @brief Gets the reviewer ID.
     * @return The ID of the reviewer.
     */
    int getReviewerID() const;

    /**
     * @brief Gets the submission ID.
     * @return The ID of the submission.
     */
    int getSubmissionID() const;

    /**
     * @brief Gets the matching weight/score.
     * @return The match value.
     */
    int getMatch() const;

    /**
     * @brief Returns a string representation formatted as Reviewer-Submission-Match.
     * @return The formatted string.
     */
    string toStringRevSubMatch() const;

    /**
     * @brief Returns a string representation formatted as Submission-Reviewer-Match.
     * @return The formatted string.
     */
    string toStringSubRevMatch() const;
};

#endif //MYPROJECT_RESULT_H