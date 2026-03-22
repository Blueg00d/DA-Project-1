#include "ConferenceManager.h"

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

/**
 * @copybrief createNodes
 *
 * Makes source have default value of 0 and sink have default value of 1
 *
 * Time complexity: O(R+S)
 * Iterates over all Reviewers (R) and all Submissions (S) and adds them to an unordered map
 */
void ConferenceManager::createNodes() {
    this->graph.addVertex(0); // Represents Source
    this->graph.addVertex(1); //Represents Sink

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
    // Connecting Source to Reviewers
    for (pair<int, Reviewer*> p: this->nodesToReviewers) {
        this->graph.addEdge(0, p.first, this->params.getMaxReviewsPerReviewer());
    }

    // Connecting submissions to Sink
    for (pair<int, Submission*>p: this->nodesToSubmissions) {
        this->graph.addEdge(p.first, 1, this->params.getMinReviewsPerSubmission());
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
        case 0: break; //Only prints results in terminal but doesn't write output
        case 1:
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
                       (submission.second->getSecondary() != -1 && reviewer.second->getPrimary() == submission.second->getSecondary()) //Verifies Reviewer's Primary Expertise Area with Submission's Secondary Area
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
                        (submission.second->getSecondary() != -1 && reviewer.second->getPrimary() == submission.second->getSecondary()) || // Verifies Reviewer's Primary Expertise Area with Submission's Secondary Area
                        (reviewer.second->getSecondary() != -1 && reviewer.second->getSecondary() == submission.second->getPrimary()) || // Verifies Reviewer's Secondary Expertise Area with Submission's Primary Area
                        (reviewer.second->getSecondary() != -1 && submission.second->getSecondary() != -1 && reviewer.second->getSecondary() == submission.second->getSecondary()) // Verifies Reviewer's Secondary Expertise Area with Submission's Secondary Area
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
 */
void ConferenceManager::debugGraph() const{

    // Print Source to Reviewers Edges
    for (Edge<int>* e: this->graph.findVertex(0)->getAdj()) {
        cout << "Source--- " << e->getWeight() << " ---" << this->nodesToReviewers.at(e->getDest()->getInfo())->getId() << endl;
    }
    cout << endl;

    // Print Reviwers to Submissions Edges
    for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
        for (Edge<int>* e: this->graph.findVertex(reviewer.first)->getAdj()) {
            cout << this->nodesToReviewers.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---" << this->nodesToSubmissions.at(e->getDest()->getInfo())->getId() << endl;
        }
    }
    cout << endl;

    // Print Submissions to Sink Edges
    for (Edge<int>* e: this->graph.findVertex(1)->getIncoming()) {
        cout << this->nodesToSubmissions.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---Sink" << endl;
    }
}

/**
 *@copybrief runAssignment
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
    double flow = graph.edmondsKarp(0,1);
}