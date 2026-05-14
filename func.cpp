#include "func.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <limits>
using namespace std;

const int MAX_LINE = 512;
const int MAX_CANDIDATES = 512;
const int MAX_FIELD = 128;
const int MAX_NAME = 64;
const int MAX_DISC = 64;

void trim(char *out, const char *in, size_t outSize) {
    const char *start = in;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    const char *end = in + strlen(in);
    while (end > start && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    size_t len = (end > start) ? (size_t)(end - start) : 0;
    if (len >= outSize) {
        len = outSize - 1;
    }
    memcpy(out, start, len);
    out[len] = '\0';
}

void toLowerInPlace(char *text) {
    for (size_t i = 0; text[i]; i++) {
        text[i] = static_cast<char>(tolower((unsigned char)text[i]));
    }
}

bool parseCandidateLine(const char *line, int &id, char *fullName, char *scoala,
                               char *profil, char *limba, char *disciplina) {
    char buffer[MAX_LINE];
    strncpy(buffer, line, MAX_LINE - 1);
    buffer[MAX_LINE - 1] = '\0';
    char *token = strtok(buffer, "|");
    if (!token) {
        return false;
    }
    trim(buffer, token, MAX_FIELD);
    if (sscanf(buffer, "%d", &id) != 1) {
        return false;
    }
    token = strtok(NULL, "|");
    if (!token) return false;
    trim(fullName, token, MAX_NAME);
    token = strtok(NULL, "|");
    if (!token) return false;
    trim(scoala, token, MAX_FIELD);
    token = strtok(NULL, "|");
    if (!token) return false;
    trim(profil, token, MAX_FIELD);
    token = strtok(NULL, "|");
    if (!token) return false;
    trim(limba, token, MAX_FIELD);
    token = strtok(NULL, "|");
    if (!token) return false;
    trim(disciplina, token, MAX_FIELD);
    return true;
}

bool parseExamLine(const char *line, int &id, double &nota1, double &nota2, double &nota3) {
    return sscanf(line, "%d %lf %lf %lf", &id, &nota1, &nota2, &nota3) == 4;
}

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void welcome() {
    cout << "                              ------------------------------------------------------------\n";
    cout << "                              | Bun Venitin programul Nr.28 Realizat de Susarencu Nikita |\n";
    cout << "                              ------------------------------------------------------------\n";
    cout << "                              (Pentru a iesi din program introduceti: 0)\n\n";
}

int menu() {
    int selection;
    cout << "1. Afisarea pe ecran a informatiei despre toti candidatii\n";
    cout << "2. Afisarea pe ecran a informatiei despre toate notele de examen\n";
    cout << "3. Adaugarea informatiei despre un candidat nou\n";
    cout << "4. Adaugarea informatiei despre notele de examen ale candidatului adaugat la punctul 1\n";
    cout << "5. Stergerea informatiei despre un candidat, al carui nume si prenume sunt introduse de la tastatura\n";
    cout << "6. Stergerea informatiei despre notele de examen ale unui candidat, al carui cod este introdus de la tastatura\n";
    cout << "7. Modificarea informatiei despre un candidat, al carui nume si prenume sunt introduse de la tastatura\n";
    cout << "8. Modificarea informatiei despre notele de examen ale unui candidat, al carui cod este introdus de la tastatura\n";
    cout << "9. Crearea fisierului text Average.txt, care contine informatii despre candidati, cu calcularea mediei\n";
    cout << "10. Afisarea pe ecran, in ordine alfabetica, a tuturor candidatilor care sustin examenul la disciplina la alegere\n";
    cout << "11. Afisarea pe ecran a informatiei despre candidatul cu cea mai mare medie\n";
    cout << "12. Afisarea pe ecran a numarului de restantieri\n";
    cout << "0. Exit program\n";
    cout << "Selectati o optiune de mai sus: ";
    cin >> selection;
    cout << "\n";
    return selection;
}

void addCandidat() {
    ifstream fin("Candidat.txt");
    int maxId = 1000;
    char line[MAX_LINE];
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        if (sscanf(line, "%d", &id) == 1 && id > maxId) {
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
    cout << "Introduceti numele: ";
    cin >> nume;
    cout << "Introduceti prenumele: ";
    cin >> prenume;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Introduceti scoala: ";
    cin.getline(scoala, MAX_FIELD);
    cout << "Introduceti profilul: ";
    cin.getline(profil, MAX_FIELD);
    cout << "Introduceti limba: ";
    cin.getline(limba, MAX_FIELD);
    cout << "Introduceti disciplina: ";
    cin.getline(disciplina, MAX_FIELD);
    ofstream fout("Candidat.txt", ios::app);
    fout << newId << " | " << nume << " " << prenume << " | " << scoala << " | " << profil << " | " << limba << " | " << disciplina << endl;
    fout.close();
    cout << "Candidat adaugat cu ID: " << newId << "\n";
}

void addGrades() {
    int id;
    double nota1, nota2, nota3;
    cout << "Introduceti ID-ul candidatului: ";
    cin >> id;
    cout << "Introduceti nota 1: ";
    cin >> nota1;
    cout << "Introduceti nota 2: ";
    cin >> nota2;
    cout << "Introduceti nota 3: ";
    cin >> nota3;
    ofstream fout("Examen.txt", ios::app);
    fout << id << " " << nota1 << " " << nota2 << " " << nota3 << "\n";
    fout.close();
    cout << "Note adaugate pentru ID: " << id << "\n";
}

void deleteCandidat() {
    char nume[MAX_NAME];
    char prenume[MAX_NAME];
    cout << "Introduceti numele: ";
    cin >> nume;
    cout << "Introduceti prenumele: ";
    cin >> prenume;
    char fullName[MAX_NAME * 2];
    snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);
    ifstream finC("Candidat.txt");
    if (!finC) {
        cout << "Nu se poate deschide fisierul Candidat.txt\n";
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
            if (idToDelete == -1 && strcmp(currentName, fullName) == 0) {
                idToDelete = id;
                continue;
            }
        }
        strncpy(lines[count], line, MAX_LINE - 1);
        lines[count][MAX_LINE - 1] = '\0';
        count++;
    }
    finC.close();
    if (idToDelete == -1) {
        cout << "Candidat nu gasit.\n";
        return;
    }
    ofstream foutC("Candidat.txt");
    for (int i = 0; i < count; i++) {
        foutC << lines[i] << "\n";
    }
    foutC.close();
    ifstream finE("Examen.txt");
    ofstream foutE("tempE.txt");
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
    remove("Examen.txt");
    rename("tempE.txt", "Examen.txt");
    cout << "Candidat sters.\n";
}

