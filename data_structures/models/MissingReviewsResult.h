//
// Created by bluegood on 3/20/26.
//

#ifndef MYPROJECT_MISSINGREVIEWSRESULT_H
#define MYPROJECT_MISSINGREVIEWSRESULT_H

#include "models/Submission.h"
#include <string>
using namespace std;

/**
 * @class MissingReviewsResult
 * @brief Stores information about a submission that did not receive the required minimum number of reviews.
 */
class MissingReviewsResult {
private:
    int submissionID;
    int primaryDomain;
    int missingReviewsNumber;
public:
    /**
     * @brief Overloads the less-than operator for sorting MissingReviewsResults.
     * @param r The other MissingReviewsResult to compare against.
     * @return True if this object should precede the other object.
     */
    bool operator<(const MissingReviewsResult& r) const {
        if (submissionID != r.submissionID) return submissionID < r.submissionID;
        else if (primaryDomain != r.primaryDomain) return primaryDomain < r.primaryDomain;
        else return missingReviewsNumber < r.missingReviewsNumber;
    };

    /**
     * @brief Constructor for MissingReviewsResult using IDs directly.
     * @param submissionID The ID of the submission.
     * @param primaryDomain The primary domain ID of the submission.
     * @param missingReviewsNumber The count of reviews missing to meet the required threshold.
     */
    MissingReviewsResult(int submissionID, int primaryDomain, int missingReviewsNumber);

    /**
     * @brief Constructor for MissingReviewsResult using a Submission object.
     * @param submission The Submission object missing reviews.
     * @param missingReviewsNumber The count of reviews missing to meet the required threshold.
     */
    MissingReviewsResult(const Submission &submission, int missingReviewsNumber);

    /** @brief Gets the submission ID. */
    int getSubmissionID() const;
    /** @brief Gets the primary domain ID. */
    int getPrimaryDomain() const;
    /** @brief Gets the number of missing reviews. */
    int getMissingReviewsNumber() const;

    /**
     * @brief Returns a formatted string representation of the missing reviews data.
     * @return The formatted string.
     */
    string toStringMissingReviewsResult() const;
};

#endif //MYPROJECT_MISSINGREVIEWSRESULT_H