#include "func.h"
#include <iostream>
#include <cstdlib>

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
    std::cout << "Selectati o optiune de mai sus: ";
    std::cin >> selection;
    std::cout << "\n";
    return selection;
}