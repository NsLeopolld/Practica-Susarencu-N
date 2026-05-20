#include "func.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

const int MAX_LINE        = 512;
const int MAX_CANDIDATES  = 512;
const int MAX_FIELD       = 128;
const int MAX_NAME        = 64;
const int MAX_DISC        = 64;

const int W_ID    = 6;
const int W_NAME  = 25;
const int W_SCH   = 26;
const int W_PROF  = 6;
const int W_LANG  = 10;
const int W_DISC  = 12;

void trim(char *out, const char *in, size_t outSize) {
    const char *start = in;
    while (*start && isspace((unsigned char)*start)) start++;
    const char *end = in + strlen(in);
    while (end > start && isspace((unsigned char)*(end - 1))) end--;
    size_t len = (end > start) ? (size_t)(end - start) : 0;
    if (len >= outSize) len = outSize - 1;
    memcpy(out, start, len);
    out[len] = '\0';
}

void toLowerInPlace(char *text) {
    for (size_t i = 0; text[i]; i++)
        text[i] = static_cast<char>(tolower((unsigned char)text[i]));
}

bool isNonEmpty(const char *s) {
    while (*s) { if (!isspace((unsigned char)*s)) return true; s++; }
    return false;
}

void recoverCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool readDouble(double &val, double lo, double hi, const char *prompt) {
    for (int attempt = 0; attempt < 3; attempt++) {
        cout << prompt;
        if (!(cin >> val)) {
            recoverCin();
            cout << "  [Eroare] Valoare numerica invalida. Incercati din nou.\n";
            continue;
        }
        if (val < lo || val > hi) {
            cout << "  [Eroare] Valoarea trebuie sa fie intre " << lo
                 << " si " << hi << ". Incercati din nou.\n";
            continue;
        }
        return true;
    }
    cout << "  [Eroare] Prea multe incercari esuate. Operatie anulata.\n";
    return false;
}

bool readPositiveInt(int &val, const char *prompt) {
    for (int attempt = 0; attempt < 3; attempt++) {
        cout << prompt;
        if (!(cin >> val)) {
            recoverCin();
            cout << "  [Eroare] Numar intreg invalid. Incercati din nou.\n";
            continue;
        }
        if (val <= 0) {
            cout << "  [Eroare] Valoarea trebuie sa fie pozitiva. Incercati din nou.\n";
            continue;
        }
        return true;
    }
    cout << "  [Eroare] Prea multe incercari esuate. Operatie anulata.\n";
    return false;
}

bool readNonEmptyLine(char *buf, size_t size, const char *prompt) {
    while (true) {
        cout << prompt;
        if (!cin.getline(buf, size)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [Eroare] Input invalid. Incercati din nou.\n";
            continue;
        }
        if (!isNonEmpty(buf)) {
            cout << "  [Eroare] Campul nu poate fi gol. Incercati din nou.\n";
            continue;
        }
        return true;
    }
}

bool parseCandidateLine(const char *line, int &id, char *fullName, char *scoala,
                        char *profil, char *limba, char *disciplina) {
    char buffer[MAX_LINE];
    strncpy(buffer, line, MAX_LINE - 1);
    buffer[MAX_LINE - 1] = '\0';

    char *token = strtok(buffer, "|");
    if (!token) return false;
    char tmp[MAX_FIELD];
    trim(tmp, token, MAX_FIELD);
    if (sscanf(tmp, "%d", &id) != 1) return false;

    token = strtok(NULL, "|"); if (!token) return false; trim(fullName,   token, MAX_NAME);
    token = strtok(NULL, "|"); if (!token) return false; trim(scoala,     token, MAX_FIELD);
    token = strtok(NULL, "|"); if (!token) return false; trim(profil,     token, MAX_FIELD);
    token = strtok(NULL, "|"); if (!token) return false; trim(limba,      token, MAX_FIELD);
    token = strtok(NULL, "|"); if (!token) return false; trim(disciplina, token, MAX_FIELD);
    return true;
}

bool parseExamLine(const char *line, int &id, double &nota1, double &nota2, double &nota3, double &nota4) {
    return sscanf(line, "%d %lf %lf %lf %lf", &id, &nota1, &nota2, &nota3, &nota4) == 5;
}

static string cell(const char *s, int width) {
    string t(s);
    if ((int)t.size() > width) t = t.substr(0, width - 1) + "~";
    return t;
}

