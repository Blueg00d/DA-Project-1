#include "ConferenceManager.h"
#include <fstream>
using namespace std;

#include "Utils.h"

#include <utility>

ConferenceManager::ConferenceManager(
        vector<Reviewer> reviewers,
        vector<Submission> submissions,
        Parameters params
    ) 
{
        this->reviewers = std::move(reviewers);
        this->submissions = std::move(submissions);
        this->params = std::move(params);
}
void ConferenceManager::setFilename(string filename) {
    this->filename = filename;
}
string ConferenceManager::getFilename() {
    return this->filename;
}

void ConferenceManager::setParams(Parameters params) {
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
void ConferenceManager::createNodes() {
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
 * Time complexity: O(R+S)
 *First for loop iterates through all reviewers (R) and second one through all Submissions (S)
 */
void ConferenceManager::connectSourceSinkToNodes() {
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

    // Connecting Source to Reviewers
    //for (pair<int, Reviewer*> p: this->nodesToReviewers) {
    //    this->graph.addEdge(0, p.first, this->params.getMaxReviewsPerReviewer());
    //}

    // Connecting Submissions to Sink
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
void ConferenceManager::connectNodes() {
    switch (this->params.getGenerateAssigLevel()) {
        case 0: case 1: //Only prints results in terminal but doesn't write output
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary()) { //Verifies Reviewer's primary expertise area with Submission's primary area
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
                }
            }
            break;
        case 2:
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary() || //Verifies Reviewer's Primary Expertise Area with Submission's Primary Area
                       (submission.second->getSecondary() != NOT_DEFINED && reviewer.second->getPrimary() == submission.second->getSecondary()) //Verifies Reviewer's Primary Expertise Area with Submission's Secondary Area
                ) {
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
                }
            }
            break;
        case 3: 
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary() || // Verifies Reviewer's Primary Expertise Area with Submission's Primary Area
                        (submission.second->getSecondary() != NOT_DEFINED && reviewer.second->getPrimary() == submission.second->getSecondary()) || // Verifies Reviewer's Primary Expertise Area with Submission's Secondary Area
                        (reviewer.second->getSecondary() != NOT_DEFINED && reviewer.second->getSecondary() == submission.second->getPrimary()) || // Verifies Reviewer's Secondary Expertise Area with Submission's Primary Area
                        (reviewer.second->getSecondary() != NOT_DEFINED && submission.second->getSecondary() != NOT_DEFINED && reviewer.second->getSecondary() == submission.second->getSecondary()) // Verifies Reviewer's Secondary Expertise Area with Submission's Secondary Area
                    ) {
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
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
void ConferenceManager::buildGraph() {
    createNodes();
    connectSourceSinkToNodes();
    connectNodes();
}

/**
 * @copybrief debugGraph
 *
 * Time complexity: O(R*S)
 * Nested loops of Reviewers R and Submissions S
 */
void ConferenceManager::debugGraph() const{

    // Print Source to Reviewers Edges
    for (Edge<int>* e: this->graph.findVertex(SOURCE)->getAdj()) {
        cout << "Source--- " << e->getWeight() << " ---" << this->nodesToReviewers.at(e->getDest()->getInfo())->getId() << endl;
    }
    cout << endl;

    // Print Reviewers to Submissions Edges
    for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
        for (Edge<int>* e: this->graph.findVertex(reviewer.first)->getAdj()) {
            cout << this->nodesToReviewers.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---" << this->nodesToSubmissions.at(e->getDest()->getInfo())->getId() << endl;
        }
    }
    cout << endl;

    // Print Submissions to Sink Edges
    for (Edge<int>* e: this->graph.findVertex(SINK)->getIncoming()) {
        cout << this->nodesToSubmissions.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---Sink" << endl;
    }
}

/**
 * @copybrief runAssignment
 *
 * Time complexity: O((R+S)*(R*S)^2)
 * Calls edmondsKarp with complexity O(V*E^2);
 * V is number of vertices (R+S+2);
 * E is number of edges (Source->Reviewers == R; Reviewers->Submissions == up to R*S; Submissions->Sink == S) Let it be approx. R*S
 * O(R+S+2*(R*S)^2) ≈ O((R+S)*(R*S)^2)
 */
void ConferenceManager::runAssignment() {
    //Erase previous graph so we can start over
    this->graph = Graph<int>();
    //Build the new graph with new data
    buildGraph();
    double flow = graph.edmondsKarp(SOURCE,SINK);
}

