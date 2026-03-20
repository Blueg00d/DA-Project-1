#include "Parser.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <filesystem> // Required for modern file system operations
using namespace std;
enum Section {
    NONE,
    SUBMISSIONS,
    REVIEWERS,
    PARAMETERS,
    CONTROL
};

bool Parser::parseFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << TXT_BOLD << FG_RED << TXT_INVERT << "ERROR OPENING FILE!" << endl << TXT_RESET;
        return false;
    }

    Section currentSection = NONE;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("#Submissions") != string::npos) {
            currentSection = SUBMISSIONS;
            continue;
        }
        if (line.find("#Reviewers") != string::npos) {
            currentSection = REVIEWERS;
            continue;
        }
        if (line.find("#Parameters") != string::npos) {
            currentSection = PARAMETERS;
            continue;
        }
        if (line.find("#Control") != string::npos) {
            currentSection = CONTROL;
            continue;
        }

        if (line[0] == '#') continue;

        auto fields = split(line);
        for (auto& f : fields) f = trim(f);

        // SUBMISSIONS
        if (currentSection == SUBMISSIONS) {
            Submission s;

            s.setId(stoi(fields[0]));
            s.setTitle(fields[1]);
            s.setAuthor(fields[2]);
            s.setEmail(fields[3]);
            s.setPrimary(stoi(fields[4]));
                if (fields.size() > 5 && !fields[5].empty()) {
                    s.setSecondary(std::stoi(fields[5]));
                }
                else {
                    s.setSecondary(-1);
                }
            submissions.push_back(s);
        }

        // REVIEWERS
        else if (currentSection == REVIEWERS) {
            Reviewer r;

            r.setId(stoi(fields[0]));
            r.setName(fields[1]);
            r.setEmail(fields[2]);
            r.setPrimary(stoi(fields[3]));
            if (fields.size() > 4 && !fields[4].empty()) {
                r.setSecondary(std::stoi(fields[4]));
            }
            else {
                r.setSecondary(-1);
            }
            reviewers.push_back(r);
        }

        // PARAMETERS
        else if (currentSection == PARAMETERS) {
            if (fields[0] == "MinReviewsPerSubmission")
                params.setMinReviewsPerSubmission(stoi(fields[1]));

            if (fields[0] == "MaxReviewsPerReviewer")
                params.setMaxReviewsPerReviewer(stoi(fields[1]));
        }

        // CONTROL
        else if (currentSection == CONTROL) {
            if (fields[0] == "GenerateAssignments")
                params.setGenerateAssigLevel(stoi(fields[1]));

            if (fields[0] == "RiskAnalysis")
                params.setRiskAnalLevel(stoi(fields[1]));

            if (fields[0] == "OutputFileName")
                params.setOutputFilename(fields[1]);
        }
    }

    return true;
}

void Parser::testParser() {
    if (this->getReviewers().empty() || this->getSubmissions().empty()) {
        cout << TXT_BOLD << FG_RED << "There isn't any Reviewers or Submissions to display." << TXT_RESET << endl;
        return;
    }

    cout << TXT_BOLD << FG_YELLOW << "|SUBMISSIONS|" << TXT_RESET << endl;
    for (const Submission& s: this->submissions) {
        cout << FG_GREEN << "ID: " << TXT_RESET << s.getId() <<
        FG_GREEN << "; Title: " << TXT_RESET << s.getTitle() <<
        FG_GREEN << "; Authors: " << TXT_RESET << s.getAuthor() <<
        FG_GREEN << "; Email: " << TXT_RESET << s.getEmail() <<
        FG_GREEN << "; Primary: " << TXT_RESET << s.getPrimary() <<
        FG_GREEN << ";  Secondary: " << TXT_RESET << s.getSecondary() << endl;
    }
    cout << endl;

    cout << TXT_BOLD << FG_YELLOW << "|REVIEWERS|" << TXT_RESET << endl;
    for (const Reviewer& r: this->reviewers) {
        cout << FG_GREEN << "ID: " << TXT_RESET << r.getId() <<
         FG_GREEN << "; Name: " << TXT_RESET << r.getName() <<
         FG_GREEN << "; Email: " << TXT_RESET << r.getEmail() <<
         FG_GREEN << "; Primary: " << TXT_RESET << r.getPrimary() <<
         FG_GREEN << "; Secondary: " << TXT_RESET << r.getSecondary() << endl;
    }
    cout << endl;

    cout << TXT_BOLD << FG_YELLOW << "|CONTROL PARAMETERS|" << TXT_RESET << endl;
    cout << FG_GREEN << "MinReviewsPerSubmission: " << TXT_RESET << this->params.getMinReviewsPerSubmission() << endl;
    cout << FG_GREEN << "MaxReviewsPerReviewer: " << TXT_RESET << this->params.getMaxReviewsPerReviewer() << endl;
    cout << FG_GREEN << "GenerateAssignments: " << TXT_RESET << this->params.getGenerateAssigLevel() << endl;
    cout << FG_GREEN << "RiskAnalysis: " << TXT_RESET << this->params.getRiskAnalLevel() << endl;
    cout << FG_GREEN << "OutputFilename: " << TXT_RESET << this->params.getOutputFilename() << endl;
    cout << endl;
}
