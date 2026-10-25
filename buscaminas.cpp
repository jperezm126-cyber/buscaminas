/*
Fercho - Buscaminas Completo (Con Historial de Movimientos)
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;

// Función para validar la entrada de números enteros (evita bucles infinitos con letras)
bool leerEntero(int &val) {
    if (!(cin >> val)) {
        cin.clear(); 
        cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

// Obtener la mayor puntuación almacenada en archivo
int cargarRecord() {
    ifstream archivo("record_buscaminas.txt");
    int record = 0;
    if (archivo.is_open()) {
        archivo >> record;
        archivo.close();
    }
    return record;
}

// Guardar una nueva puntuación si supera la anterior
void guardarRecord(int puntajeActual) {
    int recordActual = cargarRecord();
    if (puntajeActual > recordActual) {
        ofstream archivo("record_buscaminas.txt");
        if (archivo.is_open()) {
            archivo << puntajeActual;
            archivo.close();
            cout << "   ¡NUEVO RECORD HISTORICO REGISTRADO!\n";
        }
    }
}

int main() {
    srand(time(NULL));
    bool reiniciar = true;

    while (reiniciar) {
        int opcionNivel = 0;
        int tamano = 0;
        int vidas = 0;
        int intentos = 0;
        bool fin = false;

        // Vector para almacenar el historial de movimientos de la partida actual
        vector<string> historialMovimientos;

        system("cls");
        cout << "\n         Bienvenido a Buscaminas Fercho\n\n";
        cout << "   Record Historico Actual: " << cargarRecord() << " Puntos\n\n";
        cout << "   Selecciona todas las casillas libres sin agotar tus vidas.\n\n";

        // Menú de dificultad con validación de entrada
        do {
            cout << "   Nivel             Vidas           Opcion\n\n";
            cout << "   Practica          1000            1\n";
            cout << "   Facil             8               2\n";
            cout << "   Medio             5               3\n";
            cout << "   Dificil           3               4\n";
            cout << "   Muy Dificil       1               5\n\n";
            cout << "   elije un nivel (1-5): ";
            
            if (!leerEntero(opcionNivel) || opcionNivel < 1 || opcionNivel > 5) {
                system("cls");
                cout << "   Opcion invalida. Por favor ingresa un numero entre 1 y 5.\n\n";
                opcionNivel = 0;
            }
        } while (opcionNivel < 1 || opcionNivel > 5);

        switch (opcionNivel) {
            case 1: tamano = 10; vidas = 1000; break;
            case 2: tamano = 8;  vidas = 8;    break;
            case 3: tamano = 6;  vidas = 5;    break;
            case 4: tamano = 6;  vidas = 3;    break;
            case 5: tamano = 5;  vidas = 1;    break;
        }

        // Tableros
        vector<vector<int> > m(tamano, vector<int>(tamano));
        vector<vector<int> > valNumeros(tamano, vector<int>(tamano)); 
        vector<vector<string> > mT(tamano, vector<string>(tamano, "#"));

        int casillasSinMina = 0;
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                m[i][j] = rand() % 2; // 1 = Mina, 0 = Segura
                valNumeros[i][j] = (rand() % 8) + 1; // Número aleatorio del 1 al 8 para casillas seguras
                if (m[i][j] == 0) {
                    casillasSinMina++;
                }
            }
        }

        int casillasReveladasSeguras = 0;

        // Bucle principal del juego
        while (!fin) {
            system("cls");
            cout << "                 Buscaminas\n";
            cout << "     Intentos: " << intentos << "      Vidas: " << vidas << "\n\n";

            // Coordenadas Horizontales (Columnas: 0, 1, 2, ...)
            cout << "     ";
            for (int j = 0; j < tamano; j++) {
                cout << j << "  ";
            }
            cout << "\n\n";

            // Coordenadas Verticales (Filas: 0, 1, 2, ...) + Tablero
            for (int i = 0; i < tamano; i++) {
                cout << "  " << i << "  ";
                for (int j = 0; j < tamano; j++) {
                    cout << mT[i][j] << "  ";
                }
                cout << "\n";
            }
            cout << "\n";

            // Mostrar el historial de movimientos realizados en esta partida
            if (!historialMovimientos.empty()) {
                cout << "   --- movimientos que hiciste;
                for (size_t k = 0; k < historialMovimientos.size(); k++) {
                    cout << "   " << historialMovimientos[k] << "\n";
                }
                cout << "   --------------------------------\n\n";
            }

            // Condición de Victoria
            if (casillasReveladasSeguras >= casillasSinMina) {
                int puntaje = (casillasReveladasSeguras * 100) - (intentos * 10);
                if (puntaje < 0) puntaje = 0;

                cout << "   ganaste ro";
                cout << "   Puntuacion obtenida: " << puntaje << " Puntos\n\n";
                guardarRecord(puntaje);
                fin = true;
                break;
            }

            int fila = -2;
            int columna = -2;

            // Fila
            do {
                cout << "   Digite la fila (0 a " << tamano - 1 << " o -1 para ver minas): ";
                if (!leerEntero(fila)) {
                    cout << "   Opcion invalida. Debes ingresar un numero.\n";
                    fila = -2;
                    continue;
                }

                // Truco para revelar minas (-1)
                if (fila == -1) {
                    cout << "\n   --- POSICION DE LAS MINAS (TRUCO) ---\n   ";
                    for (int i = 0; i < tamano; i++) {
                        for (int j = 0; j < tamano; j++) {
                            if (m[i][j] == 1) {
                                cout << "[" << i << "," << j << "] ";
                            }
                        }
                    }
                    cout << "\n   -------------------------------------\n\n";
                    
                    // Registrar el truco en el historial
                    historialMovimientos.push_back("Truco usado: Revelar posiciones de minas");
                    
                    fila = -2;
                    continue;
                }

                if (fila < 0 || fila >= tamano) {
                    cout << "   Opcion invalida. Fila fuera de rango.\n";
                }
            } while (fila < 0 || fila >= tamano);

            // Columna
            do {
                cout << "   Digite la columna (0 a " << tamano - 1 << "): ";
                if (!leerEntero(columna)) {
                    cout << "   Opcion invalida. Debes ingresar un numero.\n";
                    columna = -2;
                    continue;
                }

                if (columna < 0 || columna >= tamano) {
                    cout << "   Opcion invalida. Columna fuera de rango.\n";
                }
            } while (columna < 0 || columna >= tamano);

            // Verificar si ya fue descubierta
            if (mT[fila][columna] != "#") {
                cout << "   Esa casilla ya ha sido descubierta.\n";
                Sleep(1200);
                continue;
            }

            intentos++;

            // Revelar Casilla y registrar en historial
            if (m[fila][columna] == 0) {
                // Revela número aleatorio entre 1 y 8
                char numChar = '0' + valNumeros[fila][columna];
                string s(1, numChar);
                mT[fila][columna] = s;
                casillasReveladasSeguras++;

                // Registro del movimiento seguro
                char fChar = '0' + fila;
                char cChar = '0' + columna;
                string registro = "Turno " + string(1, '0' + intentos) + ": Fila " + string(1, fChar) + ", Col " + string(1, cChar) + " -> Segura [" + s + "]";
                historialMovimientos.push_back(registro);

            } else {
                mT[fila][columna] = "*";
                vidas--;

                // Registro del impacto con mina
                char fChar = '0' + fila;
                char cChar = '0' + columna;
                string registro = "Turno " + string(1, '0' + intentos) + ": Fila " + string(1, fChar) + ", Col " + string(1, cChar) + " -> MINA (*)";
                historialMovimientos.push_back(registro);

                if (vidas <= 0) {
                    system("cls");
                    int puntaje = (casillasReveladasSeguras * 50);
                    cout << "                 GAME OVER\n";
                    cout << "             Has perdido todas tus vidas.\n";
                    cout << "   Puntuacion final: " << puntaje << " Puntos\n\n";

                    // Mostrar historial completo al perder
                    cout << "   --- HISTORIAL DE LA PARTIDA ---\n";
                    for (size_t k = 0; k < historialMovimientos.size(); k++) {
                        cout << "   " << historialMovimientos[k] << "\n";
                    }
                    cout << "   -------------------------------\n\n";

                    guardarRecord(puntaje);
                    fin = true;
                }
            }
        }

        // Menú final de reinicio (1 Si, 2 No)
        int opcionReinicio = 0;
        do {
            cout << "\n   ¿Deseas volver a jugar?\n";
            cout << "   1. Si\n";
            cout << "   2. No\n";
            cout << "   Escribe tu opcion (1 o 2): ";
            
            if (!leerEntero(opcionReinicio) || (opcionReinicio != 1 && opcionReinicio != 2)) {
                cout << "   Opcion invalida. Ingresa 1 para Si o 2 para No.\n";
                opcionReinicio = 0;
            }
        } while (opcionReinicio != 1 && opcionReinicio != 2);

        if (opcionReinicio == 2) {
            reiniciar = false;
        }
    }

    cout << "\n   ¡Gracias por jugar Buscaminas Fercho!\n";
    return 0;
}
