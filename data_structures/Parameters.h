#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
using namespace std;

class Parameters {
private:
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;
    int generateAssignments;
    int riskAnalysis;
    string outputFileName;

public:

    void setMinReviewsPerSubmission(int min) {this->minReviewsPerSubmission = min;}
    void setMaxReviewsPerReviewer(int max) {this->maxReviewsPerReviewer = max;}
    void setGenerateAssigLevel(int level) {this->generateAssignments = level;}
    void setRiskAnalLevel(int level) {this->riskAnalysis = level;}
    void setOutputFilename(string filename) {this->outputFileName = filename;}

    int getMinReviewsPerSubmission() {return this->minReviewsPerSubmission;}
    int getMaxReviewsPerReviewer() {return this->maxReviewsPerReviewer;}
    int getGenerateAssigLevel() {return this->generateAssignments;}
    int getRiskAnalLevel() const {return this->riskAnalysis;}
    string getOutputFilename() {return this->outputFileName;}
};
#endif //PARAMETERS_H