static void printCandidatSeparator() {
    cout << "+"  << setfill('-') << setw(W_ID   +2) << "-"
         << "+"  << setw(W_NAME +2) << "-"
         << "+"  << setw(W_SCH  +2) << "-"
         << "+"  << setw(W_PROF +2) << "-"
         << "+"  << setw(W_LANG +2) << "-"
         << "+"  << setw(W_DISC +2) << "-"
         << "+"  << setfill(' ') << "\n";
}

static void printCandidatRow(const char *id, const char *name, const char *scoala,
                              const char *profil, const char *limba, const char *disc) {
    cout << "| " << left  << setw(W_ID)   << cell(id,     W_ID)
         << " | " << setw(W_NAME) << cell(name,   W_NAME)
         << " | " << setw(W_SCH)  << cell(scoala, W_SCH)
         << " | " << setw(W_PROF) << cell(profil, W_PROF)
         << " | " << setw(W_LANG) << cell(limba,  W_LANG)
         << " | " << setw(W_DISC) << cell(disc,   W_DISC)
         << " |\n";
}

void printCandidatTable(ifstream &fin) {
    printCandidatSeparator();
    printCandidatRow("ID", "Nume Prenume", "Scoala", "Profil", "Limba", "Disciplina");
    printCandidatSeparator();

    char line[MAX_LINE];
    bool any = false;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id;
        char name[MAX_NAME], scoala[MAX_FIELD], profil[MAX_FIELD],
             limba[MAX_FIELD], disc[MAX_FIELD];
        if (parseCandidateLine(line, id, name, scoala, profil, limba, disc)) {
            char idStr[16];
            snprintf(idStr, sizeof(idStr), "%d", id);
            printCandidatRow(idStr, name, scoala, profil, limba, disc);
            any = true;
        }
    }
    if (!any) cout << "|" << setw(W_ID+W_NAME+W_SCH+W_PROF+W_LANG+W_DISC+13)
                   << left << " (fisier gol)" << "|\n";
    printCandidatSeparator();
}

static void printExamSeparator() {
    cout << "+" << setfill('-') << setw(W_ID+2) << "-"
         << "+" << setw(11) << "-"
         << "+" << setw(11) << "-"
         << "+" << setw(11) << "-"
         << "+" << setw(11) << "-"
         << "+" << setfill(' ') << "\n";
}

void printExamTable(ifstream &fin) {
    printExamSeparator();
    cout << "| " << left << setw(W_ID) << "ID"
         << " | " << setw(9) << "Nota 1"
         << " | " << setw(9) << "Nota 2"
         << " | " << setw(9) << "Nota 3"
         << " | " << setw(9) << "Nota 4"
         << " |\n";
    printExamSeparator();

    char line[MAX_LINE];
    bool any = false;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id;
        double n1, n2, n3, n4;
        if (parseExamLine(line, id, n1, n2, n3, n4)) {
            cout << fixed << setprecision(2);
            cout << "| " << left  << setw(W_ID) << id
                 << " | " << right << setw(9)  << n1
                 << " | " << setw(9) << n2
                 << " | " << setw(9) << n3
                 << " | " << setw(9) << n4
                 << " |\n";
            any = true;
        }
    }
    if (!any) cout << "|" << setw(W_ID+47) << left << " (fisier gol)" << "|\n";
    printExamSeparator();
    cout.unsetf(ios::fixed);
}


bool candidateExists(int id) {
    ifstream f("Candidat.txt");
    if (!f) return false;
    char line[MAX_LINE];
    while (f.getline(line, MAX_LINE)) {
        int cid;
        if (sscanf(line, "%d", &cid) == 1 && cid == id) return true;
    }
    return false;
}

bool gradeExists(int id) {
    ifstream f("Examen.txt");
    if (!f) return false;
    char line[MAX_LINE];
    int cid; double n1, n2, n3, n4;
    while (f.getline(line, MAX_LINE))
        if (parseExamLine(line, cid, n1, n2, n3, n4) && cid == id) return true;
    return false;
}

int getMaxCandidateId() {
    ifstream fin("Candidat.txt");
    int maxId = 1000;
    char line[MAX_LINE];
    while (fin.getline(line, MAX_LINE)) {
        int id;
        if (sscanf(line, "%d", &id) == 1 && id > maxId) maxId = id;
    }
    return maxId;
}

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void welcome() {
    cout << "\n";
    cout << "  +-----------------------------------------------------------+\n";
    cout << "  |   Bun Venit in programul Nr.28 - Realizat de Susarencu N  |\n";
    cout << "  +-----------------------------------------------------------+\n";
    cout << "  (Pentru a iesi din program introduceti: 0)\n\n";
}

