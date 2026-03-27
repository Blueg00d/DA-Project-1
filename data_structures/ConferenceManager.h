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
    string filename;

    vector<MatchResult> matchResults;
    vector<MissingReviewsResult> missingReviewsResults;
    int success = -1;

    unordered_map<int, Reviewer*> nodesToReviewers;
    unordered_map<int, Submission*> nodesToSubmissions;

    Graph<int> graph;

    vector<int> riskyReviewers;

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

    void buildGraph();
    void debugGraph() const;
    void runAssignment();
    void debugGraphFLow() const;
    void interpretFlowResults();
    void debugInterpretationResults() const;
    void saveOutput(const string& folder);
    void runRiskAnalysis();
    void executeAllTasks(const string& folder);
    void setFilename(string filename);
    string getFilename();
    void setParams(Parameters params);
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H