#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "Reviewer.h"
#include "Submission.h"
#include <string>
#include <unordered_map>
using namespace std;

class ConferenceManager {
    private:
    unordered_map<int, Reviewer> reviewers;
    unordered_map<int, Submission> submissions;
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer; // Acho que fica melhor assim, sara vê pls

    void createNodes();

    public:
    bool loadFromCSV(string filename);
    void buildGraph();
    void runAssignment();
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H