int menu() {
    cout << " 1.  Afisarea candidatilor (tabel)\n";
    cout << " 2.  Afisarea notelor de examen (tabel)\n";
    cout << " 3.  Adaugare candidat nou\n";
    cout << " 4.  Adaugare note examen pentru candidat\n";
    cout << " 5.  Stergere candidat dupa Nume Prenume\n";
    cout << " 6.  Stergere note dupa ID\n";
    cout << " 7.  Modificare candidat dupa Nume Prenume\n";
    cout << " 8.  Modificare note dupa ID\n";
    cout << " 9.  Creare fisier Average.txt\n";
    cout << "10.  Candidati dupa disciplina (alfabetic)\n";
    cout << "11.  Candidatul cu cea mai mare medie\n";
    cout << "12.  Numarul de restantieri\n";
    cout << " 0.  Exit\n";
    cout << "\nSelectati optiunea: ";

    int selection;
    if (!(cin >> selection)) {
        recoverCin();
        return -1;
    }
    cout << "\n";
    return selection;
}

void addCandidat() {
    int newId = getMaxCandidateId() + 1;

    char nume[MAX_NAME], prenume[MAX_NAME];
    char scoala[MAX_FIELD], profil[MAX_FIELD], limba[MAX_FIELD], disciplina[MAX_FIELD];

    cout << "Introduceti numele: ";
    if (!(cin >> nume) || !isNonEmpty(nume)) {
        recoverCin();
        cout << "  [Eroare] Numele nu poate fi gol.\n"; return;
    }
    cout << "Introduceti prenumele: ";
    if (!(cin >> prenume) || !isNonEmpty(prenume)) {
        recoverCin();
        cout << "  [Eroare] Prenumele nu poate fi gol.\n"; return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Introduceti scoala: ";
    cin.getline(scoala, MAX_FIELD);
    if (!isNonEmpty(scoala)) { cout << "  [Eroare] Scoala nu poate fi goala.\n"; return; }

    cout << "Introduceti profilul (Real/Uman): ";
    cin.getline(profil, MAX_FIELD);
    {
        char tmp[MAX_FIELD]; trim(tmp, profil, MAX_FIELD); toLowerInPlace(tmp);
        if (strcmp(tmp, "real") != 0 && strcmp(tmp, "uman") != 0) {
            cout << "  [Eroare] Profilul trebuie sa fie 'Real' sau 'Uman'.\n"; return;
        }
    }

    cout << "Introduceti limba: ";
    cin.getline(limba, MAX_FIELD);
    if (!isNonEmpty(limba)) { cout << "  [Eroare] Limba nu poate fi goala.\n"; return; }

    cout << "Introduceti disciplina: ";
    cin.getline(disciplina, MAX_FIELD);
    if (!isNonEmpty(disciplina)) { cout << "  [Eroare] Disciplina nu poate fi goala.\n"; return; }

    ofstream fout("Candidat.txt", ios::app);
    if (!fout) { cout << "  [Eroare] Nu se poate scrie in Candidat.txt\n"; return; }
    ostringstream oss;
    oss << newId
        << " | " << left << setw(23) << (string(nume) + " " + prenume)
        << " | " << setw(23) << scoala
        << " | " << setw(4) << profil
        << " | " << setw(8) << limba
        << " | " << disciplina;
    fout << oss.str() << "\n";
    fout.close();
    cout << "  Candidat adaugat cu ID: " << newId << "\n";
}

void addGrades() {
    int id;
    if (!readPositiveInt(id, "Introduceti ID-ul candidatului: ")) return;

    if (!candidateExists(id)) {
        cout << "  [Eroare] Nu exista un candidat cu ID " << id << " in Candidat.txt.\n";
        return;
    }
    if (gradeExists(id)) {
        cout << "  [Eroare] Note deja existente pentru ID " << id
             << ". Folositi optiunea 8 pentru modificare.\n";
        return;
    }

    double nota1, nota2, nota3, nota4;
    if (!readDouble(nota1, 1.0, 10.0, "Introduceti nota 1 (1-10): ")) return;
    if (!readDouble(nota2, 1.0, 10.0, "Introduceti nota 2 (1-10): ")) return;
    if (!readDouble(nota3, 1.0, 10.0, "Introduceti nota 3 (1-10): ")) return;
    if (!readDouble(nota4, 1.0, 10.0, "Introduceti nota 4 (1-10): ")) return;

    ofstream fout("Examen.txt", ios::app);
    if (!fout) { cout << "  [Eroare] Nu se poate scrie in Examen.txt\n"; return; }
    fout << fixed << setprecision(1)
         << id << " " << nota1 << " " << nota2 << " " << nota3 << " " << nota4 << endl;
    fout.close();
    cout << "  Note adaugate pentru ID: " << id << "\n";
}

void deleteCandidat() {
    char nume[MAX_NAME], prenume[MAX_NAME];
    cout << "Introduceti numele: ";
    if (!(cin >> nume)) { recoverCin(); cout << "  [Eroare] Input invalid.\n"; return; }
    cout << "Introduceti prenumele: ";
    if (!(cin >> prenume)) { recoverCin(); cout << "  [Eroare] Input invalid.\n"; return; }

    char fullName[MAX_NAME * 2];
    snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);

    ifstream finC("Candidat.txt");
    if (!finC) { cout << "  [Eroare] Nu se poate deschide Candidat.txt\n"; return; }

    char lines[MAX_CANDIDATES][MAX_LINE];
    int  count = 0, idToDelete = -1;
    char line[MAX_LINE];

    while (finC.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; char cName[MAX_NAME], scoala[MAX_FIELD], profil[MAX_FIELD], limba[MAX_FIELD], disc[MAX_FIELD];
        if (parseCandidateLine(line, id, cName, scoala, profil, limba, disc) &&
            idToDelete == -1 && strcmp(cName, fullName) == 0) {
            idToDelete = id;
            continue;
        }
        strncpy(lines[count], line, MAX_LINE - 1);
        lines[count++][MAX_LINE - 1] = '\0';
    }
    finC.close();

    if (idToDelete == -1) { cout << "  [Eroare] Candidat '" << fullName << "' nu a fost gasit.\n"; return; }

    ofstream foutC("Candidat.txt");
    if (!foutC) { cout << "  [Eroare] Nu se poate scrie in Candidat.txt\n"; return; }
    for (int i = 0; i < count; i++) foutC << lines[i] << "\n";
    foutC.close();

    ifstream finE("Examen.txt");
    ofstream foutE("tempE.txt");
    if (finE && foutE) {
        while (finE.getline(line, MAX_LINE)) {
            if (line[0] == '\0' || line[0] == '\r') continue;
            int cid; double g1, g2, g3, g4;
            if (!parseExamLine(line, cid, g1, g2, g3, g4) || cid != idToDelete)
                foutE << line << "\n";
        }
        finE.close(); foutE.close();
        remove("Examen.txt");
        rename("tempE.txt", "Examen.txt");
    }
    cout << "  Candidat '" << fullName << "' (ID " << idToDelete << ") sters.\n";
}

