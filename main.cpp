#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include "func.h"
using namespace std;

static const int MAX_LINE = 512;

static void pause() {
    cout << "\nPress RETURN to continue...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}

int main() {
    bool running = true;
    char lines[MAX_LINE];
    welcome();
    while (running) {
        int choice = menu();
        switch (choice) {
            case 0:
                running = false;
                break;
            case 1: {
                ifstream finC("Candidat.txt");
                if (!finC) {
                    cout << "Nu se poate deschide fisierul Candidat.txt\n";
                } else {
                    while (finC.getline(lines, MAX_LINE)) {
                        cout << lines << "\n";
                    }
                    finC.close();
                }
                pause();
                ClearScreen();
                break;
            }
            case 2: {
                ifstream finN("Examen.txt");
                if (!finN) {
                    cout << "Nu se poate deschide fisierul Examen.txt\n";
                } else {
                    while (finN.getline(lines, MAX_LINE)) {
                        cout << lines << "\n";
                    }
                    finN.close();
                }
                pause();
                ClearScreen();
                break;
            }
            case 3:
                addCandidat();
                pause();
                ClearScreen();
                break;
            case 4:
                addGrades();
                pause();
                ClearScreen();
                break;
            case 5:
                deleteCandidat();
                pause();
                ClearScreen();
                break;
            case 6:
                deleteGrades();
                pause();
                ClearScreen();
                break;
            case 7:
                modifyCandidat();
                pause();
                ClearScreen();
                break;
            case 8:
                modifyGradesById();
                pause();
                ClearScreen();
                break;
            case 9:
                createAverageFile();
                pause();
                ClearScreen();
                break;
            case 10:
                showCandidatesByDiscipline();
                pause();
                ClearScreen();
                break;
            case 11:
                showTopAverageCandidate();
                pause();
                ClearScreen();
                break;
            case 12:
                countRestantieri();
                pause();
                ClearScreen();
                break;
            default:
                cout << "!!!Error!!! invalid option\n\n";
                pause();
                ClearScreen();
                break;
        }
    }
    return 0;
}
