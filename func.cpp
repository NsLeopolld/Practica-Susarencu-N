#include "func.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstdio>

void ClearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void welcome() {
    std::cout << "                              ------------------------------------------------------------\n";
    std::cout << "                              | Bun Venitin programul Nr.28 Realizat de Susarencu Nikita |\n";
    std::cout << "                              ------------------------------------------------------------\n";
    std::cout << "                              (Pentru a iesi din program introduceti: 0)\n\n";
}

int menu() {
    int selection;
    std::cout << "1. Afisarea pe ecran a informatiei despre toti candidatii\n";
    std::cout << "2. Afisarea pe ecran a informatiei despre toate notele de examen\n";
    std::cout << "3. Adaugarea informatiei despre un candidat nou\n";
    std::cout << "4. Adaugarea informatiei despre notele de examen ale candidatului adaugat la punctul 1\n";
    std::cout << "5. Stergerea informatiei despre un candidat, al carui nume si prenume sunt introduse de la tastatura\n";
    std::cout << "6. Stergerea informatiei despre notele de examen ale unui candidat, al carui cod este introdus de la tastatura\n";
    std::cout << "7. Modificarea informatiei despre un candidat, al carui nume si prenume sunt introduse de la tastatura\n";
    std::cout << "8. Modificarea informatiei despre notele de examen ale unui candidat, al carui cod este introdus de la tastatura\n";
    std::cout << "9. Crearea fisierului text Average.txt, care contine informatii despre candidati, cu calcularea mediei\n";
    std::cout << "10. Afisarea pe ecran, in ordine alfabetica, a tuturor candidatilor care sustin examenul la disciplina la alegere\n";
    std::cout << "11. Afisarea pe ecran a informatiei despre candidatul cu cea mai mare medie\n";
    std::cout << "12. Afisarea pe ecran a numarului de restantieri\n";
    std::cout << "0. Exit program\n";
    std::cout << "Selectati o optiune de mai sus: ";
    std::cin >> selection;
    std::cout << "\n";
    return selection;
}

void addCandidat() {
    std::ifstream fin("Candidat.txt");
    int maxId = 1000;
    std::string line;
    while (std::getline(fin, line)) {
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            int id = std::stoi(line.substr(0, pos));
            if (id > maxId) maxId = id;
        }
    }
    fin.close();
    int newId = maxId + 1;
    std::string nume, prenume, scoala, profil, limba, disciplina;
    std::cout << "Introduceti numele: ";
    std::cin >> nume;
    std::cout << "Introduceti prenumele: ";
    std::cin >> prenume;
    std::cout << "Introduceti scoala: ";
    std::cin.ignore();
    std::getline(std::cin, scoala);
    std::cout << "Introduceti profilul: ";
    std::getline(std::cin, profil);
    std::cout << "Introduceti limba: ";
    std::getline(std::cin, limba);
    std::cout << "Introduceti disciplina: ";
    std::getline(std::cin, disciplina);
    std::ofstream fout("Candidat.txt", std::ios::app);
    fout << newId << " | " << nume << " " << prenume << " | " << scoala << " | " << profil << " | " << limba << " | " << disciplina << "\n\n";
    fout.close();
    std::cout << "Candidat adaugat cu ID: " << newId << "\n";
}

void addGrades() {
    int id;
    double nota1, nota2, nota3;
    std::cout << "Introduceti ID-ul candidatului: ";
    std::cin >> id;
    std::cout << "Introduceti nota 1: ";
    std::cin >> nota1;
    std::cout << "Introduceti nota 2: ";
    std::cin >> nota2;
    std::cout << "Introduceti nota 3: ";
    std::cin >> nota3;
    std::ofstream fout("Examen.txt", std::ios::app);
    fout << id << " " << nota1 << " " << nota2 << " " << nota3 << "\n\n";
    fout.close();
    std::cout << "Note adaugate pentru ID: " << id << "\n";
}

void deleteCandidat() {
    std::string nume, prenume;
    std::cout << "Introduceti numele: ";
    std::cin >> nume;
    std::cout << "Introduceti prenumele: ";
    std::cin >> prenume;
    std::string fullName = nume + " " + prenume;
    std::ifstream finC("Candidat.txt");
    std::string line;
    int idToDelete = -1;
    while (std::getline(finC, line)) {
        size_t pos0 = line.find(' ');
        if (pos0 != std::string::npos) {
            int id = std::stoi(line.substr(0, pos0));
            size_t pos1 = line.find(" | ", pos0);
            if (pos1 != std::string::npos) {
                size_t pos2 = line.find(" | ", pos1 + 3);
                if (pos2 != std::string::npos) {
                    std::string name = line.substr(pos1 + 3, pos2 - pos1 - 3);
                    size_t start = name.find_first_not_of(" ");
                    if (start != std::string::npos) {
                        size_t end = name.find_last_not_of(" ");
                        name = name.substr(start, end - start + 1);
                    }
                    if (name == fullName) {
                        idToDelete = id;
                        break;
                    }
                }
            }
        }
    }
    finC.close();
    if (idToDelete == -1) {
        std::cout << "Candidat nu gasit.\n";
        return;
    }
    std::ifstream finC2("Candidat.txt");
    std::ofstream foutC("tempC.txt");
    while (std::getline(finC2, line)) {
        size_t pos = line.find(' ');
        int cid = std::stoi(line.substr(0, pos));
        if (cid != idToDelete) {
            foutC << line << "\n";
        }
    }
    finC2.close();
    foutC.close();
    std::remove("Candidat.txt");
    std::rename("tempC.txt", "Candidat.txt");
    std::ifstream finE("Examen.txt");
    std::ofstream foutE("tempE.txt");
    while (std::getline(finE, line)) {
        size_t pos = line.find(' ');
        int cid = std::stoi(line.substr(0, pos));
        if (cid != idToDelete) {
            foutE << line << "\n";
        }
    }
    finE.close();
    foutE.close();
    std::remove("Examen.txt");
    std::rename("tempE.txt", "Examen.txt");
    std::cout << "Candidat sters.\n";
}

void deleteGrades() {
    int id;
    std::cout << "Introduceti ID-ul candidatului: ";
    std::cin >> id;
    std::ifstream fin("Examen.txt");
    std::ofstream fout("tempE.txt");
    std::string line;
    bool found = false;
    while (std::getline(fin, line)) {
        size_t pos = line.find(' ');
        int cid = std::stoi(line.substr(0, pos));
        if (cid != id) {
            fout << line << "\n";
        } else {
            found = true;
        }
    }
    fin.close();
    fout.close();
    std::remove("Examen.txt");
    std::rename("tempE.txt", "Examen.txt");
    if (found) {
        std::cout << "Note sterse pentru ID: " << id << "\n";
    } else {
        std::cout << "ID nu gasit.\n";
    }
}