void deleteGrades() {
    int id;
    if (!readPositiveInt(id, "Introduceti ID-ul candidatului: ")) return;

    ifstream fin("Examen.txt");
    if (!fin) { cout << "  [Eroare] Nu se poate deschide Examen.txt\n"; return; }

    char line[MAX_LINE];
    ofstream fout("tempE.txt");
    if (!fout) { cout << "  [Eroare] Nu se poate crea fisier temporar.\n"; return; }

    bool found = false;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int cid; double n1, n2, n3, n4;
        if (parseExamLine(line, cid, n1, n2, n3, n4) && cid == id) { found = true; continue; }
        fout << line << "\n";
    }
    fin.close(); fout.close();
    remove("Examen.txt");
    rename("tempE.txt", "Examen.txt");

    if (found) cout << "  Note sterse pentru ID: " << id << "\n";
    else        cout << "  [Eroare] ID " << id << " nu a fost gasit in Examen.txt\n";
}

void modifyCandidat() {
    char nume[MAX_NAME], prenume[MAX_NAME];
    cout << "Introduceti numele candidatului de modificat: ";
    if (!(cin >> nume)) { recoverCin(); cout << "  [Eroare] Input invalid.\n"; return; }
    cout << "Introduceti prenumele candidatului de modificat: ";
    if (!(cin >> prenume)) { recoverCin(); cout << "  [Eroare] Input invalid.\n"; return; }

    char fullName[MAX_NAME * 2];
    snprintf(fullName, sizeof(fullName), "%s %s", nume, prenume);

    ifstream fin("Candidat.txt");
    if (!fin) { cout << "  [Eroare] Nu se poate deschide Candidat.txt\n"; return; }

    char lines[MAX_CANDIDATES][MAX_LINE];
    int count = 0; bool found = false;
    char line[MAX_LINE];
    int foundId = -1;
    char oldName[MAX_NAME], oldScoala[MAX_FIELD], oldProfil[MAX_FIELD], oldLimba[MAX_FIELD], oldDisc[MAX_FIELD];

    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; char cName[MAX_NAME], scoala[MAX_FIELD], profil[MAX_FIELD], limba[MAX_FIELD], disc[MAX_FIELD];
        if (!found && parseCandidateLine(line, id, cName, scoala, profil, limba, disc) &&
            strcmp(cName, fullName) == 0) {
            found = true;
            foundId = id;
            strcpy(oldName, cName);
            strcpy(oldScoala, scoala);
            strcpy(oldProfil, profil);
            strcpy(oldLimba, limba);
            strcpy(oldDisc, disc);
        }
        strncpy(lines[count], line, MAX_LINE - 1);
        lines[count][MAX_LINE - 1] = '\0';
        count++;
    }
    fin.close();

    if (!found) { cout << "  [Eroare] Candidat '" << fullName << "' nu a fost gasit.\n"; return; }

    cout << "\n  Campuri actuale:\n";
    cout << "    Nume Prenume: " << oldName << "\n";
    cout << "    Scoala: " << oldScoala << "\n";
    cout << "    Profil: " << oldProfil << "\n";
    cout << "    Limba: " << oldLimba << "\n";
    cout << "    Disciplina: " << oldDisc << "\n\n";
    cout << "Alegeti campul de modificat:\n";
    cout << " 1. Nume si prenume\n";
    cout << " 2. Scoala\n";
    cout << " 3. Profil\n";
    cout << " 4. Limba\n";
    cout << " 5. Disciplina\n";
    cout << " 6. Toate campurile\n";
    cout << " 0. Renunta\n";
    cout << "Selectati optiunea: ";

    int option;
    if (!(cin >> option)) {
        recoverCin();
        cout << "  [Eroare] Optiune invalida.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    char newName[MAX_NAME], newScoala[MAX_FIELD], newProfil[MAX_FIELD], newLimba[MAX_FIELD], newDisc[MAX_FIELD];
    strcpy(newName, oldName);
    strcpy(newScoala, oldScoala);
    strcpy(newProfil, oldProfil);
    strcpy(newLimba, oldLimba);
    strcpy(newDisc, oldDisc);

    switch (option) {
        case 0:
            cout << "  Operatie anulata.\n";
            return;
        case 1: {
            char nNume[MAX_NAME], nPrenume[MAX_NAME];
            cout << "Introduceti noul nume: ";
            if (!(cin >> nNume) || !isNonEmpty(nNume)) {
                recoverCin(); cout << "  [Eroare] Nume invalid.\n"; return;
            }
            cout << "Introduceti noul prenume: ";
            if (!(cin >> nPrenume) || !isNonEmpty(nPrenume)) {
                recoverCin(); cout << "  [Eroare] Prenume invalid.\n"; return;
            }
            snprintf(newName, sizeof(newName), "%s %s", nNume, nPrenume);
            break;
        }
        case 2:
            if (!readNonEmptyLine(newScoala, MAX_FIELD, "Introduceti noua scoala: ")) return;
            break;
        case 3: {
            if (!readNonEmptyLine(newProfil, MAX_FIELD, "Introduceti noul profil (Real/Uman): ")) return;
            char tmp[MAX_FIELD]; trim(tmp, newProfil, MAX_FIELD); toLowerInPlace(tmp);
            if (strcmp(tmp, "real") != 0 && strcmp(tmp, "uman") != 0) {
                cout << "  [Eroare] Profilul trebuie sa fie 'Real' sau 'Uman'.\n";
                return;
            }
            break;
        }
        case 4:
            if (!readNonEmptyLine(newLimba, MAX_FIELD, "Introduceti noua limba: ")) return;
            break;
        case 5:
            if (!readNonEmptyLine(newDisc, MAX_FIELD, "Introduceti noua disciplina: ")) return;
            break;
        case 6: {
            char nNume[MAX_NAME], nPrenume[MAX_NAME];
            cout << "Introduceti noul nume: ";
            if (!(cin >> nNume) || !isNonEmpty(nNume)) {
                recoverCin(); cout << "  [Eroare] Nume invalid.\n"; return;
            }
            cout << "Introduceti noul prenume: ";
            if (!(cin >> nPrenume) || !isNonEmpty(nPrenume)) {
                recoverCin(); cout << "  [Eroare] Prenume invalid.\n"; return;
            }
            snprintf(newName, sizeof(newName), "%s %s", nNume, nPrenume);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (!readNonEmptyLine(newScoala, MAX_FIELD, "Introduceti noua scoala: ")) return;
            if (!readNonEmptyLine(newProfil, MAX_FIELD, "Introduceti noul profil (Real/Uman): ")) return;
            {
                char tmp[MAX_FIELD]; trim(tmp, newProfil, MAX_FIELD); toLowerInPlace(tmp);
                if (strcmp(tmp, "real") != 0 && strcmp(tmp, "uman") != 0) {
                    cout << "  [Eroare] Profilul trebuie sa fie 'Real' sau 'Uman'.\n";
                    return;
                }
            }
            if (!readNonEmptyLine(newLimba, MAX_FIELD, "Introduceti noua limba: ")) return;
            if (!readNonEmptyLine(newDisc, MAX_FIELD, "Introduceti noua disciplina: ")) return;
            break;
        }
        default:
            cout << "  [Eroare] Optiune invalida.\n";
            return;
    }

    ostringstream oss;
    oss << foundId
        << " | " << left << setw(23) << newName
        << " | " << setw(23) << newScoala
        << " | " << setw(4) << newProfil
        << " | " << setw(8) << newLimba
        << " | " << newDisc;
    string buf = oss.str();

    bool replaced = false;
    for (int i = 0; i < count; i++) {
        int id; char cName[MAX_NAME], scoala[MAX_FIELD], profil[MAX_FIELD], limba[MAX_FIELD], disc[MAX_FIELD];
        if (!replaced && parseCandidateLine(lines[i], id, cName, scoala, profil, limba, disc) &&
            id == foundId && strcmp(cName, fullName) == 0) {
            strncpy(lines[i], buf.c_str(), MAX_LINE - 1);
            lines[i][MAX_LINE - 1] = '\0';
            replaced = true;
        }
    }

    if (!replaced) {
        cout << "  [Eroare] Nu s-a putut actualiza inregistrarea.\n";
        return;
    }

    ofstream fout("Candidat.txt");
    if (!fout) { cout << "  [Eroare] Nu se poate scrie in Candidat.txt\n"; return; }
    for (int i = 0; i < count; i++) fout << lines[i] << "\n";
    fout.close();
    cout << "  Informatii candidat modificate.\n";
}

