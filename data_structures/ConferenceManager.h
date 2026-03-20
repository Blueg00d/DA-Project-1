#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "Reviewer.h"
#include "Submission.h"
#include "Parameters.h"
#include "Graph.h"
#include "MatchResult.h"
#include "MissingReviewsResult.h"
#include <string>
#include <unordered_map>
#include <sstream>
using namespace std;

class ConferenceManager {
    private:
    //Private Attributes
    vector<Reviewer> reviewers;
    vector<Submission> submissions;
    Parameters params;

    vector<MatchResult> matchResults;
    vector<MissingReviewsResult> missingReviewsResults;
    int success = -1;

    unordered_map<int, Reviewer*> nodesToReviewers;
    unordered_map<int, Submission*> nodesToSubmissions;

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
    ConferenceManager() = default;
    ~ConferenceManager() = default;

    bool loadFromCSV(string filename);
    void buildGraph();
    void debugGraph() const;
    void runAssignment();
    void debugGraphFLow() const;
    void interpretFlowResults();
    void debugInterpretationResults() const;
    void saveOutput();
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H