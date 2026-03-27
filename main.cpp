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

    string optionsColor = FG_CYAN;
    string letterColor = TXT_RESET;
    string boxColor = FG_YELLOW;

    while (true) {
        cout << TXT_BOLD << FG_YELLOW << TXT_INVERT << "☆ Choose which path to follow (this action will have consequences):" << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[1]" << TXT_RESET << letterColor << " Test Parser" << boxColor << "                                                 *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[2]" << TXT_RESET << letterColor << " Read File" << boxColor << "                                                   *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[3]" << TXT_RESET << letterColor << " Build Graph" << boxColor << "                                                 *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[4]" << TXT_RESET << letterColor << " Debug Graph" << boxColor << "                                                 *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[5]" << TXT_RESET << letterColor << " Run Edmond Karp's Algorithm" << boxColor << "                                 *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[6]" << TXT_RESET << letterColor << " Debug Graph FLow" << boxColor << "                                            *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[7]" << TXT_RESET << letterColor << " Interpret Graph Flow" << boxColor << "                                        *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[8]" << TXT_RESET << letterColor << " Debug Interpretation" << boxColor << "                                        *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[9]" << TXT_RESET << letterColor << " Save output" << boxColor << "                                                 *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[10]" << TXT_RESET << letterColor << " Run all Inputs" << boxColor << "                                             *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[11]" << TXT_RESET << letterColor << " Test all Inputs" << boxColor << "                                            *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[0]" << TXT_RESET << letterColor << " Exit :p" << boxColor << "                                                     *" << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << FG_YELLOW << TXT_INVERT << "Input: ";
        cin >> choice;
        cout << TXT_RESET << CLR_ALL;

        if (choice == 0) {
            break;
        }
        handleChoice(choice, manager);
    }
    return 0;
}
