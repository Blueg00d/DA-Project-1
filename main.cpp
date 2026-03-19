#include <iostream>
#include "data_structures/ConferenceManager.h"
#include "data_structures/Parser.h"
using namespace std;

void handleChoice(int choice, ConferenceManager& manager) {
    switch (choice) {
        case 1: case 2:
            // input file
            string filename;
            cout << "Insert the name of your file: ";
            cin >> filename;
            
            Parser parser;
            cout << "Project1SampleDatasets/input/" + filename << endl;
            parser.parseFile(filename);
            
            if (choice == 1) parser.testParser();  
            else manager = ConferenceManager(parser.getReviewers(), parser.getSubmissions(), parser.getParams());
            break;
        
    }
}

int main() {
    ConferenceManager manager;
    int choice;

    while (true) {
        cout << "☆ Choose which path to follow (this action will have consequences):" << endl;
        cout << "1: Test Parser" << endl;
        cout << "2: Read File" << endl;
        cout << "2: " << endl;
        cout << "etc" << endl;
        cout << "0: sair :p" << endl;
        cout << "input: ";
        cin >> choice;

        if (choice == 0) {
            cout << "leaving ! :p" << endl;
            break;
        }
        handleChoice(choice, manager);
    }
    return 0;
}
