#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include "CTurtle.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
/*---------- VARIABLES LOCALES Y STRUCT---------------*/
using namespace std;
using namespace cturtle;

int puntaje = 0;
int opcionMenuInt;
string EnterContinuar;
typedef struct {
    string usuario;
    string contrasena; 
    int puntaje;
} tInformacion;
/*------------------- FINAL VARIABLES LOCALES Y STRUCT---------------*/

/*----------------- TODOS LOS PROTOTIPOS--------------*/
void menu();
void dibujarAhorcado(Turtle& t, int intentos);

void menuDificultad(string &pathSeleccionado);
void CargarPalabrasSecretas(const string &path, vector<string> &dataPalabraSecreta);

void Mayusculas(string &mayus);
void VerificarAcertado(string palabra, char letra, string &palabraAdivinada, bool &acertado);
void ValidacionEsPalabra(string &palabra);
void ValidacionOpcionMenu(int &opcionMenuInt);
void ValidacionAlfa(string caracterespecial, char &letra);
void JugarAgain(bool &seguirJugando);
void EntradaValida(string &entrada, bool &entradaValida);
void ImpVict(string palabra,int puntaje);
void ImpDerr(vector<tInformacion> ListaJugadores,string palabra,int puntaje, int posicionFind);
void MuerteInstantanea(bool &muerteInstantanea, int &intentosRestantes, Turtle &t);
void GanarAutomaticamente(string &palabraAdivinada,string &entrada, string &palabra, bool &muerteInstantanea, int &intentosRestantes, Turtle &t,int & puntaje, int OpcionMenuInt);
void VerificarJugada(string &palabraAdivinada, string &entrada, string &palabra, bool &muerteInstantanea, int &intentosRestantes, Turtle &t, bool &acertado, string caracterEspeciales, bool &entradaValida, char &letra, int &puntaje, int OpcionMenuInt);
bool NumeroValido(const string &entrada);
void ValidacionRounds(string entradaUser, bool &TieneDecimales, int &limiteTurno);
void ejecutarModoUnJugador(vector<tInformacion> &ListaJugadores, int posicionFind, Turtle &t);
void ejecutarModoDosJugadores(Turtle &t);

void CargarInformacion(vector<tInformacion>& ListaJugadores);
bool existeUsuario(vector<tInformacion> ListaJugadores,string buscarUsuario, int &posicionFind);
void existeContrasena(vector<tInformacion> ListaJugadores, string buscarContrasena, bool &encontrado, int &posicionFind);
void agregarUsuario(vector<tInformacion>& ListaJugadores,string &opcionRegistro);
void iniciarSesion(vector<tInformacion>& ListaJugadores, bool encontrado, int &posicionFind, string &opcionRegistro);

void PuntajeCalc(vector<tInformacion>& ListaJugadores, int puntaje, int posicionFind);
bool CompararPuntaje(const tInformacion &JugadorA, const tInformacion &JugadorB);
void OrdenamientoScores(vector<tInformacion>& ListaJugadores);
void ImpTop(vector<tInformacion> ListaJugadores, int posicionFind);

/*------------------- FINAL TODOS LOS PROTOTIPOS---------------*/


/*---------------------------- VOIDS PRINCIPALES-----------------------*/