void modifyGradesById() {
    int id;
    if (!readPositiveInt(id, "Introduceti ID-ul candidatului: ")) return;
    if (!candidateExists(id)) {
        cout << "  [Eroare] Nu exista candidat cu ID " << id << " in Candidat.txt.\n"; return;
    }
    if (!gradeExists(id)) {
        cout << "  [Eroare] Nu exista note pentru ID " << id
             << ". Folositi optiunea 4 pentru adaugare.\n"; return;
    }

    ifstream fin("Examen.txt");
    if (!fin) { cout << "  [Eroare] Nu se poate deschide Examen.txt\n"; return; }

    char lines[MAX_CANDIDATES][MAX_LINE];
    int  count = 0;
    int  foundIndex = -1;
    char line[MAX_LINE];
    double currentN1 = 0.0, currentN2 = 0.0, currentN3 = 0.0, currentN4 = 0.0;

    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int cid; double n1, n2, n3, n4;
        if (parseExamLine(line, cid, n1, n2, n3, n4)) {
            if (cid == id && foundIndex == -1) {
                foundIndex = count;
                currentN1 = n1;
                currentN2 = n2;
                currentN3 = n3;
                currentN4 = n4;
            }
        }
        strncpy(lines[count], line, MAX_LINE - 1);
        lines[count][MAX_LINE - 1] = '\0';
        count++;
    }
    fin.close();

    if (foundIndex == -1) {
        cout << "  [Eroare] Nu s-au gasit note pentru ID " << id << ".\n";
        return;
    }

    cout << "\n  Note curente pentru ID " << id << ": " << fixed << setprecision(2)
         << currentN1 << ", " << currentN2 << ", " << currentN3 << ", " << currentN4 << "\n";
    cout.unsetf(ios::fixed);
    cout << "Alegeti ce nota doriti sa modificati:\n";
    cout << " 1. Nota 1\n";
    cout << " 2. Nota 2\n";
    cout << " 3. Nota 3\n";
    cout << " 4. Nota 4\n";
    cout << " 5. Toate notele\n";
    cout << " 0. Renunta\n";
    cout << "Selectati optiunea: ";

    int option;
    if (!(cin >> option)) {
        recoverCin();
        cout << "  [Eroare] Optiune invalida.\n";
        return;
    }

    double nn1 = currentN1, nn2 = currentN2, nn3 = currentN3, nn4 = currentN4;
    switch (option) {
        case 0:
            cout << "  Operatie anulata.\n";
            return;
        case 1:
            if (!readDouble(nn1, 1.0, 10.0, "Introduceti noua nota 1 (1-10): ")) return;
            break;
        case 2:
            if (!readDouble(nn2, 1.0, 10.0, "Introduceti noua nota 2 (1-10): ")) return;
            break;
        case 3:
            if (!readDouble(nn3, 1.0, 10.0, "Introduceti noua nota 3 (1-10): ")) return;
            break;
        case 4:
            if (!readDouble(nn4, 1.0, 10.0, "Introduceti noua nota 4 (1-10): ")) return;
            break;
        case 5:
            if (!readDouble(nn1, 1.0, 10.0, "Introduceti noua nota 1 (1-10): ")) return;
            if (!readDouble(nn2, 1.0, 10.0, "Introduceti noua nota 2 (1-10): ")) return;
            if (!readDouble(nn3, 1.0, 10.0, "Introduceti noua nota 3 (1-10): ")) return;
            if (!readDouble(nn4, 1.0, 10.0, "Introduceti noua nota 4 (1-10): ")) return;
            break;
        default:
            cout << "  [Eroare] Optiune invalida.\n";
            return;
    }

    snprintf(lines[foundIndex], MAX_LINE, "%d %.2f %.2f %.2f %.2f", id, nn1, nn2, nn3, nn4);

    ofstream fout("Examen.txt");
    if (!fout) { cout << "  [Eroare] Nu se poate scrie in Examen.txt\n"; return; }
    for (int i = 0; i < count; i++) fout << lines[i] << "\n";
    fout.close();
    cout << "  Note modificate pentru ID: " << id << "\n";
}

