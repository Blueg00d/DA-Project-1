/**
 * @file main.cpp
 * @brief Main entry point for the DA-Project-1 application.
 *
 * This file contains the main menu loop, command-line argument handling,
 * and the primary functions to interact with the Conference Manager (Brainer)
 * and input Parser.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

#include "data_structures/utils/Debugger.h"
#include "data_structures/utils/Utils.h"
#include "data_structures/core/Brainer.h"
#include "data_structures/io/Parser.h"
#include "data_structures/tests/Tester.h"
using namespace std;

/**
 * @enum MenuOption
 * @brief Defines the available options in the interactive console menu.
 */
enum class MenuOption {
    EXIT = 0,                   /**< Exit the application */
    PRINT_FILE = 1,             /**< Print the parsed dataset file contents */
    READ_FILE = 2,              /**< Read and parse a new dataset file */
    BUILD_GRAPH = 3,            /**< Build the flow graph from the parsed data */
    DEBUG_GRAPH = 4,            /**< Print the graph nodes and edges for debugging */
    RUN_EDMONDS_KARP = 5,       /**< Run the max-flow algorithm to assign reviews */
    DEBUG_GRAPH_FLOW = 6,       /**< Print the graph flows for debugging */
    INTERPRET_GRAPH_FLOW = 7,   /**< Interpret the resulting max flow into assignments */
    DEBUG_INTERPRETATION = 8,   /**< Debug the interpreted assignment results */
    SAVE_OUTPUT = 9,            /**< Save the generated assignments to a file */
    RUN_ALL_INPUTS = 10,        /**< Run the pipeline for all files in a folder */
    TEST_ALL_INPUTS = 11,       /**< Test generated outputs against expected outputs */
    CHANGE_PARAMETERS = 12      /**< Interactively change the assignment parameters */
};

/**
 * @brief Handles the user interface for changing execution parameters.
 * * Allows the user to modify parameters like min/max reviews, assignment generation
 * level, and risk analysis level.
 * * @param parser A reference to the Parser to update internal parameters.
 * @param manager A reference to the Brainer to apply the updated parameters.
 */
void handleChangeParameters(Parser& parser, Brainer& manager) {
    string filename = manager.getFilename();
    if (filename.empty()) {
        cout << CLR_ALL << FG_RED << TXT_INVERT << "No file found, PLEASE use option 2 first!" << endl;
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

        if (cin.fail() || parameter < 1 || parameter > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << CLR_ALL << TXT_RESET << FG_RED << TXT_INVERT <<  "Invalid input." << TXT_RESET << endl;
            return;
        }

        cout << TXT_RESET << CLR_ALL;
        cout << TXT_BOLD << TXT_INVERT << "What do you want to change it for? ";
        cin >> value;

        if (cin.fail() || value < 0 || (parameter == 3 && value > 3) || (parameter == 4 && value > 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << CLR_ALL << TXT_RESET << FG_RED << TXT_INVERT << "Invalid input." << TXT_RESET << endl;
            return;
        }

        Parameters& params = parser.getParams();

        switch (parameter) {
            case 1: params.setMinReviewsPerSubmission(value); break;
            case 2: params.setMaxReviewsPerReviewer(value); break;
            case 3: params.setGenerateAssigLevel(value); break;
            case 4: params.setRiskAnalLevel(value); break;
        }

        cout << CLR_ALL << TXT_RESET << FG_GREEN << TXT_INVERT << "Changed Successfully." << TXT_RESET << endl;
        manager.setParams(params);
    }
}

/**
 * @brief Executes the logic corresponding to the user's menu choice.
 * * @param choice The selected MenuOption enum value.
 * @param manager A reference to the active Brainer (Conference Manager) instance.
 * @param parser A reference to the active Parser instance.
 */
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

            parser.parseFile("samples/input/" + filename);

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
            manager.interpretFlowResults();
            string folder;
            cout << TXT_INVERT <<"Insert the name of your folder: ";
            cin >> folder;
            cout << TXT_RESET;
            manager.saveOutput(folder);
            break;
        }
        case MenuOption::RUN_ALL_INPUTS:
        case MenuOption::TEST_ALL_INPUTS: /*RUN ALL INPUTS AND TEST ALL INPUTS*/{
            Tester tester("samples");
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

/**
 * @brief Main function of the application.
 * * Supports both an interactive menu-driven mode and a batch-processing mode
 * triggered by command line arguments.
 * * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns 0 on successful execution, or 1 if an error occurred.
 */
int main(int argc, char* argv[]) {
    cout << CLR_ALL;
    Brainer manager;
    Parser parser;
    if (argc > 1 && string(argv[1])=="-b") {
        if (argc!=4) {
            cerr << "Usage: ./myProg -b input.csv output.csv" << endl;
            return 1;
        }
        string inputFile = "samples/input/" + string(argv[2]);
        string outputFile = "samples/generated/" + string(argv[3]);

        if (!parser.parseFile(inputFile)) {
            cerr << "Error parsing input file: " << inputFile << endl;
            return 1;
        }
        // Initialize manager
        manager = Brainer(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
        manager.setFilename(inputFile);

        // Run full pipeline
        manager.buildGraph();
        manager.runAssignment();
        manager.runRiskAnalysis();
        manager.interpretFlowResults();

        // Save output
        manager.saveOutput(outputFile);

        return 0;
    }
    int choiceInt;

    string optionsColor = FG_CYAN;
    string letterColor = TXT_RESET;
    string boxColor = FG_YELLOW;

    // Print the car ASCII art
    cout << FG_RED << TXT_BOLD << TXT_ITALIC <<
        "         Scientific Conference Organization Tool"
        << TXT_RESET
        << FG_RED <<R"(
                         _..-------++._
                     _.-'/ |      _||  \"--._
               __.--'`._/_\j_____/_||___\    `----.
          _.--'_____    |          \     _____    /
        _j    /,---.\   |        =o |   /,---.\   |_
        [__]==// .-. \\==`===========/==// .-. \\=[__]
         `-._|\ `-' /|___\_________/___|\ `-' /|_.-'
               `---'                     `---'
)" << TXT_RESET
    << FG_RED <<
        "       Delivering the fastest tools you'll ever need" << endl <<
            "                   Since 14/04/2026"
    << TXT_RESET << endl;

    // Add a delay in milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    cout << CLR_ALL;

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

        if (cin.fail() || choiceInt < 0 || choiceInt > 12) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << CLR_ALL << FG_RED << TXT_INVERT << "Invalid input! Please input a valid menu integer." << TXT_RESET << endl;
            continue;
        }

        cout << TXT_RESET << CLR_ALL << endl;

        MenuOption choice = static_cast<MenuOption>(choiceInt);
        if (choice == MenuOption::EXIT) {
            break;
        }
        handleChoice(choice, manager, parser);
    }
    return 0;
}