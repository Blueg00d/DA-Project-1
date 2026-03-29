#include <iostream>

#include "data_structures/Debugger.h"
#include "data_structures/Utils.h"
#include "data_structures/Brainer.h"
#include "data_structures/Parser.h"
#include "data_structures/Tester.h"
using namespace std;

enum class MenuOption {
    EXIT = 0,
    PRINT_FILE = 1,
    READ_FILE = 2,
    BUILD_GRAPH = 3,
    DEBUG_GRAPH = 4,
    RUN_EDMONDS_KARP = 5,
    DEBUG_GRAPH_FLOW = 6,
    INTERPRET_GRAPH_FLOW = 7,
    DEBUG_INTERPRETATION = 8,
    SAVE_OUTPUT = 9,
    RUN_ALL_INPUTS = 10,
    TEST_ALL_INPUTS = 11,
    CHANGE_PARAMETERS = 12
};

void handleChangeParameters(Parser& parser, Brainer& manager) {
    string filename = manager.getFilename();
    if (filename.empty()) {
        cout << FG_RED << TXT_INVERT << "no file found, PLEASE use option 2 first!" << endl;
        cout << TXT_RESET << endl;
    } else {
        int parameter;
        int value;
        string optionsColor = FG_YELLOW;
        string letterColor = TXT_RESET;
        string boxColor = FG_CYAN;

        cout << TXT_BOLD << FG_CYAN << TXT_INVERT << "Please specify which parameter you want to change." << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[1]" << TXT_RESET << letterColor << " Minimum Reviews per Submission" << boxColor << "                              *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[2]" << TXT_RESET << letterColor << " Maximum Reviews per Reviewer" << boxColor << "                                *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[3]" << TXT_RESET << letterColor << " Generate Assignments" << boxColor << "                                        *" << TXT_RESET << endl;
        cout << boxColor << "*" << optionsColor << "[4]" << TXT_RESET << letterColor << " Risk Analysis" << boxColor << "                                               *" << TXT_RESET << endl;
        cout << boxColor << "==================================================================" << TXT_RESET << endl;
        cout << FG_CYAN << TXT_INVERT << "Input: ";
        cin >> parameter;
        cout << TXT_RESET << CLR_ALL;
        cout << TXT_BOLD << TXT_INVERT << "What do you want to change it for? ";
        cin >> value;
        cout << TXT_RESET << CLR_ALL << endl;

        Parameters& params = parser.getParams();
        switch (parameter) {
            case 1: params.setMinReviewsPerSubmission(value); break;
            case 2: params.setMaxReviewsPerReviewer(value); break;
            case 3: params.setGenerateAssigLevel(value); break;
            case 4: params.setRiskAnalLevel(value); break;
        }
        manager.setParams(parser.getParams()); //warn manager that the params changed
    }
}

void handleChoice(MenuOption choice, Brainer& manager, Parser& parser) {
    switch (choice) {
        case MenuOption::PRINT_FILE: /*PRINT FILE*/ {
            string filename = manager.getFilename();
            if (filename.empty()) {
                cout << FG_RED << TXT_INVERT << "no file found, PLEASE use option 2 first!" << endl;
                cout << TXT_RESET;
            } else {
                parser.testParser();
            }
            break;
        }
        case MenuOption::READ_FILE: /*READ FILE*/ {
            // input file
            string filename;

            cout << TXT_INVERT <<"Insert the name of your file: ";
            cin >> filename;
            cout << TXT_RESET;

            parser.parseFile(filename);

            manager = Brainer(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            manager.setFilename(filename);
            break;
        }

        case MenuOption::BUILD_GRAPH:
        case MenuOption::DEBUG_GRAPH: /*BUILD GRAPH AND DEBUG GRAPH*/{
            manager.buildGraph();
            if (choice == MenuOption::DEBUG_GRAPH) Debugger::debugGraph(manager);
            break;
        }
        case MenuOption::RUN_EDMONDS_KARP:
        case MenuOption::DEBUG_GRAPH_FLOW: /*RUN EDMONDS KARP AND DEBUG GRAPH FLOW*/{
            manager.runAssignment();
            manager.runRiskAnalysis();
            if (choice == MenuOption::DEBUG_GRAPH_FLOW) Debugger::debugGraphFlow(manager);
            break;
        }

        case MenuOption::INTERPRET_GRAPH_FLOW:
        case MenuOption::DEBUG_INTERPRETATION: /*INTERPRET GRAPH FLOW AND DEBUG INTERPRETATION*/{
            manager.interpretFlowResults();
            if (choice == MenuOption::DEBUG_INTERPRETATION) Debugger::debugInterpretationResults(manager);
            break;
        }

        case MenuOption::SAVE_OUTPUT: /*SAVE OUTPUT*/{
            manager.interpretFlowResults(); //?
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            manager.saveOutput(folder);
            break;
        }
        case MenuOption::RUN_ALL_INPUTS:
        case MenuOption::TEST_ALL_INPUTS: /*RUN ALL INPUTS AND TEST ALL INPUTS*/{
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            Tester tester(folder);
            tester.executeAllInputTasks();
            if (choice == MenuOption::TEST_ALL_INPUTS) tester.compareGeneratedWithExpected();
            break;
        }
        case MenuOption::CHANGE_PARAMETERS: /*CHANGE PARAMETERS*/{
            handleChangeParameters(parser, manager);
            break;
        }
        default: break;
    }
}

int main() {
    cout << CLR_ALL;
    Brainer manager;
    Parser parser;
    int choiceInt;

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
        cout << boxColor << "*" << optionsColor << "[1]" << TXT_RESET << letterColor << " Print File" << boxColor << "                                                  *" << TXT_RESET << endl;
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
        cin >> choiceInt;
        cout << TXT_RESET << CLR_ALL << endl;

        MenuOption choice = static_cast<MenuOption>(choiceInt);
        if (choice == MenuOption::EXIT) {
            break;
        }
        handleChoice(choice, manager, parser);
    }
    return 0;
}