void deleteGrades() {
    int id;
    cout << "Introduceti ID-ul candidatului: ";
    cin >> id;
    ifstream fin("Examen.txt");
    if (!fin) {
        cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    char line[MAX_LINE];
    ofstream fout("tempE.txt");
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
    remove("Examen.txt");
    rename("tempE.txt", "Examen.txt");
    if (found) {
        cout << "Note sterse pentru ID: " << id << "\n";
    } else {
        cout << "ID nu gasit.\n";
    }
}

void modifyCandidat() {
    char nume[MAX_NAME];
    char prenume[MAX_NAME];
    cout << "Introduceti numele candidatului de modificat: ";
    cin >> nume;
    cout << "Introduceti prenumele candidatului de modificat: ";
    cin >> prenume;
    char fullName[MAX_NAME * 2];
    snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);
    ifstream fin("Candidat.txt");
    if (!fin) {
        cout << "Nu se poate deschide fisierul Candidat.txt\n";
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
            strcmp(currentName, fullName) == 0) {
            found = true;
            char newNume[MAX_NAME];
            char newPrenume[MAX_NAME];
            char newScoala[MAX_FIELD];
            char newProfil[MAX_FIELD];
            char newLimba[MAX_FIELD];
            char newDisciplina[MAX_FIELD];
            cout << "Introduceti noul nume: ";
            cin >> newNume;
            cout << "Introduceti noul prenume: ";
            cin >> newPrenume;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Introduceti noua scoala: ";
            cin.getline(newScoala, MAX_FIELD);
            cout << "Introduceti noul profil: ";
            cin.getline(newProfil, MAX_FIELD);
            cout << "Introduceti noua limba: ";
            cin.getline(newLimba, MAX_FIELD);
            cout << "Introduceti noua disciplina: ";
            cin.getline(newDisciplina, MAX_FIELD);
            snprintf(lines[count], MAX_LINE, "%d | %s %s | %s | %s | %s | %s",
                          id, newNume, newPrenume, newScoala, newProfil, newLimba, newDisciplina);
            count++;
        } else {
            strncpy(lines[count], line, MAX_LINE - 1);
            lines[count][MAX_LINE - 1] = '\0';
            count++;
        }
    }
    fin.close();
    if (!found) {
        cout << "Candidat nu gasit.\n";
        return;
    }
    ofstream fout("Candidat.txt");
    for (int i = 0; i < count; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
    cout << "Informatii candidat modificate.\n";
}

