#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "Reviewer.h"
#include "Submission.h"
#include "Parameters.h"
#include "Graph.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class ConferenceManager {
    private:
    //Private Attributes
    vector<Reviewer> reviewers;
    vector<Submission> submissions;
    Parameters params;

    unordered_map<int, Reviewer> nodesToReviewers;
    unordered_map<int, Submission> nodesToSubmissions;

    Graph<int> graph;

    //Private Methods
    void createNodes();
    void connectSourceSinkToNodes();
    void connectNodes();


    public:
    // Constructor
    ConferenceManager(
        vector<Reviewer> reviewers,
        vector<Submission> submissions,
        Parameters params
    );
    ConferenceManager();

    bool loadFromCSV(string filename);
    void buildGraph();
    void debugGraph();
    void runAssignment();
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H