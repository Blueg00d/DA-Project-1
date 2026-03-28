#include "Debugger.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @copybrief debugGraph
 *
 * Time complexity: O(R*S)
 * Nested loops of Reviewers R and Submissions S
 */
void Debugger::debugGraph(const ConferenceManager& manager) {
    // Print Source to Reviewers Edges
    for (Edge<int>* e: manager.graph.findVertex(SOURCE)->getAdj()) {
        cout << "Source--- " << e->getWeight() << " ---" << manager.nodesToReviewers.at(e->getDest()->getInfo())->getId() << endl;
    }
    cout << endl;

    // Print Reviewers to Submissions Edges
    for (pair<int, Reviewer*> reviewer: manager.nodesToReviewers) {
        for (Edge<int>* e: manager.graph.findVertex(reviewer.first)->getAdj()) {
            cout << manager.nodesToReviewers.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---" << manager.nodesToSubmissions.at(e->getDest()->getInfo())->getId() << endl;
        }
    }
    cout << endl;

    // Print Submissions to Sink Edges
    for (Edge<int>* e: manager.graph.findVertex(SINK)->getIncoming()) {
        cout << manager.nodesToSubmissions.at(e->getOrig()->getInfo())->getId() << "--- " << e->getWeight() << " ---Sink" << endl;
    }
}


/**
 * @copybrief debugGraphFLow
 * Time complexity: O(R*S)
 * Nested cycle of Reviewers R and Submissions S
 */
void Debugger::debugGraphFlow(const ConferenceManager& manager) {
    std::stringstream ss;
    for(auto v : manager.graph.getVertexSet()) {
        ss << v->getInfo() << "-> (";
        for (const auto e : v->getAdj())
            ss << (e->getDest())->getInfo() << "[Flow: " << e->getFlow() << "] ";
        ss << ") || ";
    }

    std::cout << ss.str() << std::endl << std::endl;
}


/**
 * @copybrief debugInterpretationResults
 * Time complexity: O()
 */
void Debugger::debugInterpretationResults(const ConferenceManager& manager) {
    cout << TXT_BOLD << FG_GREEN << "#SubmissionId,ReviewerId,Match" << TXT_RESET << endl;
    for (const MatchResult& ms: manager.matchResults) {
        cout << ms.toStringSubRevMatch() << endl;
    }

    cout << TXT_BOLD << FG_GREEN << "#ReviewerId,SubmissionId,Match" << TXT_RESET <<endl;
    for (const MatchResult& ms: manager.matchResults) {
        cout << ms.toStringRevSubMatch() << endl;
    }

    cout << TXT_BOLD << FG_YELLOW << "#Total: " << manager.matchResults.size() << TXT_RESET <<endl;

    if (!manager.missingReviewsResults.empty()) {
        cout << TXT_BOLD << FG_GREEN << "#SubmissionId,Domain,MissingReviews" << TXT_RESET <<endl;
        for (const MissingReviewsResult& ms: manager.missingReviewsResults) {
            cout << ms.toStringMissingReviewsResult() << endl;
        }
    }

    if (manager.params.getRiskAnalLevel() > 0) {
        cout << "#Risk Analysis: " << manager.params.getRiskAnalLevel() << endl;

        for (size_t i = 0; i < manager.riskyReviewers.size(); i++) {
            cout << manager.riskyReviewers[i] << (i == manager.riskyReviewers.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }
}