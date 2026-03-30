#include "Brainer.h"
#include <fstream>
using namespace std;

#include "utils/Utils.h"

#include <utility>

/**
 * @copybrief Brainer::Brainer
 *
 * Time complexity: O(R+S)
 * Performs a deep copy of the Reviewers and Submissions vectors.
 *
 * @param reviewers a vector of Reviewers from input
 * @param submissions a vector of Submissions from input
 * @param params Parameters from input
 */
Brainer::Brainer(
        const vector<Reviewer>& reviewers,
        const vector<Submission>& submissions,
        const Parameters& params
    )
{
        this->reviewers = reviewers;
        this->submissions = submissions;
        this->params = params;
}

/**
 * @copybrief setFilename
 *
 * Time complexity: O(1)
 *
 * @param filename The name of the file.
 */
void Brainer::setFilename(string filename) {
    this->filename = filename;
}

/**
 * @copybrief getFilename
 *
 * Time complexity: O(1)
 *
 * @return The name of the file.
 */
string Brainer::getFilename() {
    return this->filename;
}

/**
 * @copybrief setParams
 *
 * Time complexity: O(1)
 *
 * @param params The Parameters object containing the configurations.
 */
void Brainer::setParams(Parameters params) {
    this->params = params;
}

/**
 * @copybrief createNodes
 *
 * Makes source have default value of 0 and sink have default value of 1
 *
 * Time complexity: O(R+S)
 * Iterates over all Reviewers (R) and all Submissions (S) and adds them to an unordered map
 */
void Brainer::createNodes() {
    this->nodesToReviewers.clear();
    this->nodesToSubmissions.clear();
    this->graph.addVertex(SOURCE); // Represents Source
    this->graph.addVertex(SINK); // Represents Sink

    for (int i = 2; i < 2 + this->reviewers.size() + this->submissions.size(); i++) {
        this->graph.addVertex(i);

        if (i - 2 < this->reviewers.size()) {
            // Reviewers' Nodes
            this->nodesToReviewers.emplace(i, &this->reviewers[i - 2]);
        }
        else {
            // Submissions' Nodes
            this->nodesToSubmissions.emplace(i, &this->submissions[i - 2 - this->reviewers.size()]);
        }
    }
}

/**
 * @copybrief connectSourceSinkToNodes
 *
 * Time complexity: O(R log R + S)
 * Sorts reviewer nodes in O(R log R) to ensure the smallest ID is processed first,
 * then connects Reviewers (R) to Source and Submissions (S) to Sink.
 */
void Brainer::connectSourceSinkToNodes() {
    //process so that the reviser with the smallest ID is processed first
    vector<int> sortedNodeIDs;
    for (auto const& [nodeID, rev] : nodesToReviewers) {
        sortedNodeIDs.push_back(nodeID);
    }

    sort(sortedNodeIDs.begin(), sortedNodeIDs.end(), [&](int a, int b) {
        return nodesToReviewers[a]->getId() < nodesToReviewers[b]->getId();
    });

    for (int nodeID : sortedNodeIDs) {
        this->graph.addEdge(SOURCE, nodeID, this->params.getMaxReviewsPerReviewer());
    }

    for (pair<int, Submission*>p: this->nodesToSubmissions) {
        this->graph.addEdge(p.first, SINK, this->params.getMinReviewsPerSubmission());
    }
}

/**
 * @copybrief connectNodes
 *
 * Depending on the GenerateAssignments parameter the program receives, it considers different levels of expertise
 *
 * Time complexity: O(R*S)
 * Cases 1, 2 and 3 have nested for loops where the outer loop iterates through all Reviewers (R) and the inner loop through all Submissions (S)
 */
