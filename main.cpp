#include <iostream>
#include <fstream>
#include <limits>
#include "libraries/func.h"
using namespace std;

static void pause() {
    cout << "\nApasati ENTER pentru a continua...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    ClearScreen();
    bool running = true;
    welcome();
    while (running) {
        int choice = menu();
        switch (choice) {
            case 0:
                running = false;
                cout << "La revedere!\n";
                break;

            case 1: {
                ifstream fin("data/Candidat.txt");
                if (!fin) {
                    cout << "  [Eroare] Nu se poate deschide Candidat.txt\n";
                } else {
                    printCandidatTable(fin);
                    fin.close();
                }
                pause(); ClearScreen(); break;
            }

            case 2: {
                ifstream fin("data/Examen.txt");
                if (!fin) {
                    cout << "  [Eroare] Nu se poate deschide Examen.txt\n";
                } else {
                    printExamTable(fin);
                    fin.close();
                }
                pause(); ClearScreen(); break;
            }

            case 3:  addCandidat();             pause(); ClearScreen(); break;
            case 4:  addGrades();               pause(); ClearScreen(); break;
            case 5:  deleteCandidat();          pause(); ClearScreen(); break;
            case 6:  deleteGrades();            pause(); ClearScreen(); break;
            case 7:  modifyCandidat();          pause(); ClearScreen(); break;
            case 8:  modifyGradesById();        pause(); ClearScreen(); break;
            case 9:  createAverageFile();       pause(); ClearScreen(); break;
            case 10: showCandidatesByDiscipline(); pause(); ClearScreen(); break;
            case 11: showTopAverageCandidate(); pause(); ClearScreen(); break;
            case 12: countRestantieri();        pause(); ClearScreen(); break;

            default:
                cout << "  [Eroare] Optiune invalida. Alegeti un numar intre 0 si 12.\n";
                pause(); ClearScreen(); break;
        }
    }
    return 0;
}
