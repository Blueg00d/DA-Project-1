#include <iostream>
#include "Utils.h"
#include "data_structures/ConferenceManager.h"
#include "data_structures/Parser.h"
using namespace std;

void handleChoice(int choice, ConferenceManager& manager) {
    switch (choice) {
        case 1: case 2: {
            // input file
            string filename;
            cout << "Insert the name of your file: ";
            cin >> filename;
            
            Parser parser;
            parser.parseFile(filename);
            
            if (choice == 1) parser.testParser();  
            else manager = ConferenceManager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            break;
        }
        case 3: case 4: {
            manager.buildGraph();
            if (choice == 4) manager.debugGraph();
            break;
        }
        case 5: case 6: {
            manager.runAssignment();
            if (choice == 6) manager.debugGraphFLow();
            break;
        }

        case 7: case 8: {
            manager.interpretFlowResults();
            if (choice == 8) manager.debugInterpretationResults();
            break;
        }
    }
}

int main() {
    ConferenceManager manager;
    int choice;

    while (true) {
        cout << TXT_BOLD << FG_RED << "☆ Choose which path to follow (this action will have consequences):" << TXT_RESET << endl;
        cout << "1: Test Parser" << endl;
        cout << "2: Read File" << endl;
        cout << "3: Build Graph" << endl;
        cout << "4: Debug Graph" << endl;
        cout << "5: Run Edmond Karp's Algorithm" << endl;
        cout << "6: Debug Graph FLow" << endl;
        cout << "7: Interpret Graph Flow" << endl;
        cout << "8: Debug Interpretation" << endl;
        cout << "0: Exit :p" << endl;
        cout << "input: ";
        cin >> choice;
        cout << CLR_LINE <<;

        if (choice == 0) {
            cout << "leaving ! :p" << endl;
            break;
        }
        handleChoice(choice, manager);
    }
    return 0;
}
