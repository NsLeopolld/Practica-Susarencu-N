#ifndef FUNC_H
#define FUNC_H

#include <fstream>
void ClearScreen();
void welcome();
int  menu();
void printCandidatTable(std::ifstream &fin);
void printExamTable(std::ifstream &fin);
void addCandidat();
void addGrades();
void deleteCandidat();
void deleteGrades();
void modifyCandidat();
void modifyGradesById();
void createAverageFile();
void showCandidatesByDiscipline();
void showTopAverageCandidate();
void countRestantieri();

#endif
