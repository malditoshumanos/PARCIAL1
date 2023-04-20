#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <fstream>
void guardarMateria(char mode);
bool horario(char mode, int row = 0, int col = 0, const char* code = nullptr);
void programarMateria(std::fstream& materias, std::fstream& htiMaterias);

#endif // FUNCIONES_H
