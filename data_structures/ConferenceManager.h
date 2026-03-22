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

/**
 * @class ConferenceManager
 * @brief ConferenceManager gathers all info from inputs and puts them in a graph
 *
 */
class ConferenceManager {
    private:
    vector<Reviewer> reviewers;
    vector<Submission> submissions;
    Parameters params;

    vector<MatchResult> matchResults;
    vector<MissingReviewsResult> missingReviewsResults;
    int success = -1;

    unordered_map<int, Reviewer*> nodesToReviewers;
    unordered_map<int, Submission*> nodesToSubmissions;

    Graph<int> graph;

    vector<int> riskyReviewers;

    /**
     * @brief places information about Reviewers and Submissions in nodes
     *
     */
    void createNodes();

    /**
     *@brief Connects Source to Reviewers and Submissions to Sink
     */
    void connectSourceSinkToNodes();

    /**
     *@brief connects Reviewers and Submissions
     *
     * Depending on the Assignment Level indicated in the input
     */
    void connectNodes();


    public:
    /**
     * @brief Constructor of class ConferenceManager
     *
     * @param reviewers a vector of Reviewers from input
     * @param submissions a vector of Submissions from input
     * @param params Parameters from input
     */
    ConferenceManager(
        vector<Reviewer> reviewers,
        vector<Submission> submissions,
        Parameters params
    );

    /**
     * @brief Default constructor of ConferenceManager
     */
    ConferenceManager() = default;

    /**
     * @brief Destructor of class ConferenceManager
     */
    ~ConferenceManager() = default;


    /**
     * @brief Function builds graph with information from input
     *
    * Calls createNodes, connectSourceSinkToNodes and connectNodes.
     */
    void buildGraph();

    /**
     * @brief Function prints graph for debug purposes
     */
    void debugGraph() const;

    /**
     * @brief Function "restarts" graph and calls buildgraph
     */
    void runAssignment();
    void debugGraphFLow() const;
    void interpretFlowResults();
    void debugInterpretationResults() const;
    void saveOutput(const string& folder);
    void runRiskAnalysis();
    void executeAllTasks(const string& folder);
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H