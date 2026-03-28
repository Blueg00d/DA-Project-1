#include <iostream>

#include "data_structures/Debugger.h"
#include "data_structures/Utils.h"
#include "data_structures/ConferenceManager.h"
#include "data_structures/Parser.h"
#include "data_structures/Tester.h"
using namespace std;

void handleChoice(int choice, ConferenceManager& manager, Parser& parser) {
    switch (choice) {
        case 1: /*PRINT FILE*/ {
            string filename = manager.getFilename();
            if (filename.empty()) {
                cout << FG_RED << TXT_INVERT << "no file found, PLEASE use option 2 first!" << endl;
                cout << TXT_RESET;
            } else {
                parser.testParser();
            }
            break;
        }
        case 2: /*READ FILE*/ {
            // input file
            string filename;

            cout << TXT_INVERT <<"Insert the name of your file: ";
            cin >> filename;
            cout << TXT_RESET;

            parser.parseFile(filename);

            manager = ConferenceManager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            manager.setFilename(filename);
            break;
        }

        case 3: case 4: /*BUILD GRAPH AND DEBUG GRAPH*/{
            manager.buildGraph();
            if (choice == 4) Debugger::debugGraph(manager);
            break;
        }
        case 5: case 6: /*RUN EDMONDS KARP AND DEBUG GRAPH FLOW*/{
            manager.runAssignment();
            manager.runRiskAnalysis();
            if (choice == 6) Debugger::debugGraphFlow(manager);
            break;
        }

        case 7: case 8: /*INTERPRET GRAPH FLOW AND DEBUG INTERPRETATION*/{
            manager.interpretFlowResults();
            if (choice == 8) Debugger::debugInterpretationResults(manager);
            break;
        }

        case 9: /*SAVE OUTPUT*/{
            manager.interpretFlowResults(); //?
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            manager.saveOutput(folder);
            break;
        }
        case 10: case 11: /*RUN ALL INPUTS AND TEST ALL INPUTS*/{
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            Tester tester(folder);
            tester.executeAllInputTasks();
            if (choice == 11) tester.compareGeneratedWithExpected();
            break;
        }
        case 12: /*CHANGE PARAMETERS*/{
            parser.changeparams();
            manager.setParams(parser.getParams()); //warn manager that the params changed
        }
    }
}

int main() {
    cout << CLR_ALL;
    ConferenceManager manager;
    Parser parser;
    int choice;

    string optionsColor = FG_CYAN;
    string letterColor = TXT_RESET;
    string boxColor = FG_YELLOW;

    cout << FG_RED << R"(
                         _..-------++._
                     _.-'/ |      _||  \"--._
               __.--'`._/_\j_____/_||___\    `----.
          _.--'_____    |          \     _____    /
        _j    /,---.\   |        =o |   /,---.\   |_
        [__]==// .-. \\==`===========/==// .-. \\=[__]
         `-._|\ `-' /|___\_________/___|\ `-' /|_.-'
               `---'                     `---'
)" << TXT_RESET;
    while (true) {
        cout << TXT_BOLD << FG_YELLOW << TXT_INVERT << "☆ Choose which path to follow (this action will have consequences):" << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[1]" << TXT_RESET << letterColor << " Print File" << boxColor << "                                                 *" << TXT_RESET << endl;
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
        cout << boxColor << "*" << optionsColor << "[12]" << TXT_RESET << letterColor << " Change Parameters" << boxColor << "                                          *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[0]" << TXT_RESET << letterColor << " Exit :p" << boxColor << "                                                     *" << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << FG_YELLOW << TXT_INVERT << "Input: ";
        cin >> choice;
        cout << TXT_RESET << CLR_ALL;

        if (choice == 0) {
            break;
        }
        handleChoice(choice, manager, parser);
    }
    return 0;
}