/**
 * @copybrief debugGraphFLow
 * Time complexity: O(R*S)
 * Nested cycle of Reviewers R and Submissions S
 */
void ConferenceManager::debugGraphFLow() const {
    std::stringstream ss;
    for(auto v : this->graph.getVertexSet()) {
        ss << v->getInfo() << "-> (";
        for (const auto e : v->getAdj())
            ss << (e->getDest())->getInfo() << "[Flow: " << e->getFlow() << "] ";
        ss << ") || ";
    }

    std::cout << ss.str() << std::endl << std::endl;
}

/**
 * @copybrief interpretFlowResults
 * Time complexity: O((R*S)+ElogE)
 * First it iterates through all submissions S in a nested loop with the reviewers R O(R*S)
 * Sorting results takes O(ElogE) for edges E
 */
void ConferenceManager::interpretFlowResults() {
    //Initialization
    this->matchResults.clear();
    this->missingReviewsResults.clear();
    int flow = 0;
    //Iterate through all submissions
    for (const pair<int, Submission*> p: this->nodesToSubmissions) {
        int reviewsExecuted = 0;
        Submission* s = p.second;
        for (Edge<int>* e: this->graph.findVertex(p.first)->getIncoming()) {
            if (e->getFlow() > 0) {
                flow++;
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
    //If it was successful or not
    if (flow >= this->params.getMinReviewsPerSubmission() * this->nodesToSubmissions.size()) this->success = true;
    else success = false;
    //Sort results
    sort(matchResults.begin(), matchResults.end());
    sort(missingReviewsResults.begin(), missingReviewsResults.end());
}

/**
 * @copybrief debugInterpretationResults
 * Time complexity: O()
 */
void ConferenceManager::debugInterpretationResults() const {
    cout << TXT_BOLD << FG_GREEN << "#SubmissionId,ReviewerId,Match" << TXT_RESET << endl;
    for (const MatchResult& ms: this->matchResults) {
        cout << ms.toStringSubRevMatch() << endl;
    }

    cout << TXT_BOLD << FG_GREEN << "#ReviewerId,SubmissionId,Match" << TXT_RESET <<endl;
    for (const MatchResult& ms: this->matchResults) {
        cout << ms.toStringRevSubMatch() << endl;
    }

    cout << TXT_BOLD << FG_YELLOW << "#Total: " << this->matchResults.size() << TXT_RESET <<endl;

    if (!this->missingReviewsResults.empty()) {
        cout << TXT_BOLD << FG_GREEN << "#SubmissionId,Domain,MissingReviews" << TXT_RESET <<endl;
        for (const MissingReviewsResult& ms: this->missingReviewsResults) {
            cout << ms.toStringMissingReviewsResult() << endl;
        }
    }

    if (params.getRiskAnalLevel() > 0) {
        cout << "#Risk Analysis: " << params.getRiskAnalLevel() << endl;

        for (size_t i = 0; i < this->riskyReviewers.size(); i++) {
            cout << this->riskyReviewers[i] << (i == riskyReviewers.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

/**
 * @copybrief runRiskAnalysis
 * Time Complexity: O(R * (R+S)*(R*S)^2)
 * Reruns edmondsKarp() once per reviewer R
 *
 * @details This runRiskAnalysis works only for level k == 1.
 *  If we wanted to run this function for a level k > 1, we could use a brute-force approach.
 *
 *  The algorithm would be analogous to this one but
 *  instead of pushing a single reviewerID to the reviewerNodes vector,
 *  we would create every possible subset of discarded reviewers of size <= k,
 *  resulting in \f$ \sum_{i=1}^{k} C_{i}^{R} \f$ subsets.
 *  Then, for each subset of reviewers, we would set each reviewers flow to 0,
 *  similar to what this algorithm does.
 *
 *  This algorithm would result in a temporal complexity of O(2^R * (R+S)*(R*S)^2),
 *  characterized by running the Edmound's Karp Algorithm through every subset of
 *  discarded reviewers.
 */
void ConferenceManager::runRiskAnalysis() {
    int M = params.getRiskAnalLevel();
    if (M == 0) return;

    //As createNodes() clears the map at its beginning we need to save the revID so we are not affected in the next loop
    vector<int> reviewerNodes;
    for (auto const& [nodeID, rev] : nodesToReviewers) {
        reviewerNodes.push_back(nodeID);
    }

    double requiredFlow = submissions.size() * params.getMinReviewsPerSubmission();
    //Clear previous results
    this->riskyReviewers.clear();

    for (int revNodeID : reviewerNodes) {
        //Full reset of the graph since the algorithm leaves residual flow on the edges
        this->graph = Graph<int>();
        buildGraph();

        Reviewer* currentRev = nodesToReviewers[revNodeID];
        Vertex<int>* vSource = graph.findVertex(SOURCE);
        for (auto e : vSource->getAdj()) {
            if (e->getDest()->getInfo() == revNodeID) {
                e->setWeight(0);
                break;
            }
        }

        double flowAfter = graph.edmondsKarp(SOURCE,SINK);
        if (flowAfter < requiredFlow) {
            this->riskyReviewers.push_back(currentRev->getId());
        }
    }
    sort(riskyReviewers.begin(), riskyReviewers.end());

    //Leave the graph just like we found it
    this->graph = Graph<int>();
    this->buildGraph();
    this->runAssignment();
}

/**
 * @copybrief saveOutput
 * Time complexity: O(MlogM + R)
 * - M = number of matches (matchResults.size())
 * Sorts results twice (grouped by submission and then grouped by reviewer)
 *  Writes matches, missing reviews and risky reviewers to output O(M+R)
 */
void ConferenceManager::saveOutput(const string& folder) {
    string filename = folder + params.getOutputFilename();

    //if the parser couldn't read the filename, then we need to use a default one
    if (filename.empty()) { filename = "assignment.csv"; }

    //open the writing stream so we can write in the file
    ofstream outFile(filename);

    //check if we can open/create file
    if (!outFile.is_open()) {
        cerr << TXT_BOLD << FG_RED << TXT_INVERT << "ERROR OPENING FILE!" << endl << TXT_RESET;
        return;
    }

    //write in the output file just like it prints on the terminal
    outFile << "#SubmissionId,ReviewerId,Match" << endl;
    sort(matchResults.begin(), matchResults.end(), [] (MatchResult& a, MatchResult& b) {
        if (a.getSubmissionID() != b.getSubmissionID()) return a.getSubmissionID() < b.getSubmissionID();
        else if (a.getReviewerID() != b.getReviewerID()) return a.getReviewerID() < b.getReviewerID();
        else return a.getMatch() < b.getMatch();
    });
    for (const MatchResult& ms: this->matchResults) {
        outFile << ms.toStringSubRevMatch() << endl;
    }

    outFile << "#ReviewerId,SubmissionId,Match" << endl;
    sort(matchResults.begin(), matchResults.end(), [] (MatchResult& a, MatchResult& b) {
        if (a.getReviewerID() != b.getReviewerID()) return a.getReviewerID() < b.getReviewerID();
        else if (a.getSubmissionID() != b.getSubmissionID()) return a.getSubmissionID() < b.getSubmissionID();
        else return a.getMatch() < b.getMatch();
    });
    for (const MatchResult& ms: this->matchResults) {
        outFile << ms.toStringRevSubMatch() << endl;
    }

    outFile << "#Total: " << this->matchResults.size() << endl;
    if (!this->missingReviewsResults.empty()) {
        outFile << "#SubmissionId,Domain,MissingReviews" << endl;
        for (const MissingReviewsResult& ms: this->missingReviewsResults) {
            outFile << ms.toStringMissingReviewsResult() << endl;
        }
    }

    if (params.getRiskAnalLevel() > 0) {
        outFile << "#Risk Analysis: " << params.getRiskAnalLevel() << endl;

        for (size_t i = 0; i < this->riskyReviewers.size(); i++) {
            outFile << this->riskyReviewers[i] << (i == riskyReviewers.size() - 1 ? "" : ", ");
        }
        outFile << endl;
    }

    outFile.close();
    cout << "success! results saved in: " << filename << endl;
}

/**
 * @copydoc executeAllTasks
 * Time complexity: O(R * (R+S)*(R*S)^2)
 * buildGraph: O(R*S)
 * runAssignment: O((R+S)*(R*S)^2)
 * interpretFlowResults: O((R*S)+ElogE)
 * runRiskAnalysis: O(R * (R+S)*(R*S)^2)
 * saveOutput: O(MlogM + R)
 */
void ConferenceManager::executeAllTasks(const string &folder) {
    buildGraph();
    runAssignment();
    interpretFlowResults();
    runRiskAnalysis();
    saveOutput(folder);
}

