

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
using namespace std;

class parameters {
private:
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;
    int generateAssignments;
    int riskAnalysis;
    string outputFileName;

public:
    int getMinReviwesPerSubmission() {return this->minReviewsPerSubmission;}
    int getMaxReviewsPerReviewer() {return this->maxReviewsPerReviewer;}
    int getGenerateAssigLevel() {return this->generateAssignments;}
    int getRiskAnalLevel() {return this->riskAnalysis;}
    string getOutputFilename() {return this->outputFileName;}
};
#endif //PARAMETERS_H