void Brainer::connectNodes() {
    int level = this->params.getGenerateAssigLevel();
    //Order reviewers by nodeID
   vector<pair<int, Reviewer*>> sortedReviewers(nodesToReviewers.begin(), nodesToReviewers.end());
    sort(sortedReviewers.begin(), sortedReviewers.end(), [](const auto& a, const auto& b) {
        return a.second->getId() < b.second->getId();
    });

    //Order submissions by nodeID
    vector<pair<int, Submission*>> sortedSubmissions(nodesToSubmissions.begin(), nodesToSubmissions.end());
    sort(sortedSubmissions.begin(), sortedSubmissions.end(), [](const auto& a, const auto& b) {
        return a.second->getId() < b.second->getId();
    });
    for (auto& reviewer : sortedReviewers) {
        for (auto& submission : sortedSubmissions) {

            bool isEligible = false;
            int rPrim = reviewer.second->getPrimary();
            int rSec = reviewer.second->getSecondary();
            int sPrim = submission.second->getPrimary();
            int sSec = submission.second->getSecondary();

            if (level == 0 || level == 1) {
                isEligible = (rPrim == sPrim);
            } else if (level == 2) {
                isEligible = (rPrim == sPrim) || (sSec != NOT_DEFINED && rPrim == sSec);
            } else if (level == 3) {
                isEligible = (rPrim == sPrim) ||
                             (sSec != NOT_DEFINED && rPrim == sSec) ||
                             (rSec != NOT_DEFINED && rSec == sPrim) ||
                             (rSec != NOT_DEFINED && sSec != NOT_DEFINED && rSec == sSec);
            }

            if (isEligible) {
                this->graph.addEdge(reviewer.first, submission.first, 1);
            }
        }
    }
}

/**
 * @copybrief buildGraph
 *
 * Time complexity: O(R*S)
 * Biggest time complexity out of the three functions called
 */
void Brainer::buildGraph() {
    createNodes();
    connectSourceSinkToNodes();
    connectNodes();
}


/**
 * @copybrief runAssignment
 *
 * Time complexity: O(E * f_max)
 * Calls the Ford-Fulkerson algorithm implemented with DFS, where E is the number of edges
 * and f_max is the maximum possible flow in the network.
 */
void Brainer::runAssignment() {
    //Erase previous graph so we can start over
    this->graph = Graph<int>();
    //Build the new graph with new data
    buildGraph();
    graph.fordFulkerson(SOURCE,SINK);
}


/**
 * @copybrief interpretFlowResults
 * Time complexity: O((R*S)+ElogE)
 * First it iterates through all submissions S in a nested loop with the reviewers R O(R*S)
 * Sorting results takes O(ElogE) for edges E
 */
void Brainer::interpretFlowResults() {
    //Initialization
    this->matchResults.clear();
    this->missingReviewsResults.clear();

    //Iterate through all submissions
    for (const pair<int, Submission*> p: this->nodesToSubmissions) {
        int reviewsExecuted = 0;
        Submission* s = p.second;

        auto vertex = this->graph.findVertex(p.first);
        if (!vertex) continue; // Safety check if graph hasn't been built yet

        for (Edge<int>* e: vertex->getIncoming()) {
            if (e->getFlow() > 0) {
                reviewsExecuted++;
                Reviewer* r = this->nodesToReviewers.at(e->getOrig()->getInfo());
                //Determine which area is matched
                int match;
                if (r->getPrimary() == s->getPrimary() || r->getPrimary() == s->getSecondary()) match = r->getPrimary();
                else match = r->getSecondary();
                this->matchResults.emplace_back(
                        r->getId(),
                        s->getId(),
                        match
                    );
            }
        }
        //Check if enough reviews were received
        int minReviewsPerSub = this->params.getMinReviewsPerSubmission();
        if (reviewsExecuted < minReviewsPerSub) {
            this->missingReviewsResults.emplace_back(
                            s->getId(),
                            s->getPrimary(),
                            minReviewsPerSub - reviewsExecuted
                            );
        }
    }
    //If it was successful or not (Unused independent flow accumulator removed for reliability)
    this->success = (this->matchResults.size() >= this->params.getMinReviewsPerSubmission() * this->nodesToSubmissions.size());

    //Sort results
    sort(matchResults.begin(), matchResults.end());
    sort(missingReviewsResults.begin(), missingReviewsResults.end());
}


