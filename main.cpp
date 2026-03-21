#include <iostream>
#include "data_structures/Utils.h"
#include "data_structures/ConferenceManager.h"
#include "data_structures/Parser.h"
#include "data_structures/Tester.h"
using namespace std;

void handleChoice(int choice, ConferenceManager& manager) {
    switch (choice) {
        case 1: case 2: {
            // input file
            string filename;
            cout << TXT_INVERT <<"Insert the name of your file: ";
            cin >> filename;
            cout << TXT_RESET;

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
            manager.runRiskAnalysis();
            if (choice == 6) manager.debugGraphFLow();
            break;
        }

        case 7: case 8: {
            manager.interpretFlowResults();
            if (choice == 8) manager.debugInterpretationResults();
            break;
        }

        case 9: {
            manager.interpretFlowResults(); //?
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            manager.saveOutput(folder);
            break;
        }
        case 10: case 11:
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            Tester tester(folder);
            tester.executeAllInputTasks();
            if (choice == 11) tester.compareGeneratedWithExpected();
            break;
    }
}

int main() {
    cout << CLR_ALL;
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
        cout << "9: Save output" << endl;
        cout << "10: Run all Inputs" << endl;
        cout << "11: Test all Inputs" << endl;
        cout << "0: Exit :p" << endl;
        cout << TXT_INVERT << "Input: ";
        cin >> choice;
        cout << TXT_RESET << CLR_ALL;

        if (choice == 0) {
            break;
        }
        handleChoice(choice, manager);
    }
    return 0;
}
