#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "func.h"
using namespace std;

ifstream finC("Candidat.txt");
ifstream finN("Examen.txt");

int main() {
    bool running = true;
    string lines;
    welcome();
    while (running) {
        int choice = menu();
        switch (choice) {
            case 0:
                running = false;
                break;
            case 1:
                while (getline(finC, lines)) {
                    cout << lines << "\n";
                }
                finC.clear();
                finC.seekg(0);
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;

            case 2:
                while (getline(finN, lines)) {
                    cout << lines << "\n";
                }
                finN.clear();
                finN.seekg(0);
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;
            case 3:
                addCandidat();
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;
            case 4:
                addGrades();
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;
            case 5:
                deleteCandidat();
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;
            case 6:
                deleteGrades();
                cout << "\nPress RETURN to continue...";
                cin.ignore();
                cin.get();
                ClearScreen();
                break;
            default:
                cout << "!!!Error!!! invalid option\n\n";
                break;
        }
    }
    return 0;
}