/**
 * @copybrief runRiskAnalysis
 * Time Complexity: O(R * E * f_max)
 * Reruns Ford-Fulkerson once per reviewer R, with each taking O(E * f_max).
 * We used an alternative Max-Flow approach called incremental isolation,
 * where we temporarily set the capacity of the edge from the source to the reviewer node to 0,
 * effectively isolating that reviewer from the flow network.
 * After running Ford-Fulkerson, we check if the total flow is still sufficient to meet the required number of reviews.
 * If not, we consider that reviewer risky and add their ID to the riskyReviewers vector.
 * Finally, we restore the original capacity of the edge to ensure the graph is back to its initial state for the next iteration.
 * Running Ford-Fulkerson with the isolated reviewer is more efficient than rebuilding the entire graph for each reviewer and running the algorithm on a fresh graph with no flow,
 * as it allows us to reuse the existing flow and only adjust the necessary part of the graph for each iteration.
 *
 * @details This runRiskAnalysis works only for level k == 1.
 * If we wanted to run this function for a level k > 1, we could use a brute-force approach.
 *
 * The algorithm would be analogous to this one but
 * instead of pushing a single reviewerID to the reviewerNodes vector,
 * we would create every possible subset of discarded reviewers of size <= k,
 * resulting in \f$\sum_{i=1}^{k} C_{i}^{R}\f$ subsets.
 * Then, for each subset of reviewers, we would set each reviewers flow to 0,
 * similar to what this algorithm does.
 *
 * This algorithm would result in a temporal complexity of O(R^k * E * f_max),
 * characterized by running the Ford-Fulkerson algorithm through every subset of
 * discarded reviewers.
 */
void Brainer::runRiskAnalysis() {
    int M = params.getRiskAnalLevel();
    if (M == 0) return;

    vector<int> reviewerNodes;
    for (auto const& [nodeID, rev] : nodesToReviewers) {
        reviewerNodes.push_back(nodeID);
    }

    double requiredFlow = submissions.size() * params.getMinReviewsPerSubmission();
    this->riskyReviewers.clear();

    Vertex<int>* vSource = graph.findVertex(SOURCE);
    Vertex<int>* vSink = graph.findVertex(SINK);
    if (!vSource || !vSink) return;

    // Establishing the baseline max flow (O(E * f_max) - done only once!)
    double baselineFlow = graph.fordFulkerson(SOURCE, SINK);

    // Caching the baseline flow state to instantly restore it in O(E) time
    vector<double> savedFlows;
    for (auto v : graph.getVertexSet()) {
        for (auto e : v->getAdj()) {
            savedFlows.push_back(e->getFlow());
        }
    }

    for (int revNodeID : reviewerNodes) {
        Edge<int>* targetEdge = nullptr;
        double originalWeight = 0;
        double flowToRev = 0;

        for (auto e : vSource->getAdj()) {
            if (e->getDest()->getInfo() == revNodeID) {
                targetEdge = e;
                originalWeight = e->getWeight();
                flowToRev = e->getFlow();
                break;
            }
        }

        if (!targetEdge) continue;

        // Temporarily isolating the reviewer
        targetEdge->setWeight(0);
        targetEdge->setFlow(0);

        // Tracing and cancelling the flow that went exclusively through this Reviewer
        Vertex<int>* revVertex = graph.findVertex(revNodeID);
        for (auto e : revVertex->getAdj()) {
            double f = e->getFlow();
            if (f > 0) {
                e->setFlow(0); // Cancel flow from Reviewer -> Submission

                // Cancel the corresponding flow from Submission -> Sink
                Vertex<int>* subVertex = e->getDest();
                for (auto subEdge : subVertex->getAdj()) {
                    if (subEdge->getDest()->getInfo() == SINK) {
                        subEdge->setFlow(subEdge->getFlow() - f);
                        break;
                    }
                }
            }
        }

        // Running the incremental flow (only routing the cancelled flow, if possible)
        double currentFlow = baselineFlow - flowToRev;
        double flowAfter = graph.resumeFordFulkerson(SOURCE, SINK, currentFlow);

        if (flowAfter < requiredFlow) {
            this->riskyReviewers.push_back(nodesToReviewers[revNodeID]->getId());
        }

        // Restoring the reviewer capacity and the exact baseline flows
        targetEdge->setWeight(originalWeight);

        int flowIdx = 0;
        for (auto v : graph.getVertexSet()) {
            for (auto e : v->getAdj()) {
                e->setFlow(savedFlows[flowIdx++]);
            }
        }
    }

    sort(riskyReviewers.begin(), riskyReviewers.end());
}