void modifyGradesById() {
    int id;
    cout << "Introduceti ID-ul candidatului pentru care doriti sa modificati notele: ";
    cin >> id;
    ifstream fin("Examen.txt");
    if (!fin) {
        cout << "Nu se poate deschide fisierul Examen.txt\n";
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
            cout << "Introduceti noua nota 1: ";
            cin >> newNota1;
            cout << "Introduceti noua nota 2: ";
            cin >> newNota2;
            cout << "Introduceti noua nota 3: ";
            cin >> newNota3;
            snprintf(lines[count], MAX_LINE, "%d %.2f %.2f %.2f", id, newNota1, newNota2, newNota3);
            count++;
        } else {
            strncpy(lines[count], line, MAX_LINE - 1);
            lines[count][MAX_LINE - 1] = '\0';
            count++;
        }
    }
    fin.close();
    if (!found) {
        cout << "ID nu gasit.\n";
        return;
    }
    ofstream fout("Examen.txt");
    for (int i = 0; i < count; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
    cout << "Note modificate pentru ID: " << id << "\n";
}

void createAverageFile() {
    ifstream finC("Candidat.txt");
    ifstream finE("Examen.txt");
    if (!finC) {
        cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    if (!finE) {
        cout << "Nu se poate deschide fisierul Examen.txt\n";
        return;
    }
    int ids[MAX_CANDIDATES];
    char candidateLines[MAX_CANDIDATES][MAX_LINE];
    int countC = 0;
    char line[MAX_LINE];
    while (finC.getline(line, MAX_LINE) && countC < MAX_CANDIDATES) {
        if (line[0] == '\0') continue;
        int id;
        if (sscanf(line, "%d", &id) == 1) {
            ids[countC] = id;
            strncpy(candidateLines[countC], line, MAX_LINE - 1);
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
    ofstream fout("Average.txt");
    fout.setf(ios::fixed);
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
            fout << " | Media: " << average << "";
        } else {
            fout << " | Media: N/A";
        }
        fout << "\n";
    }
    fout.close();
    cout << "Fisierul Average.txt a fost creat.\n";
}

void showCandidatesByDiscipline() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    char discipline[MAX_DISC];
    cout << "Introduceti disciplina: ";
    cin.getline(discipline, MAX_DISC);
    char wanted[MAX_DISC];
    trim(wanted, discipline, MAX_DISC);
    toLowerInPlace(wanted);
    ifstream fin("Candidat.txt");
    if (!fin) {
        cout << "Nu se poate deschide fisierul Candidat.txt\n";
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
            if (strcmp(discLower, wanted) == 0) {
                strncpy(matchedLines[count], line, MAX_LINE - 1);
                matchedLines[count][MAX_LINE - 1] = '\0';
                strncpy(matchedNames[count], fullName, MAX_NAME - 1);
                matchedNames[count][MAX_NAME - 1] = '\0';
                count++;
            }
        }
    }
    fin.close();
    if (count == 0) {
        cout << "Nu s-au gasit candidati pentru disciplina " << discipline << ".\n";
        return;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(matchedNames[i], matchedNames[j]) > 0) {
                char tempName[MAX_NAME];
                char tempLine[MAX_LINE];
                strcpy(tempName, matchedNames[i]);
                strcpy(matchedNames[i], matchedNames[j]);
                strcpy(matchedNames[j], tempName);
                strcpy(tempLine, matchedLines[i]);
                strcpy(matchedLines[i], matchedLines[j]);
                strcpy(matchedLines[j], tempLine);
            }
        }
    }
    for (int i = 0; i < count; i++) {
        cout << matchedLines[i] << "\n";
    }
}

void showTopAverageCandidate() {
    ifstream finC("Candidat.txt");
    ifstream finE("Examen.txt");
    if (!finC) {
        cout << "Nu se poate deschide fisierul Candidat.txt\n";
        return;
    }
    if (!finE) {
        cout << "Nu se poate deschide fisierul Examen.txt\n";
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
        cout << "Nu exista date pentru calcularea celor mai mari medii.\n";
        finC.close();
        return;
    }
    bool found = false;
    while (finC.getline(line, MAX_LINE)) {
        if (line[0] == '\0') continue;
        int id;
        if (sscanf(line, "%d", &id) == 1 && id == bestId) {
            cout << "Candidatul cu cea mai mare medie:\n";
            cout << line << "\n";
            cout << fixed << setprecision(2);
            cout << "Media: " << bestAverage << "\n";
            cout.unsetf(ios::fixed);
            found = true;
            break;
        }
    }
    finC.close();
    if (!found) {
        cout << "Candidatul cu cea mai mare medie nu a fost gasit in Candidat.txt.\n";
    }
}

void countRestantieri() {
    ifstream fin("Examen.txt");
    if (!fin) {
        cout << "Nu se poate deschide fisierul Examen.txt\n";
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
    cout << "Numarul de restantieri este: " << count << "\n";
}
