#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "func.h"
using namespace std;

ifstream finC("Candidat.txt");

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
                while(getline(finC, lines)) {
                    
                }
                break;
            case 2:
                
                break;

            default:
                cout << "Optiune invalida, incercati din nou.\n\n";
                break;
        }
    }
    return 0;
}