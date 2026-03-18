#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "Reviewers.h"
#include "Submissions.h"
#include <string>
#include <unordered_map>
using namespace std;

class ConferenceManager {
    private:
        unordered_map<int, Reviewer> reviewers;
        unordered_map<int, Submission> submissions;
        int minReviews;
        int maxReviews; //não sei se é assim q queres ou com minSubmission gui vê

    public:
    bool loadFromCSV(string filename);
    void buildGraph();
    void runAssignment();
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H