void createAverageFile() {
    ifstream finC("Candidat.txt");
    ifstream finE("Examen.txt");
    if (!finC) { cout << "  [Eroare] Nu se poate deschide Candidat.txt\n"; return; }
    if (!finE) { cout << "  [Eroare] Nu se poate deschide Examen.txt\n";   return; }

    int    ids[MAX_CANDIDATES];
    char   candidateLines[MAX_CANDIDATES][MAX_LINE];
    int    countC = 0;
    char   line[MAX_LINE];
    while (finC.getline(line, MAX_LINE) && countC < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id;
        if (sscanf(line, "%d", &id) == 1) {
            ids[countC] = id;
            strncpy(candidateLines[countC], line, MAX_LINE - 1);
            candidateLines[countC++][MAX_LINE - 1] = '\0';
        }
    }
    finC.close();

    int    examIds[MAX_CANDIDATES];
    double n1[MAX_CANDIDATES], n2[MAX_CANDIDATES], n3[MAX_CANDIDATES], n4[MAX_CANDIDATES];
    int    countE = 0;
    while (finE.getline(line, MAX_LINE) && countE < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        if (parseExamLine(line, examIds[countE], n1[countE], n2[countE], n3[countE], n4[countE]))
            countE++;
    }
    finE.close();

    ofstream fout("Average.txt");
    if (!fout) { cout << "  [Eroare] Nu se poate crea Average.txt\n"; return; }
    fout << fixed << setprecision(2);
    for (int i = 0; i < countC; i++) {
        int id = ids[i];
        fout << candidateLines[i];
        int fi = -1;
        for (int j = 0; j < countE; j++) if (examIds[j] == id) { fi = j; break; }
        if (fi >= 0)
            fout << " | Media: " << (n1[fi] + n2[fi] + n3[fi] + n4[fi]) / 4.0;
        else
            fout << " | Media: N/A";
        fout << "\n";
    }
    fout.close();
    cout << "  Fisierul Average.txt a fost creat (" << countC << " inregistrari).\n";
}

