#include "parser.h"
#include "utils.h"
#include <fstream>
#include <iostream>
using namespace std;
enum Section {
    NONE,
    SUBMISSIONS,
    REVIEWERS,
    PARAMETERS,
    CONTROL
};

bool parser::parseFile(const string filename) {
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

//setters
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
                params.setminReviewsPerSubmission(stoi(fields[1]));

            if (fields[0] == "MaxReviewsPerReviewer")
                params.setmaxReviewsPerReviewer(stoi(fields[1]));
        }

        // CONTROL
        else if (currentSection == CONTROL) {
            if (fields[0] == "GenerateAssignments")
                params.setgenerateAssignments(stoi(fields[1]));

            if (fields[0] == "RiskAnalysis")
                params.setriskAnalysis(stoi(fields[1]));

            if (fields[0] == "OutputFileName")
                params.setoutputFileName(fields[1]);
        }
    }

    return true;
}