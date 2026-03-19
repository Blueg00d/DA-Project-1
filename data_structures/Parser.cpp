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

bool Parser::parseFile(const string filename) {
    std::filesystem::path current_dir = std::filesystem::current_path();
    cout << current_dir << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file\n";
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
    cout << "|SUBMISSIONS|" << endl;
    for (const Submission& s: this->submissions) {
        cout << "ID: " << s.getId() <<
        "; Title: " << s.getTitle() <<
        "; Authors: " << s.getAuthor() <<
        "; Email: " << s.getEmail() <<
        "; Primary: " << s.getPrimary() <<
        "; Secondary: " << s.getSecondary() << endl; 
    }
    cout << endl;

    cout << "|REVIEWERS|" << endl;
    for (const Reviewer& r: this->reviewers) {
        cout << "ID: " << r.getId() <<
        "; Name: " << r.getName() <<
        "; Email: " << r.getEmail() <<
        "; Primary: " << r.getPrimary() <<
        "; Secondary: " << r.getSecondary() << endl;
    }
    cout << endl;

    cout << "|CONTROL PARAMETERS|" << endl;
    cout << "MinReviewsPerSubmission: " << this->params.getMinReviewsPerSubmission() << endl;
    cout << "MaxReviewsPerReviewer: " << this->params.getMaxReviewsPerReviewer() << endl;
    cout << "GenerateAssignments: " << this->params.getGenerateAssigLevel() << endl;
    cout << "RiskAnalysis: " << this->params.getRiskAnalLevel() << endl;
    cout << "OutputFilename: " << this->params.getOutputFilename() << endl;
    cout << endl;
}