void menu(){
    int opcionMenu;
    cout << " " << endl;
    cout << "--- BIENVENIDO AL JUEGO DEL AHORCADO ---" << endl;
    cout << "--- SELECCION DE JUEGO ---" << endl;
    cout << "[1] Un Jugador" << endl;
    cout << "[2] Dos Jugadores" << endl;
    cout << "[3] Top scores" << endl;
    cout << "[4] Creditos y salir" << endl;
    cout << "Opcion: ";
}
void dibujarAhorcado(Turtle& t, int intentos) {
    t.pencolor({255, 255, 255});
    t.width(3);
    t.speed(TS_FASTEST);
    
    // EL POSTE QUE SIEMPRE TIENE QUE ESTAR AHI :D
    
    t.penup(); t.goTo(-100, -150); t.pendown();
    t.goTo(0, -150);          
    t.goTo(-50, -150); t.goTo(-50, 150); 
    t.goTo(50, 150);          
    t.goTo(50, 110); // 

  	// CABEZA :D
    if (intentos <= 5) {
        t.penup(); t.goTo(65, 95); t.setheading(90); t.pendown();
        t.circle(15); 
    }
    // TORSO :D
    if (intentos <= 4) {
        t.penup(); t.goTo(50, 80); t.pendown();
        t.goTo(50, 10);
    }
    // BRAZO 1
    if (intentos <= 3) {
        t.penup(); t.goTo(50, 65); t.pendown();
        t.goTo(25, 45);
    }
    // BRAZO 2
    if (intentos <= 2) {
        t.penup(); t.goTo(50, 65); t.pendown();
        t.goTo(75, 45);
    }
    // PIERNA 1
    if (intentos <= 1) {
        t.penup(); t.goTo(50, 10); t.pendown();
        t.goTo(25, -40);
    }
    //PIERNA 2
    if (intentos <= 0) {
        t.penup(); t.goTo(50, 10); t.pendown();
        t.goTo(75, -40);
    }
}

/*---------------------------- FINAL VOIDS PRINCIPALES-----------------------*/

/* ------------------Seleccion de dificultad ----------------*/

void menuDificultad(string &pathSeleccionado) {
    int opcionDif;
    cout << "\n--- SELECCION DE DIFICULTAD ---" << endl;
    cout << "[1] Facil" << endl;
    cout << "[2] Medio" << endl;
    cout << "[3] Dificil" << endl;
    cout << "Seleccione una opcion: ";
    ValidacionOpcionMenu(opcionDif); 

    switch(opcionDif) {
        /* Documentos de texto con las dificultades*/
        case 1: pathSeleccionado = "palabras_facil.txt"; break; 
        case 2: pathSeleccionado = "palabras_medio.txt"; break;
        case 3: pathSeleccionado = "palabras_dificil.txt"; break;
        default: 
            cout << "Opcion invalida. Se selecciono por defecto: Medio." << endl;
            pathSeleccionado = "palabras_medio.txt";
            break;
    }
    system("cls");
}

void CargarPalabrasSecretas(const string &path, vector<string> &dataPalabraSecreta) {
    dataPalabraSecreta.clear(); // Limpiamos para jugar otra partida sin estorbos en la terminal
    ifstream file;
    string palabrasTabla;
    file.open(path, ios::in);
    /*En caso de que no se encuentre el archivo o haya fallos, se cargan palabras de respaldo para evitar que se cierre el codigo 
    y pueda seguir la partida*/
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " ". Cargando palabras por defecto..." << endl;
        dataPalabraSecreta = {"PROGRAMACION", "POLITECNICA", "LINUX", "COMPUTADORA"};
        return;
    }

    while (getline(file, palabrasTabla)) {
        stringstream ss(palabrasTabla);
        while (getline(ss, palabrasTabla, ',')) {
            dataPalabraSecreta.push_back(palabrasTabla);
        }
    }
    file.close();
}