void showCandidatesByDiscipline() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    char discipline[MAX_DISC];
    cout << "Introduceti disciplina: ";
    cin.getline(discipline, MAX_DISC);
    if (!isNonEmpty(discipline)) { cout << "  [Eroare] Disciplina nu poate fi goala.\n"; return; }

    char wanted[MAX_DISC];
    trim(wanted, discipline, MAX_DISC);
    toLowerInPlace(wanted);

    ifstream fin("Candidat.txt");
    if (!fin) { cout << "  [Eroare] Nu se poate deschide Candidat.txt\n"; return; }

    char matchedLines[MAX_CANDIDATES][MAX_LINE];
    char matchedNames[MAX_CANDIDATES][MAX_NAME];
    int  count = 0;
    char line[MAX_LINE];

    while (fin.getline(line, MAX_LINE) && count < MAX_CANDIDATES) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; char fullName[MAX_NAME], scoala[MAX_FIELD], profil[MAX_FIELD],
                     limba[MAX_FIELD], disc[MAX_FIELD];
        if (parseCandidateLine(line, id, fullName, scoala, profil, limba, disc)) {
            char dl[MAX_FIELD]; trim(dl, disc, MAX_FIELD); toLowerInPlace(dl);
            if (strcmp(dl, wanted) == 0) {
                strncpy(matchedLines[count], line, MAX_LINE - 1);
                matchedLines[count][MAX_LINE - 1] = '\0';
                strncpy(matchedNames[count], fullName, MAX_NAME - 1);
                matchedNames[count++][MAX_NAME - 1] = '\0';
            }
        }
    }
    fin.close();

    if (count == 0) { cout << "  Nu s-au gasit candidati pentru disciplina '" << discipline << "'.\n"; return; }

    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (strcmp(matchedNames[i], matchedNames[j]) > 0) {
                char t1[MAX_NAME], t2[MAX_LINE];
                strcpy(t1, matchedNames[i]); strcpy(matchedNames[i], matchedNames[j]); strcpy(matchedNames[j], t1);
                strcpy(t2, matchedLines[i]); strcpy(matchedLines[i], matchedLines[j]); strcpy(matchedLines[j], t2);
            }

    cout << "  Candidati la disciplina '" << discipline << "' (" << count << "):\n";
    printCandidatSeparator();
    printCandidatRow("ID","Nume Prenume","Scoala","Profil","Limba","Disciplina");
    printCandidatSeparator();
    for (int i = 0; i < count; i++) {
        int id; char name[MAX_NAME], sc[MAX_FIELD], pr[MAX_FIELD], lb[MAX_FIELD], di[MAX_FIELD];
        if (parseCandidateLine(matchedLines[i], id, name, sc, pr, lb, di)) {
            char idStr[16]; snprintf(idStr, sizeof(idStr), "%d", id);
            printCandidatRow(idStr, name, sc, pr, lb, di);
        }
    }
    printCandidatSeparator();
}

