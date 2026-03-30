#ifndef DA_PROJECT_1_CONFERENCEMANAGER_H
#define DA_PROJECT_1_CONFERENCEMANAGER_H
#include "../models/Reviewer.h"
#include "../models/Submission.h"
#include "../utils/Parameters.h"
#include "../structures/Graph.h"
#include "../models/MatchResult.h"
#include "../models/MissingReviewsResult.h"
#include "../io/FileManager.h"
#include <string>
#include <unordered_map>
#include <sstream>
using namespace std;


/**
 * @class Brainer
 * @brief Brainer gathers all info from inputs and puts them in a graph
 *
 */
class Brainer {
    friend class Debugger;

    private:
    /** @brief List of reviewers available for the conference. */
    vector<Reviewer> reviewers;

    /** @brief List of submissions made to the conference. */
    vector<Submission> submissions;

    /** @brief Execution parameters determining assignment rules. */
    Parameters params;

    /** @brief Name of the input file currently being processed. */
    string filename;

    /** @brief Stores the resulting matches between reviewers and submissions. */
    vector<MatchResult> matchResults;

    /** @brief Stores submissions that did not meet the minimum required reviews. */
    vector<MissingReviewsResult> missingReviewsResults;

    /** @brief Flag or counter indicating the success status of the operations. */
    int success = -1;

    /** @brief Maps a graph node ID to a specific Reviewer object. */
    unordered_map<int, Reviewer*> nodesToReviewers;

    /** @brief Maps a graph node ID to a specific Submission object. */
    unordered_map<int, Submission*> nodesToSubmissions;

    /** @brief The flow network graph representing the assignment problem. */
    Graph<int> graph;

    /** @brief List of reviewer IDs identified during the risk analysis. */
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
     * @brief Constructor of class Brainer
     *
     * @param reviewers a vector of Reviewers from input
     * @param submissions a vector of Submissions from input
     * @param params Parameters from input
     */
    Brainer(
        const vector<Reviewer>& reviewers,
        const vector<Submission>& submissions,
        const Parameters& params
    );

    /**
     * @brief Default constructor of Brainer
     */
    Brainer() = default;

    /**
     * @brief Destructor of class Brainer
     */
    ~Brainer() = default;


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
     * @brief Saves the interpreted assignment and missing reviews results to a file.
     * @param folder The target directory/folder where the output file should be saved.
     */
    void saveOutput(const string& folder);

    /**
     * @brief Analyzes the assignments to identify risky reviewers or submissions.
     */
    void runRiskAnalysis();

    /**
     * @brief Executes the entire pipeline of tasks (building graph, running assignments, and saving) for a given input.
     * @param folder The base folder to read inputs from and write generated outputs to.
     */
    void executeAllTasks(const string& folder);

    /**
     * @brief Sets the filename currently being processed.
     * @param filename The name of the file.
     */
    void setFilename(string filename);

    /**
     * @brief Gets the current filename.
     * @return The name of the file.
     */
    string getFilename();

    /**
     * @brief Sets the execution parameters for the assignment process.
     * @param params The Parameters object containing the configurations.
     */
    void setParams(Parameters params);
};


#endif //DA_PROJECT_1_CONFERENCEMANAGER_H