/*----------------- VOIDS PARA SIMPLIFICAR MAIN--------------*/
void Mayusculas(string &mayus){
    for (int i = 0; i < mayus.length(); i++) {
        mayus[i] = toupper(mayus[i]);
    }
}
void VerificarAcertado(string palabra, char letra, string &palabraAdivinada, bool &acertado){
    for (int i = 0; i < palabra.length(); i++) {
        if (palabra[i] == letra) {
            palabraAdivinada[i] = letra;
            acertado = true;
        }
    }
}
void ValidacionEsPalabra(string &palabra){
    bool entradaPalabraValida = true;
    while (entradaPalabraValida) {
        getline(cin,palabra);
        /*Filtra si contiene caracteres numéricos*/
        bool noEsPalabra = false;
        for (int i = 0; i < palabra.length(); i++) {
            if (!isalpha(palabra[i])) {
                noEsPalabra = true;
                break;
            }
        }

        if (noEsPalabra) {
            cout << "Por Favor, ingrese una palabra: ";
            continue; // Regresa al inicio del while para pedir otra palabra
        } else if (!noEsPalabra){
            /*Hacer mayúsculas*/
            Mayusculas(palabra);
            entradaPalabraValida = false;
        } /*Llave del condicional noEsPalabra*/
    } /*llave del while entradPalabraValida*/
}

void ValidacionOpcionMenu(int &opcionMenuInt){
    bool opcionValida = false;
    string opcionMenuValidacion;
    while (!opcionValida) { 
        cin >> opcionMenuValidacion;
        /*Filtra si contiene caracteres no numéricos*/
        bool NotieneNumero = false;
        for (int i = 0; i < opcionMenuValidacion.length(); i++) {
            if (!isdigit(opcionMenuValidacion[i])) {
                NotieneNumero = true;
                break;
            }
        }

        if (NotieneNumero) {
            cout << "ERROR - NO EXISTE ESA OPCION: ";
            continue; // Regresa al inicio del while para pedir otra opcion del menu
        } else {
            opcionMenuInt = stoi(opcionMenuValidacion);
            opcionValida = true; // Todo en orden, salimos del bucle de validación
        }

    }
}
void ValidacionAlfa(string caracterespecial, char &letra){
    char mayuscula;
    while ((caracterespecial.find(letra) != string::npos) || (isdigit(letra))){
            cout << "Por favor, intenta ingresar una letra o la palabra completa: ";
            cin >> letra; 
            mayuscula = toupper(letra);
            letra = mayuscula;
        /*AQUI BORRAMOS TODO RESIDUO PARA LA SIGUIENTE LECTURA*/
            cin.ignore(100, '\n');
        }
}
void JugarAgain(bool &seguirJugando){

    string seleccionOpcion, mayusSeleccion;
    cout << "--- JUGAR DE NUEVO ---" << endl;
    cout << "[Y/N]: "; 
    getline(cin, seleccionOpcion);
    Mayusculas(seleccionOpcion);
    while ((seleccionOpcion != "Y") && (seleccionOpcion != "N")){
        cout << "Opcion invalida, intente de nuevo: " << endl;
        cout << "--- JUGAR DE NUEVO ---" << endl;
        cout << "[Y/N]: "; 
        getline(cin, seleccionOpcion);
        Mayusculas(seleccionOpcion); 
        } /*Llave del while(seleccion)*/
    if (seleccionOpcion == "Y"){
        puntaje = 0;
        seguirJugando = true;
    } else if (seleccionOpcion == "N"){
        seguirJugando = false; 
        }
    system("cls");
}
void EntradaValida(string &entrada, bool &entradaValida){
    for (int i=0; i<entrada.length(); i++)
    {
        if (!isalpha(entrada[i]))
        {
            entradaValida=false;
            break;    
        }    
    }
    if (!entradaValida){    
        cout<<"Recuerde ingresar solo letras, los simbolos y numeros no son validos. "<<endl;
    }
}
void ImpVict(string palabra,int puntaje){
    cout << "\n--- ¡FELICIDADES, ADIVINASTE! ---" << endl;
    cout << "La palabra era: " << palabra << endl;
    cout <<"Hasta ahora llevas: " << puntaje << " puntos" << endl;

}
void ImpDerr(vector<tInformacion> ListaJugadores,string palabra,int puntaje, int posicionFind){
    cout << "\n--- GAME OVER ---"<< endl; 
    cout << "Te has quedado sin intentos." << endl; 
    cout << "La palabra era: " << palabra << endl;
    PuntajeCalc(ListaJugadores,puntaje,posicionFind);
    cout << "Tu puntaje total fue de: " << puntaje << endl;
    
    ImpTop(ListaJugadores,posicionFind);
}