/**
 * @copybrief saveOutput
 * Time complexity: O(MlogM + R + S)
 * - M = number of matches (matchResults.size())
 * - R = number of risky reviewers
 * - S = number of missing review objects (up to total submissions)
 * Sorts results twice (grouped by submission and then grouped by reviewer) O(MlogM)
 * Writes matches, missing reviews and risky reviewers to output O(M + R + S)
 */
void Brainer::saveOutput(const string& path) {
    if (matchResults.empty() && missingReviewsResults.empty()) {
        cout << CLR_ALL << FG_RED << TXT_INVERT << "No results to save! Please run the assignment and interpretation first." << endl;
        cout << TXT_RESET;
        return;
    }
    string filename;

    //If path ends in .csv it's batch mode time
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".csv") {
        filename = path;
    }
    //else normal mode
    else {
        filename = path + params.getOutputFilename();
    }

    //if the parser couldn't read the filename, then we need to use a default one
    if (filename.empty()) { filename = "assignment.csv"; }

    vector<string> lines;

    //write in the output file just like it prints on the terminal
    lines.push_back("#SubmissionId,ReviewerId,Match");

    // Sort logic for Match Results
    sort(matchResults.begin(), matchResults.end(), [] (MatchResult& a, MatchResult& b) {
        if (a.getSubmissionID() != b.getSubmissionID()) return a.getSubmissionID() < b.getSubmissionID();
        else if (a.getReviewerID() != b.getReviewerID()) return a.getReviewerID() < b.getReviewerID();
        else return a.getMatch() < b.getMatch();
    });
    for (const MatchResult& ms: this->matchResults) {
        lines.push_back(ms.toStringSubRevMatch());
    }

    lines.push_back("#ReviewerId,SubmissionId,Match");
    sort(matchResults.begin(), matchResults.end(), [] (MatchResult& a, MatchResult& b) {
        if (a.getReviewerID() != b.getReviewerID()) return a.getReviewerID() < b.getReviewerID();
        else if (a.getSubmissionID() != b.getSubmissionID()) return a.getSubmissionID() < b.getSubmissionID();
        else return a.getMatch() < b.getMatch();
    });
    for (const MatchResult& ms: this->matchResults) {
        lines.push_back(ms.toStringRevSubMatch());
    }

    lines.push_back("#Total: " + to_string(this->matchResults.size()));

    if (!this->missingReviewsResults.empty()) {
        lines.push_back("#SubmissionId,Domain,MissingReviews");
        for (const MissingReviewsResult& ms: this->missingReviewsResults) {
            lines.push_back(ms.toStringMissingReviewsResult());
        }
    }

    if (params.getRiskAnalLevel() > 0) {
        lines.push_back("#Risk Analysis: " + to_string(params.getRiskAnalLevel()));

        string riskLine = "";
        for (size_t i = 0; i < this->riskyReviewers.size(); i++) {
            riskLine += to_string(this->riskyReviewers[i]) + (i == riskyReviewers.size() - 1 ? "" : ", ");
        }
        lines.push_back(riskLine);
    }

    // Use the FileManager to write all generated lines at once
    FileManager::writeLines(filename, lines);
}

/**
 * @copydoc executeAllTasks
 * Time complexity: O(R * E * f_max + MlogM + S)
 * buildGraph: O(R*S)
 * runAssignment: O(E * f_max)
 * interpretFlowResults: O((R*S)+ElogE)
 * runRiskAnalysis: O(R * E * f_max)
 * saveOutput: O(MlogM + R + S)
 */
void Brainer::executeAllTasks(const string &folder) {
    buildGraph();
    runAssignment();
    interpretFlowResults();
    runRiskAnalysis();
    saveOutput(folder);
}