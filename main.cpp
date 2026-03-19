#include <iostream>
#include "data_structures/ConferenceManager.h"
#include "data_structures/Parser.h"
using namespace std;

void handleChoice(int choice) {
    switch (choice) {
        case 1: {
            // input file
            string filename;
            cout << "insert the name of your file: ";
            cin >> filename;            
        }
    }
}

int main() {
    ConferenceManager manager;
    int choice;

    while (true) {
        cout << "☆ Choose which path to follow (this action will have consequences):" << endl;
        cout << "1: Read File" << endl;
        cout << "2: " << endl;
        cout << "etc" << endl;
        cout << "0: sair :p" << endl;
        cout << "input: ";
        cin >> choice;

        if (choice == 0) {
            cout << "leaving ! :p" << endl;
            break;
        }
        handleChoice(choice);
    }
    return 0;
}
