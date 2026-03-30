#include "Brainer.h"
#include <fstream>
#include <algorithm>
using namespace std;

#include "utils/Utils.h"

#include <utility>

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
void Brainer::setFilename(string filename) {
    this->filename = filename;
}
string Brainer::getFilename() {
    return this->filename;
}

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
 * Time complexity: O(R+S)
 *First for loop iterates through all reviewers (R) and second one through all Submissions (S)
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

    // Connecting Submissions to Sink ordered by ID
    vector<int> sortedSubIDs;
    for (auto const& [nodeID, sub] : nodesToSubmissions) {
        sortedSubIDs.push_back(nodeID);
    }

    sort(sortedSubIDs.begin(), sortedSubIDs.end(), [&](int a, int b) {
        return nodesToSubmissions[a]->getId() < nodesToSubmissions[b]->getId();
    });

    for (int nodeID : sortedSubIDs) {
        this->graph.addEdge(nodeID, SINK, this->params.getMinReviewsPerSubmission());
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

    vector<pair<int, Reviewer*>> sortedRev(this->nodesToReviewers.begin(), this->nodesToReviewers.end());
    sort(sortedRev.begin(), sortedRev.end(), [](const auto& a, const auto& b) {
        return a.second->getId() < b.second->getId();
    });

    vector<pair<int, Submission*>> sortedSub(this->nodesToSubmissions.begin(), this->nodesToSubmissions.end());
    sort(sortedSub.begin(), sortedSub.end(), [](const auto& a, const auto& b) {
        return a.second->getId() < b.second->getId();
    });

    for (const auto& reviewerPair: sortedRev) {
        for (const auto& submissionPair: sortedSub) {
            bool isEligible = false;
            Reviewer* reviewer = reviewerPair.second;
            Submission* submission = submissionPair.second;

            int rPrim = reviewer->getPrimary();
            int rSec = reviewer->getSecondary();
            int sPrim = submission->getPrimary();
            int sSec = submission->getSecondary();

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
                this->graph.addEdge(reviewerPair.first, submissionPair.first, 1);
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
 * Time complexity: O((R+S)*(R*S)^2)
 * Calls edmondsKarp with complexity O(V*E^2);
 * V is number of vertices (R+S+2);
 * E is number of edges (Source->Reviewers == R; Reviewers->Submissions == up to R*S; Submissions->Sink == S) Let it be approx. R*S
 * O(R+S+2*(R*S)^2) ≈ O((R+S)*(R*S)^2)
 */
void Brainer::runAssignment() {
    this->graph = Graph<int>();
    buildGraph();

    Vertex<int>* vSource = graph.findVertex(SOURCE);
    double requiredFlow = this->submissions.size() * this->params.getMinReviewsPerSubmission();

    vector<Edge<int>*> reviewerEdges = vSource->getAdj();
    sort(reviewerEdges.begin(), reviewerEdges.end(), [this](Edge<int>* a, Edge<int>* b) {
        int idA = this->nodesToReviewers.at(a->getDest()->getInfo())->getId();
        int idB = this->nodesToReviewers.at(b->getDest()->getInfo())->getId();
        return idA < idB;
    });

    for (Edge<int>* e : reviewerEdges) {
        e->setWeight(0);
    }

    double currentFlow = 0;
    for (Edge<int>* e : reviewerEdges) {
        e->setWeight(this->params.getMaxReviewsPerReviewer());

        currentFlow = graph.edmondsKarp(SOURCE, SINK);

        if (currentFlow >= requiredFlow) {
            break;
        }
    }
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
        for (Edge<int>* e: this->graph.findVertex(p.first)->getIncoming()) {
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
    //If it was successful or not
    this->success = (this->matchResults.size() >= this->params.getMinReviewsPerSubmission() * this->nodesToSubmissions.size());

    //Sort results
    sort(matchResults.begin(), matchResults.end());
    sort(missingReviewsResults.begin(), missingReviewsResults.end());
}


/**
 * @copybrief runRiskAnalysis
 * Time Complexity: Optimized with incremental max-flow.
 * Reruns edmondsKarp() on residual graph once per reviewer R
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
 * This algorithm would result in a temporal complexity of O(2^R * (R+S)*(R*S)^2),
 * characterized by running the Edmound's Karp Algorithm through every subset of
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

    for (int revNodeID : reviewerNodes) {
        Vertex<int>* vRev = graph.findVertex(revNodeID);
        Edge<int>* targetEdge = nullptr;
        double originalWeight = 0;
        double flowToRemove = 0;

        // Find edge from SOURCE to the specific Reviewer
        for (auto e : vSource->getAdj()) {
            if (e->getDest()->getInfo() == revNodeID) {
                targetEdge = e;
                originalWeight = e->getWeight();
                flowToRemove = e->getFlow();
                break;
            }
        }

        if (targetEdge == nullptr) continue;

        // Manually rollback flow tracing backward so that we maintain conservation
        if (flowToRemove > 0) {
            targetEdge->setFlow(0); // Removing flow from Source -> Reviewer

            double remainingToRemove = flowToRemove;
            for (auto eRevSub : vRev->getAdj()) {
                if (remainingToRemove <= 0) break;
                double f = eRevSub->getFlow();

                if (f > 0) {
                    eRevSub->setFlow(0);

                    Vertex<int>* vSub = eRevSub->getDest();
                    for (auto eSubSink : vSub->getAdj()) {
                        if (eSubSink->getDest() == vSink && eSubSink->getFlow() > 0) {
                            double reducible = std::min(eSubSink->getFlow(), f);
                            eSubSink->setFlow(eSubSink->getFlow() - reducible);
                            break;
                        }
                    }
                    remainingToRemove -= f;
                }
            }
        }

        // Temporarily nullify the target capacity for the isolated reviewer node
        targetEdge->setWeight(0);

        // Run EdmundsKarp on the residual graph to incrementally reroute dropped flow
        double flowAfter = graph.edmondsKarp(SOURCE, SINK);

        if (flowAfter < requiredFlow) {
            this->riskyReviewers.push_back(nodesToReviewers[revNodeID]->getId());
        }

        // Restore the reviewer capacity edge
        targetEdge->setWeight(originalWeight);

        // Push flow back through this reviewer to instantly restore the original max flow optimal state
        graph.edmondsKarp(SOURCE, SINK);
    }

    sort(riskyReviewers.begin(), riskyReviewers.end());
}

/**
 * @copybrief saveOutput
 * Time complexity: O(MlogM + R)
 * - M = number of matches (matchResults.size())
 * Sorts results twice (grouped by submission and then grouped by reviewer)
 * Writes matches, missing reviews and risky reviewers to output O(M+R)
 */
void Brainer::saveOutput(const string& path) {
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
 * Time complexity: O(R * (R+S)*(R*S)^2) (Note: Risk Analysis optimized incrementally)
 * buildGraph: O(R*S)
 * runAssignment: O((R+S)*(R*S)^2)
 * interpretFlowResults: O((R*S)+ElogE)
 * saveOutput: O(MlogM + R)
 */
void Brainer::executeAllTasks(const string &folder) {
    buildGraph();
    runAssignment();
    interpretFlowResults();
    runRiskAnalysis();
    saveOutput(folder);
}