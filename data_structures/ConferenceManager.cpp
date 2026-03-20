#include "ConferenceManager.h"

// Constructor
ConferenceManager::ConferenceManager(
        vector<Reviewer> reviewers,
        vector<Submission> submissions,
        Parameters params
    ) 
{
        this->reviewers = reviewers;
        this->submissions = submissions;
        this->params = params;
}

void ConferenceManager::createNodes() {
    this->graph.addVertex(0); // Represents Source
    this->graph.addVertex(1); // Represents Sink

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

void ConferenceManager::connectSourceSinkToNodes() {
    // Connecting Source to Reviewers
    for (pair<int, Reviewer*> p: this->nodesToReviewers) {
        this->graph.addEdge(0, p.first, this->params.getMaxReviewsPerReviewer());
    }

    // Connecting Submissions to Sink
    for (pair<int, Submission*>p: this->nodesToSubmissions) {
        this->graph.addEdge(p.first, 1, this->params.getMinReviewsPerSubmission());
    }
}

void ConferenceManager::connectNodes() {
    switch (this->params.getGenerateAssigLevel()) {
        case 0: break; // Ainda não percebi isto crl
        case 1:
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary()) { // Verifies Reviwer's Primary Expertise Area with Submission's Primary Area
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
                }
            }
            break;
        case 2:
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary() || // Verifies Reviwer's Primary Expertise Area with Submission's Primary Area
                       (submission.second->getSecondary() != -1 && reviewer.second->getPrimary() == submission.second->getSecondary()) // Verifies Reviwer's Primary Expertise Area with Submission's Secondary Area
                ) {
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
                }
            }
            break;
        case 3: 
            for (pair<int, Reviewer*> reviewer: this->nodesToReviewers) {
                for (pair<int, Submission*> submission: this->nodesToSubmissions) {
                    if (reviewer.second->getPrimary() == submission.second->getPrimary() || // Verifies Reviwer's Primary Expertise Area with Submission's Primary Area
                        (submission.second->getSecondary() != -1 && reviewer.second->getPrimary() == submission.second->getSecondary()) || // Verifies Reviwer's Primary Expertise Area with Submission's Secondary Area
                        (reviewer.second->getSecondary() != -1 && reviewer.second->getSecondary() == submission.second->getPrimary()) || // Verifies Reviwer's Secondary Expertise Area with Submission's Primary Area
                        (reviewer.second->getSecondary() != -1 && submission.second->getSecondary() != -1 && reviewer.second->getSecondary() == submission.second->getSecondary()) // Verifies Reviwer's Secondary Expertise Area with Submission's Secondary Area
                    ) {
                        this->graph.addEdge(reviewer.first, submission.first, 1);
                    }
                }
            }
    }
}

void ConferenceManager::buildGraph() {
    createNodes();
    connectSourceSinkToNodes();
    connectNodes();
}

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

void ConferenceManager::runAssignment() {
    //erase previous graph so we can start over
    this->graph = Graph<int>();
    //build the new graph with new data
    buildGraph();
    double flow = graph.edmondsKarp(0,1);
}