void showTopAverageCandidate() {
    ifstream finE("Examen.txt");
    if (!finE) { cout << "  [Eroare] Nu se poate deschide Examen.txt\n"; return; }

    int    bestId = -1;
    double bestAvg = -1.0;
    char   line[MAX_LINE];
    while (finE.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; double n1, n2, n3, n4;
        if (parseExamLine(line, id, n1, n2, n3, n4)) {
            double avg = (n1 + n2 + n3 + n4) / 4.0;
            if (avg > bestAvg) { bestAvg = avg; bestId = id; }
        }
    }
    finE.close();

    if (bestId == -1) { cout << "  [Eroare] Nu exista date de examen.\n"; return; }

    ifstream finC("Candidat.txt");
    if (!finC) { cout << "  [Eroare] Nu se poate deschide Candidat.txt\n"; return; }

    bool found = false;
    while (finC.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; char name[MAX_NAME], sc[MAX_FIELD], pr[MAX_FIELD], lb[MAX_FIELD], di[MAX_FIELD];
        if (parseCandidateLine(line, id, name, sc, pr, lb, di) && id == bestId) {
            cout << "  Candidatul cu cea mai mare medie:\n";
            printCandidatSeparator();
            printCandidatRow("ID","Nume Prenume","Scoala","Profil","Limba","Disciplina");
            printCandidatSeparator();
            char idStr[16]; snprintf(idStr, sizeof(idStr), "%d", id);
            printCandidatRow(idStr, name, sc, pr, lb, di);
            printCandidatSeparator();
            cout << fixed << setprecision(2) << "  Media: " << bestAvg << "\n";
            cout.unsetf(ios::fixed);
            found = true; break;
        }
    }
    finC.close();
    if (!found)
        cout << "  [Eroare] Candidatul cu ID " << bestId << " nu a fost gasit in Candidat.txt.\n";
}

void countRestantieri() {
    ifstream fin("Examen.txt");
    if (!fin) { cout << "  [Eroare] Nu se poate deschide Examen.txt\n"; return; }

    char line[MAX_LINE];
    int  count = 0;
    while (fin.getline(line, MAX_LINE)) {
        if (line[0] == '\0' || line[0] == '\r') continue;
        int id; double n1, n2, n3, n4;
        if (parseExamLine(line, id, n1, n2, n3, n4))
            if (n1 < 5.0 || n2 < 5.0 || n3 < 5.0 || n4 < 5.0) count++;
    }
    fin.close();
    cout << "  Numarul de restantieri (cel putin o nota < 5): " << count << "\n";
}