void MuerteInstantanea(bool &muerteInstantanea, int &intentosRestantes, Turtle &t){
    cout << "¡Palabra incorrecta! Muerte instantanea." << endl;
    muerteInstantanea = true;
    intentosRestantes = 0;
    dibujarAhorcado(t, 0);
}
void GanarAutomaticamente(string &palabraAdivinada,string &entrada, string &palabra, bool &muerteInstantanea, int &intentosRestantes, Turtle &t,int & puntaje, int OpcionMenuInt){
    if (entrada == palabra && OpcionMenuInt == 1) {
        palabraAdivinada = palabra;
    }else if (entrada != palabra && OpcionMenuInt == 1){
        MuerteInstantanea(muerteInstantanea, intentosRestantes, t); 
    } else if (entrada == palabra && OpcionMenuInt == 2){
        palabraAdivinada = palabra;
    } else if(entrada != palabra && OpcionMenuInt == 2){
        MuerteInstantanea(muerteInstantanea, intentosRestantes, t);
    }
}
void VerificarJugada(string &palabraAdivinada, string &entrada, string &palabra, bool &muerteInstantanea, int &intentosRestantes, Turtle &t, bool &acertado, string caracterEspeciales, bool &entradaValida, char &letra, int &puntaje, int OpcionMenuInt) {
    if (entrada.length() > 1) {
        // Si tiene más de una letra y es alfabético, intenta adivinar la palabra completa
        GanarAutomaticamente(palabraAdivinada, entrada, palabra, muerteInstantanea, intentosRestantes, t, puntaje, OpcionMenuInt);
    } else {
        // Si es una sola letra (y ya sabemos que es una letra válida por el filtro previo)
        char letraJugada = entrada[0]; 
        acertado = false;
        VerificarAcertado(palabra, letraJugada, palabraAdivinada, acertado);
        system("cls");
        if (!acertado) {
            intentosRestantes--;
            cout << "¡Incorrecto! Perdiste una vida." << endl;
        } else {
            cout << "¡Bien hecho! Acertaste una letra." << endl;
        }
        dibujarAhorcado(t, intentosRestantes);
    }
}
bool NumeroValido(const string &entrada){
    if (entrada.empty()) return false;
    for (char c : entrada){
        if (!isdigit(c)){
            return false;
        }
    }
    return true;
}
void ValidacionRounds(string entradaUser, bool &TieneDecimales, int &limiteTurno){
    if (!NumeroValido(entradaUser)){ // Validamos que la entrada sea un número entero positivo
        TieneDecimales = true;
        cout << "Por Favor, ingrese solo numeros enteros positivos y pares: ";
        return;
    }
    int n = stoi(entradaUser); //transformamos el string a int para poder evaluar si es par o impar

    if (n <= 0 || n % 2 != 0){
        TieneDecimales = true;
        cout << "Por Favor, ingrese un numero de rondas positivo y par: ";
        return;
    }

    TieneDecimales = false;
    limiteTurno = n;
}

