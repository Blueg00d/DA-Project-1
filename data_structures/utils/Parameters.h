#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
using namespace std;

/**
 * @class Parameters
 * @brief Holds the parsed configuration parameters for governing the assignment and analysis process.
 */
class Parameters {
private:
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;
    int generateAssignments;
    int riskAnalysis;
    string outputFileName;

public:

    /** @brief Sets the minimum number of reviews required per submission. */
    void setMinReviewsPerSubmission(int min) {this->minReviewsPerSubmission = min;}
    /** @brief Sets the maximum number of reviews a single reviewer can take. */
    void setMaxReviewsPerReviewer(int max) {this->maxReviewsPerReviewer = max;}
    /** @brief Sets the generation depth/level for computing assignments. */
    void setGenerateAssigLevel(int level) {this->generateAssignments = level;}
    /** @brief Sets the risk analysis computation level. */
    void setRiskAnalLevel(int level) {this->riskAnalysis = level;}
    /** @brief Sets the target filename for saving exported results. */
    void setOutputFilename(string filename) {this->outputFileName = filename;}

    /** @brief Gets the minimum number of reviews required per submission. */
    int getMinReviewsPerSubmission() {return this->minReviewsPerSubmission;}
    /** @brief Gets the maximum number of reviews a single reviewer can take. */
    int getMaxReviewsPerReviewer() {return this->maxReviewsPerReviewer;}
    /** @brief Gets the assignment generation depth/level. */
    int getGenerateAssigLevel() {return this->generateAssignments;}
    /** @brief Gets the risk analysis computation level. */
    int getRiskAnalLevel() const {return this->riskAnalysis;}
    /** @brief Gets the target filename for exported results. */
    string getOutputFilename() {return this->outputFileName;}
};
#endif //PARAMETERS_H