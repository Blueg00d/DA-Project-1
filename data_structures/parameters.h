

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
using namespace std;

class parameters {
public:
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;
    int generateAssignments;
    int riskAnalysis;
    string outputFileName;

};
#endif //PARAMETERS_H
