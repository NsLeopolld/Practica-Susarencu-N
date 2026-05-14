#include "func.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <limits>

const int MAX_LINE = 512;
const int MAX_CANDIDATES = 512;
const int MAX_FIELD = 128;
const int MAX_NAME = 64;
const int MAX_DISC = 64;

void trim(char *out, const char *in, size_t outSize) {
    const char *start = in;
    while (*start && std::isspace((unsigned char)*start)) {
        start++;
    }
    const char *end = in + std::strlen(in);
    while (end > start && std::isspace((unsigned char)*(end - 1))) {
        end--;
    }
    size_t len = (end > start) ? (size_t)(end - start) : 0;
    if (len >= outSize) {
        len = outSize - 1;
    }
    std::memcpy(out, start, len);
    out[len] = '\0';
}

void toLowerInPlace(char *text) {
    for (size_t i = 0; text[i]; i++) {
        text[i] = static_cast<char>(std::tolower((unsigned char)text[i]));
    }
}

bool parseCandidateLine(const char *line, int &id, char *fullName, char *scoala,
                               char *profil, char *limba, char *disciplina) {
    char buffer[MAX_LINE];
    std::strncpy(buffer, line, MAX_LINE - 1);
    buffer[MAX_LINE - 1] = '\0';
    char *token = std::strtok(buffer, "|");
    if (!token) {
        return false;
    }
    trim(buffer, token, MAX_FIELD);
    if (std::sscanf(buffer, "%d", &id) != 1) {
        return false;
    }
    token = std::strtok(NULL, "|");
    if (!token) return false;
    trim(fullName, token, MAX_NAME);
    token = std::strtok(NULL, "|");
    if (!token) return false;
    trim(scoala, token, MAX_FIELD);
    token = std::strtok(NULL, "|");
    if (!token) return false;
    trim(profil, token, MAX_FIELD);
    token = std::strtok(NULL, "|");
    if (!token) return false;
    trim(limba, token, MAX_FIELD);
    token = std::strtok(NULL, "|");
    if (!token) return false;
    trim(disciplina, token, MAX_FIELD);
    return true;
}

bool parseExamLine(const char *line, int &id, double &nota1, double &nota2, double &nota3) {
    return std::sscanf(line, "%d %lf %lf %lf", &id, &nota1, &nota2, &nota3) == 4;
}

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
    char line[MAX_LINE];
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        if (std::sscanf(line, "%d", &id) == 1 && id > maxId) {
            maxId = id;
        }
    }
    fin.close();
    int newId = maxId + 1;
    char nume[MAX_NAME];
    char prenume[MAX_NAME];
    char scoala[MAX_FIELD];
    char profil[MAX_FIELD];
    char limba[MAX_FIELD];
    char disciplina[MAX_FIELD];
    std::cout << "Introduceti numele: ";
    std::cin >> nume;
    std::cout << "Introduceti prenumele: ";
    std::cin >> prenume;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Introduceti scoala: ";
    std::cin.getline(scoala, MAX_FIELD);
    std::cout << "Introduceti profilul: ";
    std::cin.getline(profil, MAX_FIELD);
    std::cout << "Introduceti limba: ";
    std::cin.getline(limba, MAX_FIELD);
    std::cout << "Introduceti disciplina: ";
    std::cin.getline(disciplina, MAX_FIELD);
    std::ofstream fout("Candidat.txt", std::ios::app);
    fout << newId << " | " << nume << " " << prenume << " | " << scoala << " | " << profil << " | " << limba << " | " << disciplina << "\n";
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
    fout << id << " " << nota1 << " " << nota2 << " " << nota3 << "\n";
    fout.close();
    std::cout << "Note adaugate pentru ID: " << id << "\n";
}