void ejecutarModoUnJugador(vector<tInformacion> &ListaJugadores, int posicionFind, Turtle &t, string opcionRegistro) {
    system("cls");  
    cout << "===========================================================" << endl; 
    cout << "PARA ESTE MODO ADIVINA LA MAYOR CANTIDAD DE PALABRAS POSIBLE" << endl; 
    

    string pathPalabras;
    vector<string> dataPalabraSecreta;
    menuDificultad(pathPalabras);
    CargarPalabrasSecretas(pathPalabras, dataPalabraSecreta);

    if(dataPalabraSecreta.empty()) {
        cout << "No hay palabras disponibles para jugar." << endl;
        return;
    }

    bool seguirjugando = true;
    int puntajeLocal = 0;

    while (seguirjugando){ 
        t.reset(); 
        int posicion = rand() % dataPalabraSecreta.size(); 
        string palabra = dataPalabraSecreta[posicion]; 
        string palabraAdivinada(palabra.length(), '_'); 
        
        int intentosRestantes = 6;
        bool muerteInstantanea = false;
        bool acertado = false;
        string entrada;
        char letra = ' ';
        const string caracterEspeciales = "+!#$%&/()=?¡°|||¨*[]ñ_:.-{},;<'>\"\'\\";

        dibujarAhorcado(t, intentosRestantes);

        while (intentosRestantes > 0 && palabraAdivinada != palabra){
            cout << "\nPalabra: " << palabraAdivinada << endl;
            cout << "Intentos restantes: " << intentosRestantes << endl;
            cout << "Introduce una letra o palabra: ";
            cin >> entrada;
        
            Mayusculas(entrada);
            bool entradaValida = true;  
            EntradaValida(entrada, entradaValida); 
            if(!entradaValida){
                continue;
            }
            VerificarJugada(palabraAdivinada, entrada, palabra, muerteInstantanea, intentosRestantes, t, acertado, caracterEspeciales, entradaValida, letra, puntajeLocal, 1); 
        }

        if(palabra == palabraAdivinada){
            system("cls");
            puntajeLocal = puntajeLocal + 100;
            ImpVict(palabra,puntajeLocal);
            system("pause");
            system("cls");
            
        } else if (palabra != palabraAdivinada && opcionRegistro == "Y"){
            ImpDerr(ListaJugadores,palabra,puntajeLocal,posicionFind);
            cin.ignore(1000, '\n'); 
            JugarAgain(seguirjugando);
        } else if (palabra != palabraAdivinada && opcionRegistro == "N"){
            cin.ignore(1000, '\n'); 
            JugarAgain(seguirjugando);
            if(seguirjugando){
                puntajeLocal = 0;
            }
        }
    }  
}
void ejecutarModoDosJugadores(Turtle &t) {
    system("cls");
    cin.ignore(10000, '\n');
    bool seguirjugando = true;
    const string caracterEspeciales = "+!#$%&/()=?¡°|||¨*[]ñ_:.-{},;<'>\"\'\\";
    
    while (seguirjugando){
        system("cls");
        bool tieneDecimales = true;
        cout << "===========================================================" << endl;
        cout << "PARA ESTE MODO DECIDAN ENTRE USTEDES: Jugador Uno | Jugador Dos" << endl;
        cout << "ESCOJAN EL NUMERO PAR DE RONDAS A JUGAR: ";
        string entradaUser;
        int limiteTurno = 0;
        t.reset();
        while (tieneDecimales) {
            getline(cin, entradaUser);
            ValidacionRounds(entradaUser, tieneDecimales, limiteTurno);
        }

        int turnoJugadores = 0;
        int puntajePlayer1 = 0;
        int puntajePlayer2 = 0;
        bool BucleInternoRondas = true;
        
        while (BucleInternoRondas){
            string palabra, palabraAdivinada, entrada;
            char letra = ' ';
            int intentosRestantes = 6;
            bool acertado = false;

            cout <<"INGRESE UNA PALABRA, EL OTRO TIENE QUE ADIVINAR: "; 
            ValidacionEsPalabra(palabra);
            system("cls");

            palabraAdivinada = string(palabra.length(), '_');
            cout <<"- ADIVINA LA PALABRA DE TU AMIGO! -" <<endl;
            t.reset();
            
            bool muerteInstantanea = false;
            dibujarAhorcado(t, intentosRestantes);
            
            while (intentosRestantes > 0 && palabraAdivinada != palabra) {
                cout << "\nPalabra: " << palabraAdivinada << endl;
                cout << "Intentos restantes: " << intentosRestantes << endl;
                cout << "Introduce una letra o palabra: ";
                cin >> entrada;
            
                Mayusculas(entrada);
                bool entradaValida = true;
                EntradaValida(entrada, entradaValida); 
                if(!entradaValida){
                    continue;
                }
                VerificarJugada(palabraAdivinada, entrada, palabra, muerteInstantanea, intentosRestantes, t, acertado, caracterEspeciales, entradaValida, letra, puntaje, 2); 
            }
            
            cin.ignore(10000, '\n');
            if (palabraAdivinada == palabra) {
                cout << "\n--- ¡FELICIDADES, ADIVINASTE! ---" << endl;
                if (turnoJugadores % 2 == 0) puntajePlayer1++;
                else puntajePlayer2++;
            } else {
                cout << "\n--- NO ADIVINASTE---"<< endl;  
            }
            
            cout << "La palabra era: " << palabra << endl;
            cout << "====================================" << endl;
            cout << "--- PUNTAJES ACTUALES ---" << endl;
            cout << "Jugador Uno: " << puntajePlayer1 << endl;
            cout << "Jugador Dos: " << puntajePlayer2 << endl;
            turnoJugadores++;
            cout << "====================================" << endl;
            
            if (turnoJugadores == limiteTurno){
                BucleInternoRondas = false;
                cout << "\n====================================" << endl;
                cout << "--- PUNTAJES FINALES ---" << endl;
                cout << "Jugador Uno: " << puntajePlayer1 << endl;
                cout << "Jugador Dos: " << puntajePlayer2 << endl;
                cout << "--- RESULTADO ---" << endl;
                if (puntajePlayer1 > puntajePlayer2) cout << "Jugador Uno es el GANADOR" << endl;
                else if (puntajePlayer1 < puntajePlayer2) cout << "Jugador Dos es el GANADOR" << endl;
                else cout << "EMPATE" << endl;
                cout << "====================================" << endl;
            }
        } 
        JugarAgain(seguirjugando);
    } 
}


