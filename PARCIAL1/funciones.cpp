#include "funciones.h"

extern int contMateriasO; // Contador de materias no académicas inscritas

// Esta función recibe la string que contiene la información de la materia, guarda las horas de clase en el horario y retorna HTI
void programarMateria(std::fstream& materias, std::fstream& htiMaterias){ // Se debe pasar el objeto fstream por referencia a la función

    char* infoMateria = new char[32]; // Para guardar el horario/los créditos de la materia
    char* codigoMateria = new char[7]; // Para guardar el codigo de la materia
    //char* nombreMateria = new char[32]; // Para guardar el nombre de la materia
    int hti; // Para guardar las hti

    materias.getline(codigoMateria, 17, '/'); //Obtener código de la materia
    std::cout << "Procesando nueva materia...";
    std::cout << "  Codigo: " << codigoMateria << '\n';

    // Determinar tipo de actividad
    char mode = 'A'; // Se inicia en modo "materia de la U (modo académico)"
    if(codigoMateria[0] == '0' &&
        codigoMateria[1] == '0' &&
        codigoMateria[2] == '0' &&
        codigoMateria[3] == '0' &&
        codigoMateria[4] == '0') mode = 'O'; // Se establece el modo "otro tipo de actividades" si el codigo empieza por 00000

    materias.getline(infoMateria, 17, '/'); //Obtener nombre de la materia
    std::cout << "  Nombre: " << infoMateria << '\n';

    materias.getline(infoMateria, 17, '/'); //Obtener horario de la materia
    std::cout << "  Horario: " << infoMateria << '\n';

    if(mode == 'O') materias.ignore(5, '\n'); // Cuando está en modo O (y no hay créditos), se va a la siguiente fila

    int i = 0;
    while(infoMateria[i] != '\0')  i++; //Contar cuántos chars representan el horario
    int horasClase = i/3; // Cada hora de clase está representada por 3 caracteres
    std::cout << "  Cantidad de horas de clase: " << horasClase << '\n';
    int indiceDia;
    int indiceHora;
    for(int j = 0; j < i; j+=3){ // Programar cada hora de clase...
        char diaDeSemana = infoMateria[j];
        switch(diaDeSemana){
            case 'L':
                indiceDia = 0;
                break;
            case 'M':
                indiceDia = 1;
                break;
            case 'W':
                indiceDia = 2;
                break;
            case 'J':
                indiceDia = 3;
                break;
            case 'V':
                indiceDia = 4;
                break;
            case 'S':
                indiceDia = 5;
                break;
            case 'D':
                indiceDia = 6;
                break;
        }
        indiceHora = (infoMateria[j+1] - '0')*10 + (infoMateria[j+2] - '0'); // Extraer numero de la hora
        std::cout << "  Programando clase... ";
        std::cout << "  Dia: " << indiceDia << ", ";
        std::cout << "Hora: " << indiceHora << '\n';
        indiceHora-=7; // Se resta 7 pues mis horas activas empiezan a las 7am
        horario('M', indiceHora, indiceDia, codigoMateria);
    }

    if(mode == 'A'){
        materias.getline(infoMateria, 17); //Obtener numero de créditos de la materia
        std::cout << "  Creditos: " << infoMateria[0] << '\n';
        int creditos = infoMateria[0] - '0';

        hti = (creditos*48/16) - horasClase; // Calcular las hti

        // Almacenar las hti
        htiMaterias << codigoMateria;
        htiMaterias << '/';
        if(hti<10){
            htiMaterias << '0';      //el formato de htiMaterias.txt es 1234567/09 ó 1234567/11
        }
        htiMaterias << hti;

        htiMaterias << '\n';
    }
}






// row, col es la posición de la hora a modificar. code es el código de la actividad a introducir (y mide 7 chars).
// Modo V: Crear horario vacío // Modo M: Guardar hora de clase // Modo E: Examinar si la hora dada está libre
bool horario(char mode, int row, int col, const char* code){
// Si hay parámetros predeterminados en la función, estos se ponen en la declaración PERO NO EN LA DEFINICIÓN de la función
    bool isFree = 0; // Bool para retornar cuando está en modo 'E'
    std::fstream outFile;

    // Note que las lineas miden 57 (incluyendo \n)
    int indFila = 57 * row; // Indice del comienzo de la fila que quiero modificar
    int indCol = 8 * col; // Indice dentro de la fila de la hora que quiero modificar
    int ind = indFila + indCol; // Indice en el archivo

    if(mode == 'V'){ // Crear horario vacío
        outFile.open("horario.txt", std::fstream::out | std::fstream::in | std::fstream::trunc); // Se abre el archivo en modo trunc para descartar los contenidos previos
        if(!outFile.is_open()){
            std::cout << "No se pudo abrir el archivo para escritura.\n";
        }
        outFile.seekg(0);
        for(int i = 0; i < 15; i++) outFile << "7777777/7777777/7777777/7777777/7777777/7777777/7777777\n";
    }

    if(mode == 'M'){ // Modificar horario
        outFile.open("horario.txt", std::fstream::out | std::fstream::in);
        if(!outFile.is_open()){
            std::cout << "No se pudo abrir el archivo para escritura.\n";
        }
        if(code == nullptr) std::cout << "ERROR: No se ingresó adecuadamente el código de la actividad. \n";

        outFile.seekg(ind);

        for(int i = 0; i < 7; i++){
           outFile << code[i];
        }
    }

    if(mode == 'E'){
        outFile.open("horario.txt", std::fstream::in);
        if(!outFile.is_open()){
            std::cout << "No se pudo abrir el archivo para escritura.\n";
        }

        char* codigoEvaluado = new char[7];
        outFile.seekg(ind);
        outFile.getline(codigoEvaluado, 7);

        for(int k = 0; k < 6; k++){
            if(codigoEvaluado[k] != '7') {
                isFree = 0;
                break;
            }
            else isFree = 1;
        }
    }

    outFile.close();
    return isFree;
}






// Modo O: otro tipo de actividad (no académica) // Modo A: Actividad académica
void guardarMateria(char mode){
    std::fstream outFile("materias.txt", std::fstream::out | std::fstream::app);
    if(!outFile.is_open()){
        std::cout << "No se pudo abrir el archivo para escritura.\n";
    }
    char* infoMateria = new char[32]; // Máximo 32 caracteres representan los datos a guardar

    std::cout << "Ingrese los datos de la materia: ";

    if(mode == 'A'){ // Al guardar materia académica se debe específicar el código
        std::cout << "Codigo: ";
        std::cin >> infoMateria;
        outFile << infoMateria;
    } else if (mode == 'O'){ // Al guardar actividad académica el código se pone automáticamente
        outFile << "000000";
        outFile << contMateriasO;
        contMateriasO++;
    }
    outFile << '/';


    std::cout << "Nombre: ";
    std::cin >> infoMateria;
    outFile << infoMateria;
    outFile << '/';

    while(true){ // Guardar horas de clase
        std::cout << "Horas de clase (ej. de formato: L08, W16)(X para dejar de introducir horas de clase): ";
        std::cin >> infoMateria;
        if(infoMateria[0] == 'X' || infoMateria[0] == 'x') break;
        outFile << infoMateria;
    }
    outFile << '/';

    if(mode == 'A'){ // Si es materia académica tiene créditos
        std::cout << "Numero de creditos: ";
        std::cin >> infoMateria;
        outFile << infoMateria;
        outFile << '/';
    }

    outFile << '\n';
    outFile.close();
}
