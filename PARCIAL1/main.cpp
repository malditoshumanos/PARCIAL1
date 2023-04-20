#include <iostream>
#include <fstream>

void guardarMateria(char mode);
bool horario(char mode, int row = 0, int col = 0, const char* code = nullptr);
int programarMateria(std::fstream& materias);

int main()
{

    /*
    horario('V');

    char * addCode = new char[7];
    for (int i = 0; i < 7; i++) {
        addCode[i] = '0';
        if (i == 6) addCode[i] = '1';
    }
    std::cout << addCode << '\n';
    horario('M', 14, 6, addCode);


    //guardarMateria('A');
    //guardarMateria('O');

    std::fstream materias("materias.txt", std::fstream::in);
    if(!materias.is_open()){
        std::cout << "No se pudo abrir el archivo de materias para lectura.\n";
    }

    int hti1 = programarMateria(materias);
    int hti2 = programarMateria(materias);
    int hti3 = programarMateria(materias);

    materias.close();

    bool huh = horario('E', 12, 5);
    std::cout << huh;
    */

    std::cout << "***PROGRAMA PARA ORGANIZAR EL TIEMPO***" << '\n';
    char principalMenu;
    while(principalMenu != 'X'){
        std::cout << "Ingrese una opcion: \n"
                "   I: Introducir materias\n"
                "   C: Mostrar horario solo con las clases\n"
                "   O: Mostras horario con sigerencias de tiempos de estudio\n"
                "   >> ";
        std::cin >> principalMenu;
        if(principalMenu != 'I' || principalMenu != 'C' || principalMenu != 'O') std::cout << "Ingrese una opcion valida!\n";
        switch(principalMenu){
            case 'I':
                char IMenu;
                std::cout << "  Desea guardar una actividad académica (A) o de otro tipo (O)?\n"
                             "  >> ";
                std::cin >> IMenu;
                //TODOTODOTODO: Haga que el programa empiece desde 0 cada vez que se inicie (borre todo el contenido de los archivos que este usa)
                //Ver chatgtp para saber como implementar el contador global de materias no académicas en guardarMateria.
                // En este mismo caso (I), se deben introducir las horas libres. Cree un array de ints de tamaño definido
                //      por las filas del archivo de materias para guardar las horas hti a asignar.
                //      Asigne las horas HTI. Considere crear un archivo con los códigos de las horas HTI de cada materia (nuevo modo de guardarMateria()? si usa return en un método lo termina) o tal vez un solo archivo con lineas que solo tengan código y nombre??? :ooo)
                // Implemente una función que imprima el horario. Debe leer (el? :OOO) los archivos de las materias y de las HTI para saber el
                //          nombre de la actividad correspondiente a cada código. Debe recibir el archivo del horario a imprimir.
                if(IMenu )
                break;
        }
    }

    return 0;
}




// Esta función recibe la string que contiene la información de la materia, guarda las horas de clase en el horario y retorna HTI
int programarMateria(std::fstream& materias){ // Se debe pasar el objeto fstream por referencia a la función

    char* infoMateria = new char[17]; // Máximo 17 caracteres representan los datos a guardar
    char* codigoMateria = new char[7]; // Para guardar el codigo de la materia
    int hti;

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
    if(mode == 'O') materias.ignore(5, '\n'); // Cuando está en modo O y no hay créditos, se va a la siguiente fila
    int i = 0;
    while(infoMateria[i] != '\0')  i++; //Contar cuántos chars representan el horario
    int horasClase = i/3; // Cada hora de clase está representada por 3 caracteres
    std::cout << "  Cantidad de horas de clase: " << horasClase << '\n';

    int indiceDia;
    int indiceHora;
    for(int j = 0; j < i; j+=3){ // Para cada hora de clase...
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

        hti = (creditos*48/16) - horasClase;
    }

    if(mode == 'O'){
        hti = -1; // la función retorna -1 si se procesó una catividad no académica
    }
    return hti;
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

    //TODO: agregar automáticamente el código en MODO O
    std::cout << "Ingrese los datos de la materia: ";
    std::cout << "Codigo: ";
    std::cin >> infoMateria;
    outFile << infoMateria;
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