/*------------------- FINAL VOIDS SIMPLIFICAR----------------------*/

/*--------------- STRUCTS INFORMACION JUGADOR-----------------------*/
void CargarInformacion(vector<tInformacion>& ListaJugadores){
    string path = "listaJugadores.csv";
    ifstream fileCargar; 
    string IngresoStruct;
    fileCargar.open(path, ios::in);
    
    while(getline(fileCargar, IngresoStruct)){
        tInformacion CargarDatos;
        stringstream ss(IngresoStruct);
        string nombreCargar;
        string contraCargar;
        string puntajeCargar;
        if((getline(ss,nombreCargar,',')) && (getline(ss,contraCargar,',')) && (getline(ss,puntajeCargar))){
            CargarDatos.usuario = nombreCargar;
            CargarDatos.contrasena = contraCargar;
            CargarDatos.puntaje = stoi(puntajeCargar);

            ListaJugadores.push_back(CargarDatos);
        }
    }

}

bool existeUsuario(vector<tInformacion> ListaJugadores,string buscarUsuario, int &posicionFind) {
    string nombre;
    
    for(int i = 0; i < ListaJugadores.size(); i++){
        if (ListaJugadores.at(i).usuario == buscarUsuario) {
            posicionFind = i;
            return true;
        } 
    }
    return false;
}

void existeContrasena(vector<tInformacion> ListaJugadores, string buscarContrasena, bool &encontrado, int &posicionFind) {
    string contrasena;
    for(int i = 0; i < ListaJugadores.size(); i++){
        if (ListaJugadores.at(i).contrasena == buscarContrasena) {
            encontrado = true;
            posicionFind = i;
            break;
        } else if(ListaJugadores.at(i).contrasena != buscarContrasena && (i+1 == ListaJugadores.size())){
            encontrado = false;
        }
    }
}

