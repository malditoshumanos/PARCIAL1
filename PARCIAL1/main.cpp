#include "funciones.h"

int contMateriasO = 1; // Contador de materias no académicas

int main()
{
    // Borrar el contenido de materias.txt y htiMaterias.txt
    std::fstream bMaterias("materias.txt", std::fstream::out | std::fstream::trunc);
    if(!bMaterias.is_open()){
        std::cout << "No se pudo abrir el archivo materias.txt\n";
    }
    bMaterias.close();    
    std::fstream cMaterias("htiMaterias.txt", std::fstream::out | std::fstream::trunc);
    if(!cMaterias.is_open()){
        std::cout << "No se pudo abrir el archivo htiMaterias.txt\n";
    }
    cMaterias.close();





    horario('V'); // Crear horario vacío


    std::cout << "***PROGRAMA PARA ORGANIZAR EL TIEMPO***" << '\n';
    char principalMenu;
    while(principalMenu != 'X'){
        std::cout << "Ingrese una opcion: \n"
                "   I: Introducir materias\n"
                "   O: Mostras horario con sugerencias de tiempos de estudio\n"
                "   X: salir\n"
                "   >> ";
        std::cin >> principalMenu;
        if(principalMenu != 'I' && principalMenu != 'C' && principalMenu != 'O' && principalMenu != 'X') std::cout << "Ingrese una opcion valida!\n";
        switch(principalMenu){
            case 'I':{
                char IMenu;

                while(IMenu != 'V'){
                    std::cout << "   Desea guardar una actividad academica (A) o de otro tipo (O)?\n"
                                 "   (V para volver al menu principal)"
                                 "   >> ";
                    std::cin >> IMenu;
                    if(IMenu == 'A') guardarMateria('A');
                    if(IMenu == 'O') guardarMateria('O');                   
                }

                //Procesar las materias guardadas
                std::fstream htiMaterias("htiMaterias.txt", std::fstream::in | std::fstream::out);
                if(!htiMaterias.is_open()){
                    std::cout << "No se pudo abrir el archivo htiMaterias.txt\n";
                }

                std::fstream materias("materias.txt", std::fstream::in);
                if(!materias.is_open()){
                    std::cout << "No se pudo abrir el archivo materias.txt\n";
                }

                std::fstream htiHorario("htiHorario.txt", std::fstream::out);
                if(!htiHorario.is_open()){
                    std::cout << "No se pudo abrir el archivo htiHorario.txt\n";
                }

                while(materias.peek() != -1){ // Leer materias mientras no se llegue al final del archivo
                    programarMateria(materias, htiMaterias);
                }

                // Programar las horas libres
                int cantHorasHti = 0; // Para contar cuántas horas hti se van a programar
                htiMaterias.seekg(0, std::ios_base::beg); // Volver al principio de htiMaterias.txt
                while(!htiMaterias.eof()){
                    htiMaterias.ignore(16, '/'); // Ignorar el código de la materia
                    char* cantHti = new char[3];
                    htiMaterias.getline(cantHti, 16); // Leer la cantidad de Hti
                    int i = 0;
                    while(cantHti[i] != '\0')  i++; //Contar cuántos chars representan las hti
                    if(i == 1) cantHorasHti += (cantHti[0] - '0');
                    if(i == 2) cantHorasHti += ((cantHti[0] - '0')*10) + (cantHti[1]-'0');
                }
                htiMaterias.close();


                //Mientras hayan horas HTI pendientes para programar
                while(cantHorasHti > 0){
                    int lineHtiMat = 0; // Para guardar en qué linea del archivo htiMaterias.txt está
                    for(int col = 0; col < 7 ; col++){ // Iterar sobre las columnas de horario.txt
                        for(int fila = 0; fila < 15; fila++){  // Iterar sobre las filas de horario.txt
                            bool libre = horario('E', fila, col); // Bool que guarda si la hora especificada está libre
                            if(libre){
                                std::ifstream htiMateriasIN("htiMaterias.txt", std::ifstream::in); //Volver a abrir htiMaterias.txt para evitar bugs
                                if(!htiMateriasIN.is_open()){
                                    std::cout << "No se pudo abrir el archivo htiMaterias.txt\n";
                                }
                                    if(lineHtiMat>0) htiMateriasIN.seekg(lineHtiMat + 2);
                                    int fin = htiMateriasIN.peek();
                                    if(fin == -1){
                                        lineHtiMat = 0;
                                        htiMateriasIN.seekg(lineHtiMat);
                                    }
                                    // Extraer código de la materia
                                    char* codigoMateria = new char[16];
                                    htiMateriasIN.getline(codigoMateria, 8, '/');
                                    std::streampos posFinCode = htiMateriasIN.tellg();
                                    codigoMateria[0] = 'I'; // El código 1234567 se vuelve I234567 para indicar que es una HTI

                                    //Obtener las HTI de htiMaterias.txt
                                    char* cantHti = new char[4];
                                    int hti;
                                    htiMateriasIN.getline(cantHti, 16); // Leer la cantidad de HTI. Lee hasta la siguiente linea.
                                    hti = ((cantHti[0] - '0')*10) + (cantHti[1]-'0'); // Calcular las hti

                                htiMateriasIN.close();

                                std::fstream htiMateriasOUT("htiMaterias.txt", std::ios::in | std::ios::out);
                                if(!htiMateriasOUT.is_open()){
                                    std::cout << "No se pudo abrir el archivo htiMaterias.txt\n";
                                }

                                    // Si hay HTI pendietes para programar
                                    if(hti>0){ // Se programa la HTI y se resta 1 a las horas para programar
                                        horario('M', fila, col, codigoMateria);
                                        hti--;
                                        cantHorasHti--;
                                    }

                                    // Volver a guardar las HTI a htiMaterias.txt
                                    if(lineHtiMat == 0) htiMateriasOUT.seekp(lineHtiMat + 8);
                                    else if(lineHtiMat > 0) htiMateriasOUT.seekp(lineHtiMat + 10); // Escribir al comienzo de hti
                                    if(hti < 10) {
                                        char cero[1] = {'0'};
                                        htiMateriasOUT.write(cero, 1);
                                    }
                                    htiMateriasOUT << hti;
                                    lineHtiMat =  htiMateriasOUT.tellp();
                                htiMateriasOUT.close();
                            }
                        }
                    }
                }


                htiMaterias.close();
                materias.close();
                htiHorario.close();
            break;}
        case 'O':{
                std::ifstream file("horario.txt");
                    if (file.is_open()) {
                        const int MAX_SIZE = 100;
                        char buffer[MAX_SIZE];
                        while (file.getline(buffer, MAX_SIZE)) {
                            std::cout << buffer << std::endl;
                        }
                        file.close();
                    }
                break;}
        }

    }







    return 0;
}

