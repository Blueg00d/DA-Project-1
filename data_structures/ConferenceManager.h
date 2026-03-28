#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "Reviewer.h"
#include "Submission.h"
#include "Parameters.h"
#include "Graph.h"
#include "MatchResult.h"
#include "MissingReviewsResult.h"
#include "FileManager.h"
#include <string>
#include <unordered_map>
#include <sstream>
using namespace std;
#define SOURCE 0
#define SINK 1
#define NOT_DEFINED (-1)

/**
 * @class ConferenceManager
 * @brief ConferenceManager gathers all info from inputs and puts them in a graph
 *
 */
class ConferenceManager {
    friend class Debugger;

    private:
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
     * @brief Function "restarts" graph and calls buildgraph
     */
    void runAssignment();

    /**
     * @brief Function maps the flows on edges given by the max flow into a sorted list
     */
    void interpretFlowResults();

    /**
     * @brief
     * @param folder
     */
    void saveOutput(const string& folder);

    /**
     * @brief
     */
    void runRiskAnalysis();

    /**
     * @brief
     * @param folder
     */
    void executeAllTasks(const string& folder);
    void setFilename(string filename);
    string getFilename();
    void setParams(Parameters params);
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H