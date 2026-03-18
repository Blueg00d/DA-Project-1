#include <iostream>
#include "data_structures/ConferenceManager.h"
using namespace std;

void handleChoice(int choice, ConferenceManager &conferenceManager) {
    switch (choice) {
        //TODO
    }
}
int main() {
    ConferenceManager manager;
    int choice;

    while (true) {
        cout << "☆ Choose which path to follow (this action will have consequences):" << endl;
        cout << "1: " << endl;
        cout << "2: " << endl;
        cout << "etc" << endl;
        cout << "0: sair :p" << endl;
        cout << "input: ";
        cin >> choice;

        if (choice == 0) break;
        handleChoice(choice, manager);
    }
    return 0;
}