void agregarUsuario(vector<tInformacion>& ListaJugadores,string &opcionRegistro) {
    tInformacion nuevoIngreso;
    string nombreNuevo;
    string contraNueva;
    string puntajeNuevo = "0";
    bool usuarioRepetido;
    int posicionFind;

    do {
        cout << "Ingrese el nombre para su usuario: ";
        getline(cin, nombreNuevo);
        /*NO DEBE EXISTIR EL MISMO NOMBRE DE USUARIO, O HABRÁ ERRORES AL MOMENTO DE COMPARAR PUNTAJES Y NOMBRES*/
        
        usuarioRepetido = existeUsuario(ListaJugadores, nombreNuevo, posicionFind);

        if (usuarioRepetido) {
            cout << "Este nombre ya es utilizado por otro usuario." << endl;
        }
    } while (usuarioRepetido);

    cout << "Ingrese una contrasenia: ";
    getline(cin,contraNueva);
    

    ofstream fileIngresar;
    string path = "listaJugadores.csv";

    nuevoIngreso.usuario = nombreNuevo;
    nuevoIngreso.contrasena = contraNueva;
    nuevoIngreso.puntaje = stoi(puntajeNuevo);
    ListaJugadores.push_back(nuevoIngreso);

    fileIngresar.open("listaJugadores.csv", ios::app);
    if(fileIngresar.is_open()){
        fileIngresar << nuevoIngreso.usuario << "," << nuevoIngreso.contrasena << "," << nuevoIngreso.puntaje << endl;
        fileIngresar.close();
        cout << "Usuario creado correctamente \n";
        opcionRegistro = "Y";
    } else {
        cout << "Error al abrir el archivo para escribir." << endl;
    }

}

void iniciarSesion(vector<tInformacion>& ListaJugadores, bool encontrado, int &posicionFind, string &opcionRegistro) {
    
    string nombre;
    string contrasena;

    cout << "Ingrese el nombre de su usuario: ";
    cin >> nombre;
    
    encontrado = existeUsuario(ListaJugadores, nombre, posicionFind);
    cin.ignore(1000000, '\n');
    
    if(encontrado != true) {
        cout << "Usuario no encontrado" << endl;

        
        cout << "¿Desea registrar su usuario? Marque [Y/N]: ";
        getline(cin, opcionRegistro);
        Mayusculas(opcionRegistro);

        while (opcionRegistro != "Y" && opcionRegistro != "N") {
            cout << "Opcion no valida. Ingrese de nuevo su opcion [Y/N]: ";
            getline(cin,opcionRegistro);
            Mayusculas(opcionRegistro);
        }

        if (opcionRegistro == "Y") {
            agregarUsuario(ListaJugadores, opcionRegistro);
            existeUsuario(ListaJugadores, nombre, posicionFind);
        }
        if (opcionRegistro == "N") {
            cout << "ADVERTENCIA: No se guardaran tus puntajes en nuestros registros." << endl; 
            system("pause");
            system("cls");
        }
    }
    
    if (encontrado == true) {
        cout << "Ingrese su contrasenia: ";
        cin >> contrasena;
        
        while(ListaJugadores.at(posicionFind).contrasena != contrasena) {
            cout << "Contrasenia incorrecta" << endl;
            cout << "Vuelva a ingresar la contrasenia: ";
            cin >> contrasena;
        }
        
        cout << "Ingreso exitoso. Bienvenid@, " << ListaJugadores.at(posicionFind).usuario << endl;
        opcionRegistro = "Y";
        system("pause");
        system("cls");
        
    }
}
/*-------------------------- FINAL INFORMACION JUGADOR-----------------------*/