void deleteCandidat() {
    char nume[MAX_NAME];
    char prenume[MAX_NAME];
    std::cout << "Introduceti numele: ";
    std::cin >> nume;
    std::cout << "Introduceti prenumele: ";
    std::cin >> prenume;
    char fullName[MAX_NAME * 2];
    std::snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);
    std::ifstream finC("Candidat.txt");
    if (!finC) {
        std::cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    char lines[MAX_CANDIDATES][MAX_LINE];
    int count = 0;
    int idToDelete = -1;
    char line[MAX_LINE];
    while (finC.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int id;
        char currentName[MAX_NAME];
        char scoala[MAX_FIELD];
        char profil[MAX_FIELD];
        char limba[MAX_FIELD];
        char disciplina[MAX_FIELD];
        if (parseCandidateLine(line, id, currentName, scoala, profil, limba, disciplina)) {
            if (idToDelete == -1 && std::strcmp(currentName, fullName) == 0) {
                idToDelete = id;
                continue;
            }
        }
        std::strncpy(lines[count], line, MAX_LINE - 1);
        lines[count][MAX_LINE - 1] = '\0';
        count++;
    }
    finC.close();
    if (idToDelete == -1) {
        std::cout << "Candidat nu gasit.\n";
        return;
    }
    std::ofstream foutC("Candidat.txt");
    for (int i = 0; i < count; i++) {
        foutC << lines[i] << "\n";
    }
    foutC.close();
    std::ifstream finE("Examen.txt");
    std::ofstream foutE("tempE.txt");
    while (finE.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int cid;
        double g1, g2, g3;
        if (!parseExamLine(line, cid, g1, g2, g3) || cid != idToDelete) {
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
    if (!fin) {
        std::cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    char line[MAX_LINE];
    std::ofstream fout("tempE.txt");
    bool found = false;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int cid;
        double nota1, nota2, nota3;
        if (parseExamLine(line, cid, nota1, nota2, nota3) && cid == id) {
            found = true;
            continue;
        }
        fout << line << "\n";
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

void modifyCandidat() {
    char nume[MAX_NAME];
    char prenume[MAX_NAME];
    std::cout << "Introduceti numele candidatului de modificat: ";
    std::cin >> nume;
    std::cout << "Introduceti prenumele candidatului de modificat: ";
    std::cin >> prenume;
    char fullName[MAX_NAME * 2];
    std::snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);
    std::ifstream fin("Candidat.txt");
    if (!fin) {
        std::cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    char lines[MAX_CANDIDATES][MAX_LINE];
    int count = 0;
    char line[MAX_LINE];
    bool found = false;
    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int id;
        char currentName[MAX_NAME];
        char scoala[MAX_FIELD];
        char profil[MAX_FIELD];
        char limba[MAX_FIELD];
        char disciplina[MAX_FIELD];
        if (!found && parseCandidateLine(line, id, currentName, scoala, profil, limba, disciplina) &&
            std::strcmp(currentName, fullName) == 0) {
            found = true;
            char newNume[MAX_NAME];
            char newPrenume[MAX_NAME];
            char newScoala[MAX_FIELD];
            char newProfil[MAX_FIELD];
            char newLimba[MAX_FIELD];
            char newDisciplina[MAX_FIELD];
            std::cout << "Introduceti noul nume: ";
            std::cin >> newNume;
            std::cout << "Introduceti noul prenume: ";
            std::cin >> newPrenume;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Introduceti noua scoala: ";
            std::cin.getline(newScoala, MAX_FIELD);
            std::cout << "Introduceti noul profil: ";
            std::cin.getline(newProfil, MAX_FIELD);
            std::cout << "Introduceti noua limba: ";
            std::cin.getline(newLimba, MAX_FIELD);
            std::cout << "Introduceti noua disciplina: ";
            std::cin.getline(newDisciplina, MAX_FIELD);
            std::snprintf(lines[count], MAX_LINE, "%d | %s %s | %s | %s | %s | %s",
                          id, newNume, newPrenume, newScoala, newProfil, newLimba, newDisciplina);
            count++;
        } else {
            std::strncpy(lines[count], line, MAX_LINE - 1);
            lines[count][MAX_LINE - 1] = '\0';
            count++;
        }
    }
    fin.close();
    if (!found) {
        std::cout << "Candidat nu gasit.\n";
        return;
    }
    std::ofstream fout("Candidat.txt");
    for (int i = 0; i < count; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
    std::cout << "Informatii candidat modificate.\n";
}

void modifyGradesById() {
    int id;
    std::cout << "Introduceti ID-ul candidatului pentru care doriti sa modificati notele: ";
    std::cin >> id;
    std::ifstream fin("Examen.txt");
    if (!fin) {
        std::cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    char lines[MAX_CANDIDATES][MAX_LINE];
    int count = 0;
    char line[MAX_LINE];
    bool found = false;
    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int cid;
        double nota1, nota2, nota3;
        if (!found && parseExamLine(line, cid, nota1, nota2, nota3) && cid == id) {
            found = true;
            double newNota1, newNota2, newNota3;
            std::cout << "Introduceti noua nota 1: ";
            std::cin >> newNota1;
            std::cout << "Introduceti noua nota 2: ";
            std::cin >> newNota2;
            std::cout << "Introduceti noua nota 3: ";
            std::cin >> newNota3;
            std::snprintf(lines[count], MAX_LINE, "%d %.2f %.2f %.2f", id, newNota1, newNota2, newNota3);
            count++;
        } else {
            std::strncpy(lines[count], line, MAX_LINE - 1);
            lines[count][MAX_LINE - 1] = '\0';
            count++;
        }
    }
    fin.close();
    if (!found) {
        std::cout << "ID nu gasit.\n";
        return;
    }
    std::ofstream fout("Examen.txt");
    for (int i = 0; i < count; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
    std::cout << "Note modificate pentru ID: " << id << "\n";
}

void createAverageFile() {
    std::ifstream finC("Candidat.txt");
    std::ifstream finE("Examen.txt");
    if (!finC) {
        std::cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    if (!finE) {
        std::cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    int ids[MAX_CANDIDATES];
    char candidateLines[MAX_CANDIDATES][MAX_LINE];
    int countC = 0;
    char line[MAX_LINE];
    while (finC.getline(line, MAX_LINE) && countC < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int id;
        if (std::sscanf(line, "%d", &id) == 1) {
            ids[countC] = id;
            std::strncpy(candidateLines[countC], line, MAX_LINE - 1);
            candidateLines[countC][MAX_LINE - 1] = '\0';
            countC++;
        }
    }
    finC.close();
    int examIds[MAX_CANDIDATES];
    double n1[MAX_CANDIDATES];
    double n2[MAX_CANDIDATES];
    double n3[MAX_CANDIDATES];
    int countE = 0;
    while (finE.getline(line, MAX_LINE) && countE < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        if (parseExamLine(line, examIds[countE], n1[countE], n2[countE], n3[countE])) {
            countE++;
        }
    }
    finE.close();
    std::ofstream fout("Average.txt");
    fout.setf(std::ios::fixed);
    fout.precision(2);
    for (int i = 0; i < countC; i++) {
        int id = ids[i];
        fout << candidateLines[i];
        int foundIndex = -1;
        for (int j = 0; j < countE; j++) {
            if (examIds[j] == id) {
                foundIndex = j;
                break;
            }
        }
        if (foundIndex >= 0) {
            double average = (n1[foundIndex] + n2[foundIndex] + n3[foundIndex]) / 3.0;
            fout << " | Media: " << average;
        } else {
            fout << " | Media: N/A";
        }
        fout << "\n";
    }
    fout.close();
    std::cout << "Fisierul Average.txt a fost creat.\n";
}

void showCandidatesByDiscipline() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    char discipline[MAX_DISC];
    std::cout << "Introduceti disciplina: ";
    std::cin.getline(discipline, MAX_DISC);
    char wanted[MAX_DISC];
    trim(wanted, discipline, MAX_DISC);
    toLowerInPlace(wanted);
    std::ifstream fin("Candidat.txt");
    if (!fin) {
        std::cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    char matchedLines[MAX_CANDIDATES][MAX_LINE];
    char matchedNames[MAX_CANDIDATES][MAX_NAME];
    int count = 0;
    char line[MAX_LINE];
    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int id;
        char fullName[MAX_NAME];
        char scoala[MAX_FIELD];
        char profil[MAX_FIELD];
        char limba[MAX_FIELD];
        char disc[MAX_FIELD];
        if (parseCandidateLine(line, id, fullName, scoala, profil, limba, disc)) {
            char discLower[MAX_FIELD];
            trim(discLower, disc, MAX_FIELD);
            toLowerInPlace(discLower);
            if (std::strcmp(discLower, wanted) == 0) {
                std::strncpy(matchedLines[count], line, MAX_LINE - 1);
                matchedLines[count][MAX_LINE - 1] = '\0';
                std::strncpy(matchedNames[count], fullName, MAX_NAME - 1);
                matchedNames[count][MAX_NAME - 1] = '\0';
                count++;
            }
        }
    }
    fin.close();
    if (count == 0) {
        std::cout << "Nu s-au gasit candidati pentru disciplina " << discipline << ".\n";
        return;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (std::strcmp(matchedNames[i], matchedNames[j]) > 0) {
                char tempName[MAX_NAME];
                char tempLine[MAX_LINE];
                std::strcpy(tempName, matchedNames[i]);
                std::strcpy(matchedNames[i], matchedNames[j]);
                std::strcpy(matchedNames[j], tempName);
                std::strcpy(tempLine, matchedLines[i]);
                std::strcpy(matchedLines[i], matchedLines[j]);
                std::strcpy(matchedLines[j], tempLine);
            }
        }
    }
    for (int i = 0; i < count; i++) {
        std::cout << matchedLines[i] << "\n";
    }
}

void showTopAverageCandidate() {
    std::ifstream finC("Candidat.txt");
    std::ifstream finE("Examen.txt");
    if (!finC) {
        std::cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    if (!finE) {
        std::cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    int bestId = -1;
    double bestAverage = -1.0;
    char line[MAX_LINE];
    while (finE.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        double nota1, nota2, nota3;
        if (parseExamLine(line, id, nota1, nota2, nota3)) {
            double average = (nota1 + nota2 + nota3) / 3.0;
            if (average > bestAverage) {
                bestAverage = average;
                bestId = id;
            }
        }
    }
    finE.close();
    if (bestId == -1) {
        std::cout << "Nu exista date pentru calcularea celor mai mari medii.\n";
        finC.close();
        return;
    }
    bool found = false;
    while (finC.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        if (std::sscanf(line, "%d", &id) == 1 && id == bestId) {
            std::cout << line << " | Media: " << std::fixed << std::setprecision(2) << bestAverage << "\n";
            found = true;
            break;
        }
    }
    finC.close();
    if (!found) {
        std::cout << "Candidatul cu cea mai mare medie nu a fost gasit in Candidat.txt.\n";
    }
}

void countRestantieri() {
    std::ifstream fin("Examen.txt");
    if (!fin) {
        std::cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    char line[MAX_LINE];
    int count = 0;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        double nota1, nota2, nota3;
        if (parseExamLine(line, id, nota1, nota2, nota3)) {
            if (nota1 < 5.0 || nota2 < 5.0 || nota3 < 5.0) {
                count++;
            }
        }
    }
    fin.close();
    std::cout << "Numarul de restantieri este: " << count << "\n";
}