/*----------------------- STRUCT CALCULO DE PUNTAJES--------------------*/
void PuntajeCalc(vector<tInformacion>& ListaJugadores, int puntaje, int posicionFind){
    ofstream filePuntaje;
    string path = "listaJugadores.csv";
    if(puntaje > ListaJugadores.at(posicionFind).puntaje){

        ListaJugadores.at(posicionFind).puntaje = puntaje;
        filePuntaje.open(path);
        for(int i = 0; i < ListaJugadores.size(); i++){
            if (filePuntaje.is_open()) {
                filePuntaje << ListaJugadores.at(i).usuario << "," << ListaJugadores.at(i).contrasena  << "," << ListaJugadores.at(i).puntaje << "\n";
            } else {
                cout << "Error al abrir el archivo para escribir." << endl;
            }   
        }
        filePuntaje.close();
    }
}

bool CompararPuntaje(const tInformacion &JugadorA, const tInformacion &JugadorB) {
    return JugadorA.puntaje > JugadorB.puntaje; 
}
void OrdenamientoScores(vector<tInformacion>& ListaJugadores){
    /*Limpiamos EL VECTOR que guarda la info de los jugadores mientras corre el juego, 
    esto para que luego volvamos a cargar la información, así actualizamos la lista 
    en tiempo real cuando se haya registrado un usuario nuevo y su puntaje se haya
    actualizado*/
    ListaJugadores.clear();
    CargarInformacion(ListaJugadores);

    
    
    sort(ListaJugadores.begin(), ListaJugadores.end(), CompararPuntaje);

    cout << "\n=============================================" << endl;
    cout << right << setw(30) << "--- TOP SCORES ---" << endl;
    cout << "=============================================" << endl; //45 doble guiones
    
    for (int i = 0; i < ListaJugadores.size(); i++) {

        cout << left << setw(4) << to_string(i + 1) + ".";
        
        
        cout << left << setw(20) << ListaJugadores[i].usuario;
        
        
        cout << right << setw(15) << ListaJugadores[i].puntaje << " pts" << endl;
    }
    cout << "=============================================" << endl;

    system("pause");
    system("cls");
}
void ImpTop(vector<tInformacion> ListaJugadores, int posicionFind){

    string usuarioObjetivo = ListaJugadores.at(posicionFind).usuario;

    sort(ListaJugadores.begin(), ListaJugadores.end(), CompararPuntaje);

    cout << "\n=============================================" << endl;

    for (int i = 0; i < ListaJugadores.size(); i++) {
        if(usuarioObjetivo == ListaJugadores.at(i).usuario){
        cout << "Estas en el top: " << i + 1 << ". " << endl;
        break;
        }
    }
    cout << "=============================================" << endl;
}

/*---------------------- FINAL CALCULO DE PUNTAJES ------------------*/


int main() {

    vector<tInformacion> ListaJugadores; 
    CargarInformacion(ListaJugadores); 
    bool acceso = false; 
    int posicionFind = -1; 
    string opcionRegistro = "";
    iniciarSesion(ListaJugadores, acceso, posicionFind, opcionRegistro);



    TurtleScreen screen;
    screen.bgcolor({30, 30, 30});
    Turtle t(screen);
    t.hideturtle();

    srand(time(0));

    do {
        menu(); 
        ValidacionOpcionMenu(opcionMenuInt);
        
        switch (opcionMenuInt) { 
            case 1:
                ejecutarModoUnJugador(ListaJugadores, posicionFind, t, opcionRegistro);
                break;

            case 2:
                ejecutarModoDosJugadores(t);
                break;
                 
            case 3:
                system("cls");
                OrdenamientoScores(ListaJugadores);
                break;
                
            case 4:
                system("cls");
                cout << "--- GRACIAS POR JUGAR ---" << endl;
                cout << "--- CREDITOS FINALES ---" << endl;
                cout << "- William Chuquimarca" << endl;
                cout << "- Cristina Granda" << endl;
                cout << "- Jael Pacheco" << endl;
                cout << "- Sergio Betancourt" << endl;
                
                break;

            default:
                cout << "ERROR - NO EXISTE ESA OPCION: " << endl;
        }
    } while (opcionMenuInt != 4);

    system("pause");
    return 0;
}